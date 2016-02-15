#include "server.h"

int main(){

    printf("This is Server\n");

    int broadcast_socket;
    socket_init_broadcast(&udp_broadcast_arg.brcst_sock,PORT_UDP);

    pthread_t thread_brcst;
    pthread_create(&thread_brcst, NULL, udp_broadcast, &udp_broadcast_arg);

    pthread_t thread_lstn_snd;
    int code_sender = 0;
    pthread_create(&thread_lstn_snd, NULL, listner, &code_sender);

    sleep(1);
    print_port();
    console();



    return 0;
}
