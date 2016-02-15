#include "server.h"

void print_help(){
    printf("Comands: q - shut down the server\n");
    printf("         p - print current port for clients\n");
    printf("         l - print number of clients online\n");
    printf("         c - clear console\n");
    printf("         h - print this help\n\n");
    return;
};

void print_port(){
    printf("Port for clients:     %d\n",udp_broadcast_arg.port_current);
};

void print_clients(){
    printf("Number of clients connected: %d of %d\n",udp_broadcast_arg.number_of_clients,MAX_NUMBER_CLIENTS);
}

void console(){
    int c;
    while(1){
        c=getc(stdin);
        switch(c) {
            case 'p'    :
                print_port();
            break;
            case 'q'    :
                pthread_mutex_lock(&shared_mutex);
                exit = 1;
                pthread_mutex_unlock(&shared_mutex);
                printf("Server shuting down\n");
                printf("5..\n");
                sleep(1);
                printf("4..\n");
                sleep(1);
                printf("3..\n");
                sleep(1);
                printf("2..\n");
                sleep(1);
                printf("1..\n");
                sleep(1);
                close(udp_broadcast_arg.brcst_sock);
                pthread_mutex_unlock(&shared_mutex);

                return;
            break;
            case 'c'    :
                printf("\033[2J"); // Clear screen
                printf("\033[0;0H"); //Go to 0:0
            break;
            case 'l'    :
                print_clients();
            break;
            case 'h'    :
                print_help();
            break;
        }
    }
};

size_t socket_init_broadcast(int *sock,int port){

    int BroadcastPermission = 1;

    if ( (*sock = socket(AF_INET,SOCK_DGRAM,0)) < 0){
        perror("Error opening socket");
        return -1;
    }

    if ( setsockopt(*sock, SOL_SOCKET, SO_BROADCAST, (char *)&BroadcastPermission,sizeof(BroadcastPermission)) < 0 ) {
        perror("Error setsockopt to broadcast");
        return -1;
    }

    return 0;
};

size_t send_msg(int sock,struct sockaddr_in addr,char *msg,size_t len){
    size_t send_bytes = sendto (sock,msg, len, 0,(struct sockaddr *)&addr, sizeof (addr));
    if( (int)send_bytes < 0 ){
        perror("Sending\n");
    }
//    printf("To port(%d) Message:%s --- bytes[%d]\n",(int)ntohs(addr.sin_port),msg,send_bytes);
    return send_bytes;
};

void *udp_broadcast(void *arg){

    /*Struct addres for broadcast to senders*/
    struct sockaddr_in broadcast_senders;
    broadcast_senders.sin_family = AF_INET;
    broadcast_senders.sin_port = htons(PORT_UDP);
    broadcast_senders.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    while(1){


        pthread_mutex_lock(&shared_mutex);

        if( udp_broadcast_arg.number_of_clients < MAX_NUMBER_CLIENTS )
            udp_broadcast_arg.signal = 3;
        else
            udp_broadcast_arg.signal = 1;

        if(exit == 1)
           udp_broadcast_arg.signal = 2;

        pthread_mutex_unlock(&shared_mutex);


        char msg[64]="";
        sprintf(msg,"%d#%d\0",udp_broadcast_arg.signal,udp_broadcast_arg.port_current);
        int bytes = send_msg(udp_broadcast_arg.brcst_sock,
                 broadcast_senders,
                 msg,
                 strlen(msg) );

        usleep(BROADCAST_TIMEOUT);
    }
};

void *listner(void *arg){
    int code = *(int *)arg;
    struct sockaddr_in serv_addr,cli_addr; // структура сокета сервера и клиента

    while(1){
        int	sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
           perror("ERROR opening socket");
           return -1;
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY; // сервер принимает подключения на все IP-адреса
        serv_addr.sin_port = 0;

        bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

        socklen_t len = sizeof(serv_addr);
        if (getsockname(sockfd, (struct sockaddr *)&serv_addr, &len) == -1) {
            perror("Getsockname");
            continue;
        }

        udp_broadcast_arg.port_current = (int) ntohs(serv_addr.sin_port);

        listen(sockfd,1);

        int clilen;
        int newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0){
            perror("ERROR on accept");
            continue;
        }

        pthread_mutex_lock(&shared_mutex);
        udp_broadcast_arg.number_of_clients++;
        pthread_mutex_unlock(&shared_mutex);


        pthread_t client_thr;
        pthread_create(&client_thr, NULL, client_verification, &newsockfd);

        close(sockfd);
    }
    return 0;

};

void *client_verification(void *arg){
    int sock = *(int *)arg;
    int bytes_recv;
    int bytes_snd;
    char message[MSG_BUF_LEN]="";

	if ( (bytes_recv = read(sock,&message[0],sizeof(message))) < 0 )
        //perror("ERROR reading from socket");

    message[bytes_recv] = '\0';

    if(verification(message) == 0)
        bytes_snd = write(sock, "1", 1);
    else
        bytes_snd = write(sock, "0", 1);

    if(bytes_snd < 0)
        perror("ERROR sending to socket");

    pthread_mutex_lock(&shared_mutex);
    udp_broadcast_arg.number_of_clients--;
    pthread_mutex_unlock(&shared_mutex);

	close(sock);
};

int verification(char *msg){
    char buf[MSG_BUF_LEN]="";

    FILE *file = fopen(FILE_PATH, "r");
    if(file == NULL){
        perror("fopen");
        return -1;
    }
//    char message_buf[MSG_BUF_LEN]="";
//    strcpy(message_buf,msg);
    msg[strlen(msg)]='\0';

    while(!feof(file)){
        fgets(buf,MSG_BUF_LEN,file);
        buf[strlen(buf)]='\0';


        //printf("Msg: %s\nBuf: %s\n",msg,buf);
        //printf("Strcmp: %d\n\n",strcmp(buf,msg));

        if(strcmp(buf,msg) == 0 ){
            fclose(file);
            return 0;
        }
        else
            continue;
    }

    fclose(file);
    return -1;
    //    char buf_id[50];
//    char buf_pass[50];
//    for(i=0;buf[i]!=' ' ;i++)
//        buf_id[i]=buf[i];
//
//    buf_id[i]='\0';
//
//    if(strcmp(ID,buf_id) != 0){
//        printf("Acces denied!\n");
//        return -1;
//    }
//
//    strcpy(buf_pass,buf+strlen(buf_id)+1);
//    buf_pass[strlen(buf_pass)-1] = '\0';
//
//    if(strcmp(Password,buf_pass) != 0){
//        printf("Acces denied!\n");
//        return -1;
//    }
//    else
//        printf("Acces acepted!\n");

}
