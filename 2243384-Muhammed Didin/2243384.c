/*
    Muhammed Didin - 2243384
    April 2021

    -For testing, your input file names should exactly be like input1.txt , input2.txt ...
    First, you need to enter how many input files you have.
    -Finally, your input file context should exactly be like:

    <8>
    3 4 5 4 2 1 1 6


    -8 shows how many numbers are in the file.
    You must enter the correct number of numbers in the file there.

    -In case of any problem, I can make an interview and explain how the program works.
*/

//Libraries
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>

//Function Declerations
void swap(int *, int *);
void selectionSort(int[], int);
void insertionSort(int[], int);
void printArray(int[], int);
char signal_content[100];
//Signal Handlers
void handler_sigusr1(int signum)
{
    printf("SIGUSR1 Handled\n");
}

void handler_sigusr2(int signum)
{
    printf("SIGUSR2 Handled\n");
}
int main()
{
    signal(SIGUSR1, handler_sigusr1);
    signal(SIGUSR2, handler_sigusr2);
    int nOfFiles, i, child_status, number, sleepy, z = 0, j = 1, randarr[10];
    int name = 1, selection = 0;
    FILE *finput;
    FILE *fintermediate;
    fflush(stdin);
    printf("Welcome!\n1)File operations\n");
    printf("2)Pipe operations\nType anything for exit\n");
    printf("Please enter your selection: ");
    scanf("%d", &selection);
    if (selection == 1) //For file operations(Part 1)
    {

        printf("Please enter number of files: ");
        scanf("%d", &nOfFiles);

        char fileName[100], intername[100];
        char inter[10];
        srand(time(0));
        pid_t pid[nOfFiles];
        //int pid;
        for (i = 1; i <= nOfFiles; i++)
        {
            if ((pid[i] = fork()) == 0) //children
            {
                printf("Child pid:%d\n", pid[i]);
                sprintf(fileName, "input%d.txt", i); //File name creations. File names are like: input1.txt,input2.txt...
                                                    

                printf("file:%s\n", fileName);

                //create an array of 10 with random numbers.
                for (j = 0; j < 10; j++)
                {
                    randarr[j] = rand() % 6 + 1; //numbers are between 1-7
                }

                int randnum = rand() % 10;
                sleepy = randarr[randnum]; // access random array's one random number.
                                            
                finput = fopen(fileName, "r"); //open file for reading.
                if (finput == NULL) // give error if an error occured while file opening.
                {
                    printf("Sorry.. An error occured while opening the input file");
                    return "0";
                }

                fscanf(finput, "<");
                fscanf(finput, "%d", &number);
                fscanf(finput, ">");
                fscanf(finput, "\n");
                int arr1[number];
                for (z = 0; z < number; z++)
                {
                    fscanf(finput, "%d", &arr1[z]); //import numbers to be sorted.
                }
                fclose(finput);

                //printArray(arr1, number);

                clock_t begin = time(NULL); //take sort beginnig time
                printf("beginning time: %f\n", (double)begin);
                if ((getpid() % 2) == 1) // if pid is odd, then use selection sort.
                { 
                    printf("We use selection sort\n");
                    selectionSort(arr1, number);
                }
                else //if pid is even, then use the insertion sort.
                { 
                    printf("We use insertion sort\n");
                    insertionSort(arr1, number);
                }
                sleep(sleepy);            //Have a rest for random seconds
                clock_t end = time(NULL); //take sort ending time
                printf("ending time: %f\n", (double)end);
                double time_spent = ((end - begin)); // calculating spent time for sorting.
                printf("time spent for %d th process's sorting: %f sn\n", getpid(), time_spent);

                sprintf(intername, "intermediatefile%d.txt", i); //intermediate file name creations
                                                                 //our intermediate file names are like: 
                                                                 //intermediatefile1.txt, intermediatefile2.txt ....
                printf("interfilename: %s", intername);
                fintermediate = fopen(intername, "w+"); //open intermediate file for writing.
                fprintf(fintermediate, "<");
                fprintf(fintermediate, "%d", number);
                fprintf(fintermediate, ">");
                fprintf(fintermediate, "\n");

                for (j = 0; j < number; j++)
                {
                    fprintf(fintermediate, "%d ", arr1[j]); // write the sorted array to file
                }
                fprintf(fintermediate, "\n");
                fprintf(fintermediate, "<");
                fprintf(fintermediate, "%f", time_spent);
                fprintf(fintermediate, ">");
                sleep(1);
                //Send signals. If pid is even send SIGUSR2
                if (getpid() % 2 == 0) 
                {
                    kill(getppid(), SIGUSR2);
                    time_t rawtime;
                    struct tm *timeinfo;

                    time(&rawtime);
                    timeinfo = localtime(&rawtime);
                    fprintf(fintermediate, "\nSIGUSR2 time: %s", asctime(timeinfo));
                }
                //Send signals. If pid is odd send SIGUSR1
                else
                {

                    kill(getppid(), SIGUSR1);
                    time_t rawtime;
                    struct tm *timeinfo;

                    time(&rawtime);
                    timeinfo = localtime(&rawtime);
                    fprintf(fintermediate, "\nSIGUSR1 time: %s", asctime(timeinfo));
                }
                fclose(fintermediate);

                exit(0);
            }
        }
        float outtime;
        int internumber, count = 1;
        FILE *interfile, *outfile;
        char interfilename[100],signalcontent[300];
        printf("Number of files before parent: %d\n", nOfFiles);
        for (i = 0; i < nOfFiles; i++)
        {
            pid_t pid = wait(&child_status);
            if (WIFEXITED(child_status)) //parent
            {
                printf("\nNOW PARENT!\n");
                printf("Parent %d terminated\n", pid);


                sprintf(interfilename, "intermediatefile%d.txt", count);//intermediate file name creations.

                printf("interfilename: %s\n", interfilename);
                interfile = fopen(interfilename, "r"); //open inter. file for reading purposes.
                fscanf(interfile, "<");
                fscanf(interfile, "%d", &internumber); //store number of numbers
                fscanf(interfile, ">");
                int intercontext[internumber];
                fscanf(interfile, "\n");
                printf("internumber: %d\n", internumber);
                for (j = 0; j < internumber; j++)
                {
                    fscanf(interfile, "%d", &intercontext[j]); // import sorted list 
                }

                fscanf(interfile, " \n");
                fscanf(interfile, "<");
                fscanf(interfile, "%f", &outtime);
                fscanf(interfile, ">");
                fscanf(interfile,"\n%s",signalcontent);
                printf("signalcontent: %s\n",signalcontent);
                printf("outtimes: %f\n", outtime);
                outfile = fopen("fileOutput.txt", "a+"); // final output file opening for writing purposes.
                fprintf(outfile, "<");

                fprintf(outfile, "%f", outtime);
                fprintf(outfile, ">");
                fprintf(outfile, "<");

                for (z = 0; z < internumber; z++)
                {
                    fprintf(outfile, "%d ", intercontext[z]);
                }
                fprintf(outfile, ">");
                fprintf(outfile," %s ",signalcontent);
                fprintf(outfile, "\n");

            }
            count++;
        }
    }
    else if (selection == 2) // For pipe operations (Part 2)
    {

        int i, nofiles, randarr[10], sleepy, j;
        FILE *file, *fout;
        char fileName[100];
        printf("Please enter number of files: ");
        scanf("%d", &nofiles);

        for (i = 0; i < nofiles; i++)
        {

            int fd[2], number = 5;

            if (pipe(fd) == -1) // opening pipe. if error occures give error.
            {
                printf("An error occured while opening pipe\n");
                return 1;
            }
            int id = fork();
            if (id == 0)
            { //child
                int number = 6;
                sprintf(fileName, "input%d.txt", i + 1);  // open input file.
                for (j = 0; j < 10; j++)
                {
                    randarr[j] = rand() % 6 + 1;
                }
                int randnum = rand() % 10;
                sleepy = randarr[randnum]; //random number
                file = fopen(fileName, "r");
                if (file == NULL)
                {
                    printf("Sorry.. An error occured while opening the input file");
                    return "0";
                }
                fscanf(file, "<");
                fscanf(file, "%d", &number);
                fscanf(file, ">");
                fscanf(file, "\n");
                int arr1[number];
                for (i = 0; i < number; i++)
                {
                    fscanf(file, "%d", &arr1[i]);
                }
                fclose(file);
                clock_t begin = time(NULL); //take sort beginnig time
                printf("beginning time: %f\n", (double)begin);
                if (getpid() % 2 == 1)
                {
                    selectionSort(arr1, number);
                }
                else
                {
                    insertionSort(arr1, number);
                }
                sleep(sleepy);            //Have a rest for random seconds
                clock_t end = time(NULL); //take sort ending time
                printf("ending time: %f\n", (double)end);
                double time_spent = ((double)(end - begin));
                printf("time spent for %d th process's sorting: %f sn\n", getpid(), time_spent);

                if (write(fd[1], &time_spent, sizeof(double)) < 0) //send spent time to parent.
                {                                                  //if error occures, give error
                    printf("Sending error1!\n");
                }
                if (write(fd[1], &number, sizeof(int)) < 0) //send number of number to parent.
                {
                    printf("Sending error2!\n");//if error occures, give error
                }
                if (write(fd[1], arr1, number * sizeof(int)) < 0)//send array to parent.
                {
                    printf("Sending error3!\n");//if error occures, give error
                }
                close(fd[1]);
            }

            else //parent
            {
                wait(NULL);
                close(fd[1]);
                int i, number, z;
                double time;
                if (read(fd[0], &time, sizeof(double)) < 0) // take time spent from parent.
                {
                    printf("Error while taking time\n"); //if error occures, give error
                }
                if (read(fd[0], &number, sizeof(int)) < 0) //take number of elements from parent.
                {
                    printf("Error while taking the number of elements\n"); //if error occures, give error
                }
                int y[number];
                if (read(fd[0], y, number * sizeof(int)) < 0) // take array from parent.
                {
                    printf("Error while taking the array\n"); //if error occures, give error
                }
                close(fd[0]);
                printf("Time: %f\n", time);
                printArray(y, number);

                fout = fopen("pipeoutput.txt", "a+");  //open final output file for writing

                fprintf(fout, "<");

                fprintf(fout, "%f", time); // print time to file
                fprintf(fout, ">");
                fprintf(fout, "<");

                for (z = 0; z < number; z++) //print array to file.
                {
                    fprintf(fout, "%d ", y[z]);
                }
                fprintf(fout, ">");
              if (getppid() % 2 == 0)
                {
                    time_t rawtime;
                    struct tm *timeinfo;

                    
                    timeinfo = localtime(&rawtime);
                    fprintf(fout, " SIGUSR1 time: %s", asctime(timeinfo));
                }
                else
                {
                    time_t rawtime;
                    struct tm *timeinfo;

                    
                    timeinfo = localtime(&rawtime);
                    fprintf(fout, " SIGUSR2 time: %s", asctime(timeinfo));
                }
            } 
            fclose(fout);
        }
    }
    else
    {
        printf("Bye bye!");
    }
}
//swap and sorting algorithms

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void selectionSort(int arr[], int n)
{
    int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n - 1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;

        // Swap the found minimum element with the first element
        swap(&arr[min_idx], &arr[i]);
    }
}
void insertionSort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;

        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}
void printArray(int arr[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}