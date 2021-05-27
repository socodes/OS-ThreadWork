#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>
#include<unistd.h>
//function declerations 
void printArray(int [], int );
int **arr,m;
int *routine(void **received_arr){
    int **arr = (int *)received_arr;
    int i;
    int **temp = arr;

    printf("\n");
    printf("m:%d\n",m);
    for(i=0;i<4;i++){
        printArray(temp[i],4);
    }
}
int main(){

    int d,s;
    printf("Enter number of threads(d): ");
    scanf("%d",&d);
    printf("Enter shift number(s): ");
    scanf("%d",&s); 

    FILE *finput;
    finput = fopen("input.txt", "r"); //open file for reading.
    if (finput == NULL) // give error if an error occured while file opening.
    {
        printf("Sorry.. An error occured while opening the input file");
        return 0;
    }
    fscanf(finput, "%d", &m);
    printf("\nArray size(m):%d\n",m);

    int i,j;
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
        
    }/*
    printf("\nMain array: \n");
    for(i=0;i<m;i++){
        printArray(arr[i],m);
    }*/
    printf("\n\n");
    pthread_t t[d];
    for(i=0;i<d;i++){
        if(pthread_create(&t[i],NULL,&routine,(void**)arr) != 0){
            printf("Error while creating threads\n");
        }
    }
    for(i=0;i<d;i++){
        pthread_join(t[i],NULL);
    }

    fclose(finput);
    return 0;
}
void printArray(int arr[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}
