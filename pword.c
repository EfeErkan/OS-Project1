#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "file_process.h"

#define MIN_ARGS 4
#define MAX_FILES 8

char files[MAX_FILES][255];

int main(int argc, char const *argv[])
{
    int file_num;
    int message_size;
    int message_count = 0;
    mqd_t mq;
    struct mq_attr attr;

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
            attr.mq_msgsize = message_size;
            mq_setattr(mq, &attr, NULL);

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

                    int arr_size = countNodes(root);

                    struct item *word_count_arr = malloc(sizeof(struct item) * arr_size);
                    int index = 0;
                    getInorder(root, word_count_arr, &index);
                    int current_msgsize = 0;
                    int start = 0;

                    for (int i = 0; i < arr_size; i++)
                    {
                        if (sizeof(word_count_arr[i]) + current_msgsize < message_size)
                        {
                            current_msgsize += sizeof(word_count_arr[i]);
                            continue;
                        }

                        struct item_buffer buff;
                        buff.item_array = malloc(sizeof(struct item) * (i - start));
                        buff.arr_size = i - start;

                        for (int j = 0; j < i - start; j++)
                        {
                            buff.item_array[j] = word_count_arr[j + start];
                        }

                        start = i;
                        current_msgsize = 0;
                        message_count++;
                        
                        // SEND MESSAGE TO QUEUE
                        mq_send(mq, &buff, sizeof(buff), 0);
                    }

                    free_node(root);
                    free(word_count_arr);

                    exit(0);
                }
            }

            // PARENT
            struct Node* root;
            int numRead = 0;
            for( int i = 0; i < message_count; i++)
            {   
                struct item_buffer *buff;
                numRead = mq_receive(mq, &buff, message_size, 0);

                for(int j = 0; j < buff->arr_size; j++)
                {
                    insert(&root, buff->item_array[j].string);
                }

            }

            printInorder(root);
        }
    }
    

    return 0;
}
