#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#define MIN_ARGS 4
#define MAX_FILES 8
#define MQNAME "/MQNAME"

char files[MAX_FILES][255];

// char *resize(char *str);

// char *trim(char *str);

int main(int argc, char const *argv[])
{

    int file_num;
    int message_size;
    mqd_t mq;
    struct mq_attr mq_attr;

    if(mq_unlink(MQNAME) == 0)
        fprintf(stdout, "Message queue %s removed from system.\n", MQNAME);

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

            mq = mq_open(MQNAME, O_RDWR | O_CREAT, 0666, NULL);
            mq_getattr(mq, &mq_attr);
            int bufflen = mq_attr.mq_msgsize;
            char *bufferp;

            if (mq == -1) 
            {
                perror("Parent) Message Queue cannot be opened!\n");
                exit(1);
            }

            for(int i = 0; i < file_num; i++)
            {  
                pid_t n = fork();
                if(n == 0) // child process execution
                {
                    mq = mq_open(MQNAME, O_RDWR);

                    bufferp = (char *) malloc(bufflen);

                    char str[message_size];

                    strcpy(str, "");
                    
                    FILE *fp = fopen(files[i], "r");

                    char file_str[64] = "";

                    while ( fscanf(fp, "%s", file_str) != EOF )
                    {
                        if (strlen(str) + strlen(file_str) + 1 > message_size)
                            break;
                        strcat(str, file_str);
                        strcat(str, " ");
                    }

                    mq_send(mq, str, message_size, 0);
                    mq_close(mq);
                    exit(0);
                }

            }

            int bufferlen = mq_attr.mq_msgsize;
            bufferp = (char *) malloc(bufferlen);

            mq_receive(mq, bufferp, bufferlen, NULL);

            printf("%s\n", bufferp);
            printf("%ld\n", sizeof(*bufferp));
            free(bufferp);

        }
    }

    mq_close(mq);

    return 0;
}
    
   


// char *resize(char *str)
// {
//     int length = strlen(str);
//     while(length % 4 != 3)
//     {
//         strcat(str, " ");
//         length++;
//     }

//     return str;
// }

// char *trim(char *str)
// {
//     int length = 0;
//     for(int i = 0; str[i] != ' '; i++)
//     {
//         length++;
//     }

//     char* new_str = "";
//     strncpy(str, new_str, length);
//     return new_str;
// }