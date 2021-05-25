#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void printArray(int arr[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}
void print2dArray(int **arr,int row,int col){
    int i,j;
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}
int main(){

    int d,s,m;
/*     printf("Enter number of threads(d): ");
    scanf("%d",&d);
    printf("Enter shift number(s): ");
    scanf("%d",&s); */

    FILE *finput;
    finput = fopen("input2.txt", "r"); //open file for reading.
    if (finput == NULL) // give error if an error occured while file opening.
    {
        printf("Sorry.. An error occured while opening the input file");
        return 0;
    }
    fscanf(finput, "%d", &m);
    printf("m:%d\n",m);

    int i,j;
    int **arr = (int*) malloc(m * sizeof(int));
    for(i=0;i<m;i++){
        arr[i]=(int*) malloc(m * sizeof(int)); //dynamic array that builds every row for survey
    }

    for (i = 0; i < m; i++)
    {
        for(j = 0; j < m; j++){
            fscanf(finput, "%d ", &arr[i][j]); //import numbers.
        }
        
        //fscanf(finput, "\n");
        printArray(arr[i],m);
    }
    print2dArray(arr,m,m);
    fclose(finput);
    return 0;
}