#include <pthread.h>
#include <stdio.h>

//whole vector will be of size NUMTHREADS*VECLEN
#define NUMTHREADS 40
#define VECLEN 100

pthread_mutex_t myMutex;
int *a;

void* doDotProduct(void* p)
{
	int offset = (int) p, i,
		start = offset * VECLEN, end = offset * VECLEN + VECLEN;



	double locSum = 0.0;
	for (i = start; i < end; i++)
		locSum += a[i];//no mutual exclusion needed (read-only)
	


}

int main(int argc, char* argv[])
{
    int m;
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
    int arr[m][m];

    for (i = 0; i < m; i++)
    {
        for(j = 0; j < m; j++){
            fscanf(finput, "%d ", &arr[i][j]); //import numbers.
        }
        fscanf(finput, "\n");
        
    }
    printf("\nMain array: \n");
    for(i=0;i<m;i++){
        printArray(arr[i],m);
    }
    
	pthread_t threads[NUMTHREADS];

	a = (int*) malloc(m*m * sizeof(int));


	for (i = 0; i < NUMTHREADS; i++)
		pthread_create(&threads[i], NULL, doDotProduct, (int) i);
	
	for (i = 0; i < NUMTHREADS; i++)
		pthread_join(threads[i], NULL);



	return 0;
}
