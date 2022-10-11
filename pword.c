#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "file_process.h"

int main()
{
    // for(int i = 0; i < 5; i++)
    // {  
    //     pid_t c = fork();
    //     if(c == 0) 
    //     { 
    //         printf("[son] pid %d from [parent] pid %d i = %d\n",getpid(),getppid(), i);
    //         exit(0);
    //     } 
    // }

    // for(int i = 0; i < 5; i++)
    //     wait(NULL);

    word_frequency("example.txt");

}