#include "client.h"

int main(){
    signal=2;
    pthread_t thread_brcst;
    pthread_create(&thread_brcst, NULL, get_server_addr, NULL);

    pthread_t thread_try_verification;

    while(1){
        if( signal == 2 ){
            sleep(1);
            clear_console();
            printf("Server is offline\nPlease wait...\n");
            if(thread_status == 1)
                pthread_kill(thread_try_verification, 0);
            continue;
        }

        if( thread_status == 0 && signal == 3){
            pthread_create(&thread_try_verification, NULL, try_verification, NULL);
        }
    }

    return 0;
}


