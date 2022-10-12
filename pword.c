#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "file_process.h"

#define MIN_ARGS 4

int main(int argc, char const *argv[])
{
    int file_num;

    if (argc < MIN_ARGS)
    {
        fprintf(stderr, "Argument number is insufficient!");
        exit(-1);
    }
    else
    {
        file_num = atoi(argv[3]);

        if (argc != MIN_ARGS + file_num)
        {
            fprintf(stderr, "File number and existing files are incompatible!");
            exit(-1);
        }
        else
        {
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