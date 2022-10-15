#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "file_process.h"
#include "shareddefs.h"

#define MIN_ARGS 4
#define MAX_FILES 8

char files[255][MAX_FILES];

int main(int argc, char const *argv[])
{
    int file_num;
    int message_size;
    mq_t mq;

    if (argc < MIN_ARGS || argc > MIN_ARGS + MAX_FILES)
    {
        fprintf(stderr, "Argument number is inappropriate!\n");
        exit(-1);
    }
    else
    {
        file_num = atoi(argv[3]);
        message_size = atoi(argv[1]);

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
            
            mq = mq_open(MQNAME, O_CREAT | O_RDONLY);
            if (mq == -1) 
            {
                perror("Message Queue cannot be opened!\n");
                exit(1);
            }

            for(int i = 0; i < file_num; i++)
            {  
                pid_t n = fork();
                if(n == 0) // child process execution
                {
                    struct Node *root = NULL;

                    mq = mq_open(MQNAME, O_WRONLY);
                    if (mq == -1) 
                    {
                        perror("Message Queue cannot be opened!\n");
                        exit(1);
                    }

                    root = word_frequency(files[i]);


                    free_node(root);

                    exit(0);
                }
            }
            // PARENT

        }
    }
    

    return 0;
}
