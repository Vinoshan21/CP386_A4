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
int available[];
int maximum[][];
int allocated[][];
int need[][];

// Variables
pthread_t *threads;


// Declare Methods
void readFile(char *fileName, )

int main(int argc, char *argv[]) {
    if (argc != 6){
        printf("Not enough arguments supplied")
        return -1;
    }

    // Read from file and initialize thread variables
    readFile(argv[1], &threads)

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


// Read File method
// Used to intialize the THREADS, MAXIMUM methods
void readFile(char *fileName, Thread **threads){

    FILE *f = fopen(fileName, "r");
	if (!f) {
		printf(
				"File can not be opened. Exiting program\n");
		return NULL;
	}


}