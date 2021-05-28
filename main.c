#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
//function declerations 
void printArray(int [], int );
void leftShift();
void upShift();

pthread_mutex_t myMutex;
int **arr,m,s;
int *routineLeft(void **received_arr){
    int **arr = (int *)received_arr;
    int i;
    for(i=0;i<m;i++){
        leftShift(i);
    }
}
int *routineUp(void **received_arr){
    int **arr = (int *)received_arr;
    int i;
    for(i=0;i<m;i++){
        upShift(i);
    }
}
int main(){

    int d,selection;
    int i,j,eachThread;
    FILE *finput;
    printf("1) Part 1\n");
    printf("2) Part 2\n");
    printf("Enter your selection: ");
    scanf("%d",&selection);
    printf("Select: %d\n",selection);
    if(selection == 1){

        printf("Enter number of threads(d): ");
        scanf("%d",&d);
        printf("Enter shift number(s): ");
        scanf("%d",&s); 

        
        finput = fopen("input.txt", "r"); //open file for reading.
        if (finput == NULL) // give error if an error occured while file opening.
        {
            printf("Sorry.. An error occured while opening the input file");
            return 0;
        }
        fscanf(finput, "%d", &m);
        printf("\nArray size(m):%d\n",m);

        
        eachThread = s/d; 
        arr =(int*) malloc(m*m*sizeof(int));
        for(i=0;i<m;i++){
            arr[i] = (int*) malloc(m*sizeof(int));
        }

        for (i = 0; i < m; i++)
        {
            for(j = 0; j < m; j++){
                fscanf(finput, "%d ", &arr[i][j]); //import numbers.
            }
            fscanf(finput, "\n");
            
        }
        fclose(finput);
        
        printf("\nMain array: \n");
        for(i=0;i<m;i++){
            printArray(arr[i],m);
        }
        printf("\n\n");
        pthread_t t[d];

        pthread_mutex_init(&myMutex,NULL);
        for(j=0;j<eachThread;j++){
            for(i=0;i<d;i++){
                if(pthread_create(&t[i],NULL,&routineLeft,(void**)arr) != 0){
                    printf("Error while creating threads\n");
                }
            }
        }
        for(j=0;j<eachThread;j++){
            for(i=0;i<d;i++){
                pthread_join(t[i],NULL);
            }
        }
        for(j=0;j<eachThread;j++){
            for(i=0;i<d;i++){
                if(pthread_create(&t[i],NULL,&routineUp,(void**)arr) != 0){
                    printf("Error while creating threads\n");
                }
            }
        }
        for(j=0;j<eachThread;j++){
            for(i=0;i<d;i++){
                pthread_join(t[i],NULL);
            }
        }
        printf("\nFinal Array:\n");
        for(i=0;i<m;i++){
            printArray(arr[i],m);
        }
    }
    return 0;
}
void printArray(int arr[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}
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

void upShift(int column){
    int i=0, temp=0;
    pthread_mutex_lock(&myMutex);
    
    temp = arr[0][column];
    for(i=0; i<m-1; i++)
        arr[i][column]= arr[i+1][column];
    arr[m-1][column]=temp;
    pthread_mutex_unlock(&myMutex);

}
