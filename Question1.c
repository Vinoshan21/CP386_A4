#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>
#include <semaphore.h>

// Data structures
int available[4];
int maximum[5][4];
int allocated[5][4];
int need[5][4];
char arr[5][8];

// Variables
pthread_t threads[5];
int safetyOrder[5];


// Declare Methods
void readFile(char *fileName);
bool safety_algorithm();
void *threadRun(void *v);

// Sem Locks
sem_t lock;

int main(int argc, char *argv[])
{
    if (argc != 5){
        printf("Not enough arguments supplied");
        return -1;
    }

    available[0] = atoi(argv[1]);
    available[1] = atoi(argv[2]);
    available[2] = atoi(argv[3]);
    available[3] = atoi(argv[4]);


    // Read from file and initialize thread variables
    readFile("sample4_in.txt");


    char line[100];

    // While loop to keep asking commands
    while ((strcmp(line,"Exit") != 0)){
        printf ("Enter A Command Or Enter 'Exit' To Quit Program\n");
        //scanf(" %[^\n]", line);
        fgets(line, 100, stdin);
        // line = "Run";

        // If user Inputted empty String
        if ((strcmp(line,"") == 0)){
            printf("Invalid Command");
        }
        else{

            int str = 0;

            for (int x = 0; line[x] != '\0'; x++){
                if (line[x] == ' '){
                    str++;
                }
            }

            // Split the line into different arguments
            char *token = strtok(line, " \n");
            int instr[sizeof(available)/sizeof(available[0])];

            int y = 0;
            if (str >= 2){
                for (int i = 2; i < sizeof(available)/sizeof(available[0]) + 1; i++){
                    instr[y] = atoi(&token[i]);
                     y++;
                }

            }
            int str_length = y;
            y= 0;

            char *first = &token[0];

            // Break into if cases for seperate commands

            // Request Resources
            if (strcmp(first,"RQ") == 0){

                for(int n = 2; n < str_length; n++){
                    allocated[instr[1]][n-2] = instr[n];
                }

                if (safety_algorithm()){
                    printf("State is safe and request is satisfied\n");
                    for (int i = 0; i < 5; i++){
                        for (int j = 0; j < 4; j++){
                            need[i][j] = maximum[i][j] - allocated[i][j];
                        }
                    }
                }

                else{
                    printf("State is not safe and request is denied\n");
                    for(int n = 2; n < str_length; n++){
                        allocated[instr[1]][n-2] = 0;
                    }
                }


            }

            // Release Resources
            else if (strcmp(first,"RL") == 0){

                int err;

                if (instr[1] >= 5){
                    printf("Index cannot be larget than maximum number of customers\n");
                }
                else{
                    for (int n = 2; n < str_length; n++){
                        int val = allocated[instr[1]][n-2] - instr[n];

                        if (val < 0){
                            printf("Release denied\n");
                            err = 1;
                            break;
                        }
                        else{
                            allocated[instr[1]][n-2] = val;
                        }
                        if (n == (str_length - 1)){
                            printf("The resources have been released successfully.\n");
                            for (int i = 0; i < 5; i++){
                                for (int j = 0; j < 4; j++){
                                    need[i][j] = maximum[i][j] - allocated[i][j];
                                }
                            }
                        }
                        if (err == 1){
                            continue;
                        }
                    }
                }
            }

            // Output Values of data structure
            else if (strcmp(&token[0],"Status") == 0){

                printf("Available Resources:\n");

                int temp;
                int temp2;

                for (int i = 0; i < 4; i++){
                    temp = 0;
                    for (int j = 0; j < 5; j++){
                        temp = temp + allocated[i][j];
                    }
                    temp2 = available[i] - temp;
                    available[i] = temp2;
                }

                for (int i = 0; i < 4; i++){
                    printf("%d ", available[i]);
                }
                printf("\n");
                printf("Maximum Resources:\n");

                for (int i = 0; i < 5; i++){
                    for (int j = 0; j < 4; j++){
                        printf("%d ", maximum[i][j]);
                    }
                    printf("\n");
                }
                printf("\n");

                printf("Allocated Resources:\n");

                for (int i = 0; i < 5; i++){
                    for (int j = 0; j < 4; j++){
                        printf("%d ", allocated[i][j]);
                    }
                    printf("\n");
                }
                printf("\n");

                printf("Needed Resources: \n");

                for (int i = 0; i < 5; i++){
                    for (int j = 0; j < 4; j++){
                        printf("%d ", need[i][j]);
                    }
                    printf("\n");
                }

            }

            // Find safe sequence and run thread
            else if (strcmp(first,"Run") == 0){

                bool safe = safety_algorithm();
                if (safe){

                    // Printing the safety sequence
                    printf("Safe Sequence is :");
                    for(int i = 0; i < sizeof(safetyOrder)/sizeof(safetyOrder[0]); i++){
                        printf(" %d", safetyOrder[i]);
                    }

                    // Intialize semaphore
                    sem_init(&lock, 0, 1);

                    // Creating the threads in the right order
                    for(int i = 0; i < sizeof(safetyOrder)/sizeof(safetyOrder[0]); i++){
                        pthread_create(&threads[i], NULL, threadRun,&safetyOrder[i]);
                    }

                    sem_destroy(&lock);

                    printf("Safe Sequence is :");
                    for(int i = 0; i < sizeof(safetyOrder)/sizeof(safetyOrder[0]); i++){
                        printf(" %d", safetyOrder[i]);
                    }

                }
                else{
                    printf("Program is not in a safe state");
                }

            }

            else if (strcmp(first,"Exit") == 0){

                exit(0);

            }
            else{
                printf("Invalid Command\n");
            }

        }



    }

}


