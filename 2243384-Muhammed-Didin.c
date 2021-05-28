/*
    2243384 - Muhammed Didin
    May 2021

    -In order to run;
    -gcc -o [OutputFileName] [FileName] -lpthread
    
    -For example;
    -gcc -o main  2243384-Muhammed-Didin.c -lpthread -w

    -We have a simple menu that leads us. But it works only 1 time. You need to run everytime.


    -For Part 1;
    -Input file exactly be names as "input.txt"
    and its context should exactly be like:

    4
    7 6 5 3
    8 2 4 5
    7 3 1 9
    2 1 0 8

    -4 is the number of rows and columns.
    -Each line represents a row in matrix.

    -In case of any problem, I can make an interview and explain how the program works.

*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include <time.h>


//function declerations 
void printArray(int [], int );
void leftShift();
void upShift();
void pickup(int);
void putdown(int);
void test(int);
void initialization_code();

//Global variables
pthread_mutex_t myMutex;
int **arr,m,s;
enum{THINKING,HUNGRY,EATING} state[5]; 
pthread_cond_t condition[5];
pthread_mutex_t mutex;
float *timeArr;
time_t begin,end;
int forky;


//Routine function that runs in Part 2
void *routine(void *id){
    int myid = (int) id;
    int i=0;
    while (i != 50) {	
        
        if(forky==1){
            printf("Only 1 fork in the box left\n");
        }
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

//Routine function that runs in Part 1. Does Left Shift only in columns.
int *routineLeft(void **received_arr){
    int **arr = (int *)received_arr;
    int i;
    for(i=0;i<m;i++){
        leftShift(i); //Function that does left shift.
    }
}
//Routine function that runs in Part 1. Does Up Shift only in rows.
int *routineUp(void **received_arr){
    int **arr = (int *)received_arr;
    int i;
    for(i=0;i<m;i++){
        upShift(i);
    }
}

int main(){

    //Local variables
    int d,selection;
    int i,j,eachThread;
    FILE *finput;
    srand(time(NULL));
    int number;
    int maxID;
    printf("1) Part 1\n");
    printf("2) Part 2\n");
    printf("Enter your selection: ");
    scanf("%d",&selection);
    //Part 1 starts.
    if(selection == 1){
        printf("Enter number of threads(d): ");
        scanf("%d",&d);
        finput = fopen("input.txt", "r"); //open file for reading.
        if (finput == NULL) // give error if an error occured while file opening.
        {
            printf("Sorry.. An error occured while opening the input file");
            return 0;
        }
        fscanf(finput, "%d", &m);
        if(d>m  || d<1 ){
            printf("d should be bigger than zero and smaller than m value which is %d\n",m);
            return 1;
        }
            
        printf("Enter shift number(s): ");
        scanf("%d",&s); 

        

        printf("\nArray size(m):%d\n",m);

        
        //Create 2d array to read file.
        arr =(int*) malloc(m*m*sizeof(int));
        for(i=0;i<m;i++){
            arr[i] = (int*) malloc(m*sizeof(int));
        }
        //read file and assign it to our 2d array.
        for (i = 0; i < m; i++)
        {
            for(j = 0; j < m; j++){
                fscanf(finput, "%d ", &arr[i][j]); //import numbers.
            }
            fscanf(finput, "\n");
            
        }
        fclose(finput);
        //Print main array
        printf("\nMain array: \n");

        //I call the printArray function that writes 1d array in a for loop.
        //So that, it writes 2d array.
        for(i=0;i<m;i++){
            printArray(arr[i],m);
        }
        printf("\n");
        pthread_t t[d];
        eachThread = s/d; 
        pthread_mutex_init(&myMutex,NULL);

        //Threads that are responsible for Left Shifting. We have d times threads
        //and each thread is responsible for s/d columns.
        for(j=0;j<eachThread;j++){
            for(i=0;i<d;i++){
                if(pthread_create(&t[i],NULL,&routineLeft,(void**)arr) != 0){
                    printf("Error while creating threads\n");
                }
            }
        }
        //join threads that does left shifting
        for(j=0;j<eachThread;j++){
            for(i=0;i<d;i++){
                pthread_join(t[i],NULL);
            }
        }
        //Threads that are responsible for Up Shifting. We have d times threads
        //and each thread is responsible for s/d rows.
        for(j=0;j<eachThread;j++){
            for(i=0;i<d;i++){
                if(pthread_create(&t[i],NULL,&routineUp,(void**)arr) != 0){
                    printf("Error while creating threads\n");
                }
            }
        }
        //join threads that does up shifting
        for(j=0;j<eachThread;j++){
            for(i=0;i<d;i++){
                pthread_join(t[i],NULL);
            }
        }

        //Print Final array.
        //I call the printArray function that writes 1d array in a for loop.
        //So that, it writes 2d array.
        printf("\nFinal Array:\n");
        for(i=0;i<m;i++){
            printArray(arr[i],m);
        }
    }
    //Part 2 starts.
    else{
        printf("Enter number of philosophers: ");
        scanf("%d",&number);
        forky = number/3;
        pthread_t t[number];
        timeArr = (float*)malloc(number * sizeof(float));
        pthread_mutex_init(&mutex,NULL);

        //create threads for philosophers.
        for(i=1;i<number+1;i++){
            if(pthread_create(&t[i],NULL,&routine, i) != 0){
                printf("Error while creating threads\n");
            }
        }
        //join the threads.
        for(i=0;i<number;i++){
            pthread_join(t[i],NULL);
        }
        //Calculations about time.
        float total = 0,average;
        //find total time
        for(i=0;i<number;i++){
            total += timeArr[i];
        }
        //Calc. total and average.
        printf("Total: %f\n",total);
        average = total / number;
        printf("Average: %f\n",average);
        float max = timeArr[0];

        //find the max.
        for (i = 0; i < number; i++)
            if (timeArr[i] > max){
                max = timeArr[i];
                maxID = i;
            }
                
        printf("Max: %f with ID: %d\n",max,maxID+1);

        }
    return 0;
}
//Function that writes 1d array.
void printArray(int arr[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}
//Function that does left shift for specific line. 
//I call this function to do left shift for every line 
//by doing a for loop.
void leftShift(int line)
{
    int i=0, temp=0;
    pthread_mutex_lock(&myMutex);
    
    temp = arr[line][m-1];
    for(i=m-1; i>0; i--)
        arr[line][i]= arr[line][i-1];
    arr[line][0]=temp;

    pthread_mutex_unlock(&myMutex);
}
//Function that does up shift for specific line. 
//I call this function to do up shift for every line 
//by doing a for loop.
void upShift(int column){
    int i=0, temp=0;
    pthread_mutex_lock(&myMutex);
    
    temp = arr[0][column];
    for(i=0; i<m-1; i++)
        arr[i][column]= arr[i+1][column];
    arr[m-1][column]=temp;
    pthread_mutex_unlock(&myMutex);

}
//function that does the things for HUNGRY state.
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
    forky--;
}
//function that does the things for THINKING state.
void putdown (int i){
    pthread_mutex_lock(&mutex);
    state[i] = THINKING;
    sleep(rand()%5+1);
    printf("id: %d state: THINKING\n",i);
    
    test((i+4)%5);
    test((i+1)%5);
    pthread_mutex_unlock(&mutex);
    forky++;
}
//function that does the things for EATING state.
void test (int i){
    if((state[(i+4)%5] != EATING) && (state[(i+1)%5] != EATING) && (state[i] == HUNGRY) && forky>=1){
        state[i] = EATING;
        sleep(rand()%5+1);
        printf("id: %d state: EATING\n",i);
        time(&end);
        timeArr[i] += difftime(end, begin);
        pthread_cond_signal(&condition[i]);

    }
}
//function that does inits the Part 2.
void initialization_code(){
    for(int i = 0;i<5;i++){
        state[i] = THINKING;
        pthread_cond_init(&condition[i],NULL);
    }
}