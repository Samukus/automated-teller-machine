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

int thread_status;

pthread_mutex_t shared_mutex;
pthread_mutex_t status_mutex;

int socket_init_udp(int *sock,int port);

int create_message(char *message);

void try_verification();

void get_server_addr();

int parse_message(char *message);


