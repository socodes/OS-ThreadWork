/*
*   @StudentName: Berkay Aktunç
*   @StudentNumber: 1862499
*   @NameofTask: Programming Assignment # 2 Question1
*/

/*
to run
    gcc -pthread -o {output file name} {c file}
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Global variable declarations
pthread_mutex_t myMutex;
int m=0, offset=0;
int **matrix;

// Function declarations
void print(char*);
void* bottomTopShift(void*);
void* leftRightShift(void*);
int **readFile(FILE *);


int main() {
    int i=0, j=0, d=0, s=0;
    FILE *file;

    file = fopen("input.txt","r");

    if(!file)
        printf("No file!");
    else
    {
        fscanf(file,"%d\n", &m);
        matrix = readFile(file);
    }
    fclose(file);

    do{
        printf("Enter d value: ");
        scanf("%d", &d);
        if(d<1 || d>m)
            printf("D should be bigger than zero\n     and smaller than M value which is %d\n",m);
    }while(d<1 || d>m);
    printf("Enter s value: ");
    scanf("%d", &s);

    offset = m/d;
    pthread_t threads[d];
    pthread_mutex_init(&myMutex, NULL);

    printMatrix("Orjinal");

    // Create thread for left-right shift
    for (i = 0; i < d; i++){
        if( m - (i*offset) >= offset )
            pthread_create(&threads[i], NULL, leftRightShift, (int) (i*offset));
        else{
            offset += m - (i*offset);
            pthread_create(&threads[i], NULL, leftRightShift, (int) (i*offset));
        }
    }
    
    // Join created threads to work together
    for(j = 0; j < s; j++){
        for (i = 0; i < d; i++){
            pthread_join(threads[i], NULL);
        }
    }

    offset = m/d;

    // Create thread for bottom-top shift
    for (i = 0; i < d; i++){
        if( m - (i*offset) >= offset )
            pthread_create(&threads[i], NULL, bottomTopShift, (int) (i*offset));
        else{
            offset += m - (i*offset); 
            pthread_create(&threads[i], NULL, bottomTopShift, (int) (i*offset));
        }
    }

    // Join created threads to work together
    for(j = 0; j < s; j++){
        for (i = 0; i < d; i++){
            pthread_join(threads[i], NULL);
        }
    }

    printMatrix("Shifted");
}


void printMatrix(char *txt) {
    int i=0, j=0;

    printf("\n%s:\n",txt);
    for(i = 0; i < m; i++) {
        printf("\t");
        for(j = 0; j < m; j++)
            printf("%d ",matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}


void* bottomTopShift(void* column) {
    int i=0, j=0, temp=0;
    pthread_mutex_lock(&myMutex);
	
    for(j = (int)column; j < ((int)column+offset); j++) {
        temp = matrix[ 0 ][ j ];
 
        for(i = 0; i < m-1; i++)
            matrix[ i ][ j ] = matrix[i+1][ j ];
 
        matrix[ m-1 ][ j ] = temp;
    }

    pthread_mutex_unlock( &myMutex );
}


void* leftRightShift(void* line) {
    int i=0, j=0, temp=0;
    pthread_mutex_lock( &myMutex );
    
    for(j = (int)line; j < ((int)line+offset); j++) {
        temp = matrix[ j ][ m-1 ];
 
        for(i = (m-1); i > 0; i--)
            matrix[ j ][ i ] = matrix[ j ][ i-1 ];
 
        matrix[ j ][ 0 ] = temp;
    }

    pthread_mutex_unlock(&myMutex);
}


int **readFile(FILE *file) {
    int i=0, j=0;
    
    matrix = (int**) malloc (m*sizeof(int*));
    
    for(i = 0; i < m; i++)
        matrix[i] = (int *)malloc(m * sizeof(int));

    for(i = 0; i < m; i++) {
        for(j = 0; j < m; j++)
            fscanf(file,"%d",&matrix[i][j]);
        fscanf(file,"\n");
    }

    return matrix;
}