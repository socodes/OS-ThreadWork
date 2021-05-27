#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

int *routine(){
    printf("Start from thread\n");
    sleep(1);
    printf("Ending thread\n");
    return 1;
}
int main(){
    int number,i,res;
    printf("How many threads you want: ");
    scanf("%d",&number);
    pthread_t t[number];
    for(i=0;i<number;i++){
        if(pthread_create(&t[i],NULL,&routine,NULL) != 0){
            printf("Error while creating threads\n");
        }

    }
    for(i=0;i<number;i++){
        pthread_join(t[i],&res);
        printf("Number: %d\n",res);
    }
    return 0;
}