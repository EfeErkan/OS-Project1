#ifndef SHAREDDEFS
#define SHAREDDEFS

#define MQNAME "/MQname"

struct item
{
    int num;
    char *string;
};

struct item_buffer
{
    char** str_arr;
    int* count_arr;
    int arr_size;
};

#endif