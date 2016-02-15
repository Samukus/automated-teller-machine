#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED
#include "defs.h"
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/msg.h>


size_t socket_init_broadcast(int *sock,int port);

size_t send_msg(int sock,struct sockaddr_in addr,char *msg,size_t len);

void *udp_broadcast_recievers(void *arg);

void *udp_broadcast(void *arg);

void *listner(void *arg);

void *client_verification(void *arg);

void console();

void print_help();

void print_port();

void print_queue();

void queue_init();

void add_message(char *message);

int get_message(char *message);

int verification(char *msg);

pthread_mutex_t shared_mutex;


struct thread_broadcasting_argument{
    int port_current;
    int number_of_clients;
    int brcst_sock;
    int signal;
} udp_broadcast_arg;

int exit;

#endif // SERVER_H_INCLUDED
