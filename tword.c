#include <stdio.h>
#include <ctype.h>
#include <pthread.h>

#include "tree.h"

#define MIN_ARGS 3
#define MAX_FILES 8
#define MAX_SIZE 64

char files[255][MAX_FILES];

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

        pthread_t *threads = malloc(sizeof(pthread_t) * file_num);

        for (int i = 0; i < file_num; i++)
        {
            strcpy(files[i], argv[MIN_ARGS + i]); // storing the file names
        }

        for (int i = 0; i < file_num; i++)
        {
            char *file_name;
            strcpy(file_name, files[i]);

            threads[i] = i;
            pthread_create(&threads[i], NULL, file_processing, (void *) file_name);
        }

        for (int i = 0; i != listLength; i++) 
        {
            pthread_join(threads[i], NULL);
        }


        // TODO:Writing to the output file


        free(threads);
    }
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
