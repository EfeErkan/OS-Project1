#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "file_process.h"

#define MIN_ARGS 4
#define MAX_FILES 8

char files[255][MAX_FILES];

int main(int argc, char const *argv[])
{
    int file_num;

    if (argc < MIN_ARGS || argc > MIN_ARGS + MAX_FILES)
    {
        fprintf(stderr, "Argument number is inappropriate!\n");
        exit(-1);
    }
    else
    {
        file_num = atoi(argv[3]);

        if (argc != MIN_ARGS + file_num)
        {
            fprintf(stderr, "File number and existing files are incompatible!\n");
            exit(-1);
        }
        else
        {
            for (int i = 0; i < file_num; i++)
            {
                strcpy(files[i], argv[MIN_ARGS + i]); // storing the file names
            }

            for(int i = 0; i < file_num; i++)
            {  
                pid_t n = fork();
                if(n == 0) // child process execution
                { 
                    // call word_frequency for each child process
                } 
            }

            for(int i = 0; i < file_num; i++)
                wait(NULL);

        }
    }
    

    return 0;
}