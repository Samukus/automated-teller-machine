#include <stdio.h>
#include <sys/time.h>
#include "defs.h"

int main(){
    struct timeval time;
    gettimeofday(&time, NULL);
    time.tv_usec;
    srand(time.tv_usec);

	printf("FILE_PATH:%s\n",FILE_PATH);
    FILE *fp;
    fp=fopen(FILE_PATH,"w");
	if(fp == NULL){
		perror("Fopen");
		return -1;
	}

    fwrite("------ ID ------|-Pin\n", 22, 1, fp);

    int i,j;
    for(j=0;j<500;j++){
        char buf_id[16]="";
        char buf_pin[4]="";
        char buf[21]="";
        for(i=0;i<16;i++)
            sprintf(buf_id+i,"%d",rand()%10);
        for(i=0;i<4;i++)
            sprintf(buf_pin+i,"%d",rand()%10);
        strncpy(buf,buf_id,16);
        buf[16]=' ';
        strncpy(buf+17,buf_pin,4);
        buf[21]='\n';
        printf("Str#%d: %s",j+1,buf);
        fwrite(buf, 22, 1, fp);
    }

    close(fp);


    return 0;

};
