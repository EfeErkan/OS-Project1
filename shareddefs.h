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
    struct item *item_array;
    int arr_size;
}

#endif