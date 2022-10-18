#include <stdio.h>
#include <ctype.h>
#include <pthread.h>

#include "tree.h"

#define MIN_ARGS 3
#define MAX_FILES 8
#define MAX_SIZE 64

char files[MAX_FILES][255];

struct Node *root = NULL;

char *toUpperCase(char *str);

void *file_processing(void *param);

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
        file_num = atoi(argv[2]);

        if (argc != MIN_ARGS + file_num)
        {
            fprintf(stderr, "File number and existing files are incompatible!\n");
            exit(-1);
        }

        pthread_t threads[file_num];

        for (int i = 0; i < file_num; i++)
        {
            strcpy(files[i], argv[MIN_ARGS + i]);
        }

        for (int i = 0; i < file_num; i++)
        {
            threads[i] = i;
            pthread_create(&threads[i], NULL, file_processing, (void *) files[i]);
        }

        for (int i = 0; i < file_num; i++) 
        {
            pthread_join(threads[i], NULL);
        }

        FILE *fp = fopen(argv[1], "w");

        writeInorder(root, fp);

    }

    free_node(root);
    
    return 0;
}

void *file_processing(void *param)
{
    char *file_name = (char *) param;

    FILE *fp = fopen(file_name, "r");

    char buffer[MAX_SIZE];

    while ( fscanf(fp, "%s", buffer) != EOF )
    {
        insert(&root, toUpperCase(buffer));
    }

    fclose(fp);

    pthread_exit(0);

}

char *toUpperCase(char *str)
{
    char *result = str;
    for (int i = 0; i < strlen(str); i++)
    {
        result[i] = toupper(result[i]);
    }
    return result;
}
