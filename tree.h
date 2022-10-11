#ifndef TREE
#define TREE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DECLARATION
struct Node 
{
    char *str;
    int count;
    struct Node *left;
    struct Node *right;
};

void insert(struct Node **root, char *str);

void free_node(struct Node *root);

void printInorder(struct Node *node);


//IMPLEMENTATION
void insert(struct Node **root, char *str)
{
    if ( *root == NULL ) 
    {
        *root = (struct Node*) malloc(sizeof(struct Node));
        (*root)->str = (char*) malloc(strlen(str) + 1);

        strcpy((*root)->str, str);
        (*root)->left = NULL;
        (*root)->right = NULL;
        (*root)->count = 1;
    } 
    else if ( strcmp(str, (*root)->str) < 0 )
    {
        insert( &(*root)->left, str);
    }
    else if ( strcmp(str, (*root)->str) > 0 )
    {
        insert( &(*root)->right, str);
    }
    else
    {
        (*root)->count++;
    }
}


void printInorder(struct Node *node)
{
    if (node != NULL)
    {
        printInorder(node->left);
        printf("%s %d\n", node->str, node->count);
        printInorder(node->right);
    }
}

void free_node(struct Node *root)
{
    if (root != NULL)
    {
        free_node(root->left);
        free_node(root->right);
        free(root->str);
        free(root);
        root = NULL;
    }
}
#endif