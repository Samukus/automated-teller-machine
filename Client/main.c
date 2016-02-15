#include "client.h"

int main(){

    pthread_t thread_brcst;
    pthread_create(&thread_brcst, NULL, get_server_addr, NULL);

    while(1){
        clear_console();
        if( signal == 2 || signal == 0 ){
            printf("Please wait...\n");
            sleep(1);
            continue;
        }

        int result = try_verification(server_addr,current_port);
        switch(result){
            case -1:
                printf("Verification failed\n");
            break;
            case -2:
                printf("Connection problem\n");
            break;
            case 0:
                printf("Verification successfuly complete!\n");
            break;
        }
        sleep(3);

    }

    return 0;
}