void readFile(char* fileName)//use this method in a suitable way to read file
{
	FILE *in = fopen(fileName, "r");
	if(!in)
	{
		printf("File can not be opened. Exiting program\n");
	}

	struct stat st;
	fstat(fileno(in), &st);
	char* fileContent = (char*)malloc(((int)st.st_size+1)* sizeof(char));
	fileContent[0]='\0';	
	while(!feof(in))
	{
		char line[100];
		if(fgets(line,100,in)!=NULL)
		{
			strncat(fileContent,line,strlen(line));
		}
	}
	fclose(in);

	char* command = NULL;
	int threadCount = 0;
	char* fileCopy = (char*)malloc((strlen(fileContent)+1)*sizeof(char));
	strcpy(fileCopy,fileContent);
	command = strtok(fileCopy,"\r\n");
	while(command!=NULL)
	{
		threadCount++;
		command = strtok(NULL,"\r\n");
	}

	char* lines[threadCount];
	command = NULL;
	int i=0;
	command = strtok(fileContent,"\r\n");
	while(command!=NULL)
	{
		lines[i] = malloc(sizeof(command)*sizeof(char));
		strcpy(lines[i],command);
		i++;
		command = strtok(NULL,"\r\n");
	}

    char arr2[5][8];

	for(int k=0; k<threadCount; k++)
	{
		char* token = NULL;
		int j=0;
		token =  strtok(lines[k],";");
		while(token!=NULL)
		{
			strcpy(arr[k], token);
			strcpy(arr2[k], token);
            allocated[k][j] = 0;
            j++;
            token = strtok(NULL, " ");

		}
	}
    char * a;
    for (int i = 0; i < 5; i++)
    {
        a = strtok(arr2[i], ",");
        int j = 0;
        while (a != NULL)
        {
            maximum[i][j] = atoi(a);
            need[i][j] = atoi(a);
            a = strtok(NULL, ",");
            j++;
        }
    }
}

bool safety_algorithm(){
    int work[sizeof(available)/sizeof(available[0])];
    bool finish[sizeof(threads)/sizeof(threads[0])];

    bool added = false;
    int customersAdded = 0;

    // Copys available into work
    for (int k = 0; k < sizeof(available)/sizeof(available[0]); k++){
        work[k] = available[k];
    }

    for (int i = 0; i < sizeof(finish)/sizeof(finish[0]); i++){
        finish[i] = false;
    }

    int i = 0;
    // Keeps looping to find safe sequence
    while (true){

        // If the customer has not already been run
        if (finish[i] == false){

            int tempWork[sizeof(work)/sizeof(work[0])];
            // Copys work into tempWork
            for (int k = 0; k < sizeof(available)/sizeof(available[0]); k++){
                tempWork[k] = work[k];
            }


            int rCounter = 0;

            // Loop through each resource
            for (int j = 0; j < sizeof(available)/sizeof(available[0]); j++){

                // Checking if the needed resource is less or equal to available
                if (need[i][j] <= tempWork[j]){
                    tempWork[j] = tempWork[j] + allocated[i][j];
                    rCounter++;
                }
                // If the needed amount is greater than available
                else{
                    break;
                }
            }

            // If all needs for process i have been met
            if (rCounter == sizeof(available)/sizeof(available[0])){
                finish[i] = true;
                // Copys tempWork into work
                for (int k = 0; k < sizeof(available)/sizeof(available[0]); k++){
                    work[k] = tempWork[k];
                }
                safetyOrder[customersAdded] = i;
                customersAdded++;
                added = true;
            }

            int finishCounter = 0;
            // Check if all processes have been accounted for
            for (int j = 0; j < sizeof(threads)/sizeof(threads[0]); j++){
                if (finish[j]){
                    finishCounter++;
                }
            }

            // If all processes can be run
            if (finishCounter == sizeof(threads)/sizeof(threads[0])){
                return true;
            }
        }

        i++;

        // It has iterated through all the customers, reset required
        if (i == sizeof(threads)/sizeof(threads[0])){
            i = 0;

            // It has went through an iteration without finding a process it can run therefore
            // the system is not in a safe state
            if(added == false){
                return false;
            }
            else{
                added = false;
            }
        }

    }
}

void *threadRun(void *v){
    
    // lock semaphore
    sem_wait(&lock);

    // Get which customer to run
    int num = *((int *) v);

    printf("Customer/Thread %d\n", num);

    // Printing allocated resources
    printf("Allocated resources: ");
    for(int i = 0; i < sizeof(allocated)/sizeof(allocated[0]); i++){
        printf(" %d", allocated[num][i]);
    }
    printf("\n");

    // Printing need
    for(int i = 0; i < sizeof(need)/sizeof(need[0]); i++){
        printf(" %d", need[num][i]);
    }
    printf("\n");

    // Printing available
    for(int i = 0; i < sizeof(available)/sizeof(available[0]); i++){
        printf(" %d", available[i]);
    }
    printf("\n");

    printf("Thread has started\n");
    printf("Thread has finished\n");
    printf("Thread is releasing resources\n");

    // Release method
    for (int i = 0; i < sizeof(available)/sizeof(available[0]); i++){
        available[i] += allocated[num][i];
        allocated[num][i] = 0;
        need[num][i] = maximum[num][i];
    }

    // New available
    for(int i = 0; i < sizeof(available)/sizeof(available[0]);i++){
        printf(" %d", available[i]);
    }
    printf("\n");

    sem_post(&lock);

    pthread_exit(0);
}