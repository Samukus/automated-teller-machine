#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "defs.h"

struct message{
    int len;
    int sleep_time;
    char message_text[MSG_BUF_LEN-1];
} buf_message;

struct sockaddr_in server_addr;

int current_port;

int signal;

pthread_mutex_t shared_mutex;

int socket_init_udp(int *sock,int port);

int create_message(char *message);

int try_verification(struct sockaddr_in server_addr,int current_port);

void get_server_addr();

int parse_message(char *message);


