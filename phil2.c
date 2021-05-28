#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

enum{THINKING,HUNGRY,EATING} state[5]; pthread_cond_t condition[5];
pthread_mutex_t mutex;

void pickup(int);
void putdown(int);
void test(int);
void initialization_code();
float *timeArr;
time_t begin,end;


void *routine(void *id){
    int myid = (int) id;
    int i=0;
    while (i != 5) {	
        //THINK..

        pickup(myid);
        

        //EAT (use resources)        
        putdown(myid);

        //THINK..
        i++;
    }
    // Return reference to global variable:
    pthread_exit(&myid);

}
int main()
{
    srand(time(NULL));
    int number,i;
    printf("Enter number: ");
    scanf("%d",&number);
    pthread_t t[number];
    timeArr = (float*)malloc(number * sizeof(float));
    pthread_mutex_init(&mutex,NULL);
    for(i=1;i<number+1;i++){
        if(pthread_create(&t[i],NULL,&routine, i) != 0){
            printf("Error while creating threads\n");
        }
    }
    for(i=0;i<number;i++){
        pthread_join(t[i],NULL);
    }
    float total = 0,average;
    for(i=0;i<number;i++){
        total += timeArr[i];
    }
    printf("Total: %f\n",total);
    average = total / number;
    printf("Average: %f\n",average);
    float max = timeArr[0];
    int maxID;

    for (i = 0; i < number; i++)
        if (timeArr[i] > max){
            max = timeArr[i];
            maxID = i;
        }
            
    printf("Max: %f with ID: %d\n",max,maxID);


    return 0;
}



void pickup (int i){
    pthread_mutex_lock(&mutex);
    state[i] = HUNGRY;
    printf("id: %d state: HUNGRY\n",i);
    time(&begin);
    test(i);
    
    if(state[i] != EATING){
        
        pthread_cond_wait(&condition[i],&mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void putdown (int i){
    pthread_mutex_lock(&mutex);
    state[i] = THINKING;
    sleep(rand()%5+1);
    printf("id: %d state: THINKING\n",i);
    
    test((i+4)%5);
    test((i+1)%5);
    pthread_mutex_unlock(&mutex);
}

void test (int i){
    if((state[(i+4)%5] != EATING) && (state[(i+1)%5] != EATING) && (state[i] == HUNGRY)){
        state[i] = EATING;
        sleep(rand()%5+1);
        printf("id: %d state: EATING\n",i);
        time(&end);
        timeArr[i] += difftime(end, begin);
        //time[i] += hesaplanmış time;

        pthread_cond_signal(&condition[i]);
    }
}

void initialization_code(){
    for(int i = 0;i<5;i++){
        state[i] = THINKING;
        pthread_cond_init(&condition[i],NULL);
    }
}
