#ifndef FILE_PROCESS
#define FILE_PROCESS

#include <stdio.h>
#include <ctype.h>
#include "tree.h"

#define MAX_SIZE 64

//DECLARATION
char *toUpperCase(char *str);

struct Node *word_frequency(char *file_name);


//IMPLEMENTATION
char *toUpperCase(char *str)
{
    char *result = str;
    for (int i = 0; i < strlen(str); i++)
    {
        result[i] = toupper(result[i]);
    }
    return result;
}

struct Node *word_frequency(char *file_name)
{
    FILE *fp = fopen(file_name, "r");

    struct Node *root = NULL;

    char buffer[MAX_SIZE];

    while ( fscanf(fp, "%s", buffer) != EOF )
    {
        insert(&root, toUpperCase(buffer));
    }

    fclose(fp);
    
    return root;

}
#endif