#include "client.h"

void clear_console(){
    printf("\033[2J"); // Clear screen
    printf("\033[0;0H"); //Go to 0:0
};

char *scan_id_pin(char *message){
    char ID[16]="";
    char pin[4]="";

    printf("ID:");
    scanf("%s",ID);
    printf("pin:");
    scanf("%s",pin);
    ID[strlen(ID)]=' ';
    strcpy(ID + strlen(ID),pin);

    strcpy(message,ID);
    return message;
};

int socket_init_udp(int *sock,int port){

    if ((*sock=socket(AF_INET,SOCK_DGRAM,0))<0){
        perror("Opening socket");
        return -1;
    }

    struct timeval t;
    t.tv_sec = 0;
    t.tv_usec = 500000;
    if(setsockopt(*sock, SOL_SOCKET, SO_RCVTIMEO, &t, sizeof(t)) == -1){
        perror("Setting SO_RCVTIMEO option in UDP socket for destination RX: ");
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    int i = 0;
    while(bind(*sock,(struct sockaddr *)&server, sizeof(server)) < 0){
        perror("Binding error");
        continue;
    }
    return 0;
};

void get_server_addr(){

    char buf_message[MSG_BUF_LEN]="";
    int socket_rcv;
    int port = PORT_UDP;

    while(1){

        socket_init_udp(&socket_rcv,port);

        if(socket_rcv < 0){
            close(socket_rcv);
            perror("init");
        }

        pthread_mutex_lock(&shared_mutex);
        //bzero((char *) &server_addr, sizeof(struct sockaddr_in));

        int socket_len = sizeof(struct sockaddr_in);

        int n = recvfrom(socket_rcv,buf_message,sizeof(buf_message),NULL,&server_addr,&socket_len);

        if( n <= 0 ){
            close(socket_rcv);
            signal = 2;
            perror("recieve");
            pthread_mutex_unlock(&shared_mutex);
            continue;
        }
        //printf("Message:%s\n",buf_message);
        signal = atoi(buf_message);
        current_port = atoi(buf_message+2);

        pthread_mutex_unlock(&shared_mutex);

        printf("Signal: %d   Port: %d\n\n",signal,current_port);
        close(socket_rcv);
        sleep(1);
    }
};

int try_verification(struct sockaddr_in server_addr,int current_port){

    char message[MSG_BUF_LEN]="";
    char msg_buf[MSG_BUF_LEN]="";
    char ID[16]="";
    char pin[4]="";

    printf("ID:");
    gets(ID);
    printf("pin:");
    gets(pin);
    ID[strlen(ID)]=' ';

    strcpy(msg_buf,ID);
    strcpy(msg_buf + sizeof(ID)+1,pin);

    if(signal == 2){
        return -2;
    }

    pthread_mutex_lock(&shared_mutex);

    server_addr.sin_port = htons(current_port);

    int my_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (my_sock < 0){
        //perror("ERROR opening socket");
        close(my_sock);
        pthread_mutex_unlock(&shared_mutex);
        return -2;
    }

    if (connect(my_sock,(struct sockaddr *)&server_addr,sizeof(struct sockaddr_in)) < 0){
        //perror("ERROR connecting");
        close(my_sock);
        pthread_mutex_unlock(&shared_mutex);
        return -2;
    }

    char ipAddress[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(server_addr.sin_addr), ipAddress, INET_ADDRSTRLEN);

    pthread_mutex_unlock(&shared_mutex);


   // strcpy(msg_buf,message);
    msg_buf[strlen(msg_buf)] = '\n';
    //printf("Msg:%s",msg_buf);

    if(signal == 2){
        close(my_sock);
        return -2;
    }

    int bytes_snd = send(my_sock, msg_buf, strlen(msg_buf)+1, 0);
    if(bytes_snd >= 0);
        //printf("%-25s --> [%s:%d]\n",msg_buf,ipAddress,current_port);

    if(signal == 2){
        close(my_sock);
        return -2;
    }

    bzero(msg_buf,sizeof(msg_buf));
    int bytes_rcv = recv(my_sock, msg_buf, MSG_BUF_LEN, 0);
    if(bytes_rcv >= 0);
        //printf("%-25s <-- [%s:%d]\n",msg_buf,ipAddress,current_port);

    if(signal == 2){
        close(my_sock);
        return -2;
    }

    close(my_sock);

    if(msg_buf[0] == '1')
        return 0;
    else
        return -1;
};


