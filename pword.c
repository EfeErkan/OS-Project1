#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include "file_process.h"

#define MIN_ARGS 4
#define MAX_FILES 8

char files[MAX_FILES][255];

char *resize(char *str);

char *trim(char *str);

int main(int argc, char const *argv[])
{
    printf("Start");

    int file_num;
    int message_size;
    mqd_t mq;
    struct mq_attr attr;

    if(mq_unlink(MQNAME) == 0)
        fprintf(stdout, "Message queue %s removed from system.\n", MQNAME);

    printf("Continue");

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

            // //attr.mq_flags   = 0;
            // attr.mq_maxmsg  = 1024;
            // attr.mq_msgsize = message_size;

            for(int i = 0; i < file_num; i++)
            {  
                pid_t n = fork();
                if(n == 0) // child process execution
                {
                    struct Node *root = NULL;

                    mq = mq_open(MQNAME, O_CREAT | O_WRONLY, 0666, NULL);
                    if (mq == -1) 
                    {
                        perror("Child) Message Queue cannot be opened!\n");
                        exit(1);
                    }

                    mq_getattr(mq, &attr);

                    root = word_frequency(files[i]);

                    int arr_size = countNodes(root);

                    struct item *word_count_arr = malloc(sizeof(struct item) * arr_size);
                    int index = 0;
                    getInorder(root, word_count_arr, &index);
                    int current_msgsize = sizeof(int);
                    int start = 0;

                    for (int i = 0; i < arr_size; i++)
                    {
                        if (sizeof(word_count_arr[i]) + current_msgsize < message_size)
                        {
                            current_msgsize += sizeof(word_count_arr[i]);
                            continue;
                        }

                        struct item_buffer buff;

                        char *strings[i - start];

                        //buff.str_array = malloc(sizeof(struct item) * (i - start));

                        buff.str_arr = strings;

                        buff.count_arr = malloc(sizeof(int) * (i - start));
                        buff.arr_size = i - start;

                        for (int j = start; j < i - start; j++)
                        {
                            //buff.item_array[j] = word_count_arr[j + start];
                            strcpy(buff.str_arr[j], resize(word_count_arr[j + start].string));
                            buff.count_arr[j] = word_count_arr[j + start].num;
                        }

                        start = i;
                        current_msgsize = sizeof(int);
                        printf("MESSAGE Send!!!");
                        // SEND MESSAGE TO QUEUE
                        mq_send(mq, (char *)&buff, sizeof(buff), 0);
                    }

                    free_node(root);
                    free(word_count_arr);

                    mq_close(mq);

                    exit(0);
                }
            }

            // PARENT
            mq = mq_open(MQNAME, O_CREAT | O_RDONLY, 0666, NULL);

            if (mq == -1) 
            {
                perror("Parent) Message Queue cannot be opened!\n");
                exit(1);
            }
            
            struct Node* root = NULL;

            struct item_buffer *buff;
            char* tmp = (char *) malloc(attr.mq_msgsize);
            printf("MESSAGE Receive!!!");
            int numRead = mq_receive(mq, (char *)tmp, attr.mq_msgsize, NULL);
            if(numRead == -1)
            {
                perror("Message Queue cannot be received!\n");
                exit(1);
            }

            buff = (struct item_buffer*) tmp;

            for(int j = 0; j < buff->arr_size; j++)
            {
                insertWithCount(&root, buff->str_arr[j], buff->count_arr[j]);
            }

            FILE *fp = fopen(argv[2], "w");
            writeInorder(root, fp);

            free_node(root);
            free(buff);
            free(tmp);
        }
    }
    
    mq_close(mq);

    return 0;
}

char *resize(char *str)
{
    int length = strlen(str);
    while(length % 4 != 3)
    {
        strcat(str, " ");
        length++;
    }

    return str;
}

char *trim(char *str)
{
    int length = 0;
    for(int i = 0; str[i] != ' '; i++)
    {
        length++;
    }

    char* new_str = "";
    strncpy(str, new_str, length);
    return new_str;
}