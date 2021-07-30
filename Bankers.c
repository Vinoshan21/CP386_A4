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
pthread_t *threads;


// Declare Methods
void readFile(char *fileName);

int main(int argc, char *argv[])
{
    if (argc != 5){
        printf("Not enough arguments supplied");
        return -1;
    }

    // Read from file and initialize thread variables
    readFile("sample4_in.txt");

    char *line = "";

    // While loop to keep asking commands
    while (line != "quit"){
        printf ("Enter A Command Or Enter 'quit' To Quit Program");
        fgets(line);

        // If user Inputted empty String
        if (line == ""){
            printf("Invalid Command");
        }
        else{

            // Split the line into different arguments
            char *token = strtok(line, " ");

            // Break into if cases for seperate commands

            // Request Resources
            if (token[0] == "RQ"){

            }

            // Release Resources
            elif (token[0] == "RL"){

            }

            // Output Values of data structure
            elif (token[0] == "*"){

            }

            // Find safe sequence and run thread
            elif (token[0] == "Run"){

            }
            else{
                printf("Invalid Command")
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
