#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>



struct var{
    char name[3];
    short adr;
};

struct data
{
    bool is_used;
    int size;
};

struct var *vars;
const int S = 1024;
int *memory;


struct data decode(int code){
    struct data d;
    d.is_used = (code>>8);

    d.size = code;
    d.size &= 0b011111111;

    return d;
}

// 0*24 - F - SSSSSSS
int encode(bool is_used, short size){
    int a = 0;
    a |= is_used;
    a <<= 8;
    a |= size;
    return a;
}

//size = n of ints
int* my_malloc(short size){
    int *temp = memory;
    size++;

    struct data d;
    d = decode(*temp);
    while(d.is_used || (size > d.size && d.size!=0)){
        temp += d.size;
        d = decode(*temp);
    }

    //printf("d is_used: %d, d_size %d, temp: %d \n", d.is_used, d.size, *temp);
    
    if (d.size && size-d.size!=0)
    {
        *(temp+size) = encode(0,d.size-size);
    }
    
    *temp = encode(1,size);
    
    return temp;
}

void my_free(int *ptr){
    *ptr =(*ptr &0b011111111);
}

int main() {
    memory = calloc(S, 4);
    vars = calloc(100,4);
    //short *empty = malloc(100);


    int *ptr1 = my_malloc(10);
    int *ptr2 = my_malloc(20);
    int *ptr3 = my_malloc(30);
    my_free(ptr2);
    int *ptr4 = my_malloc(5);
    int *ptr5 = my_malloc(16);
    ptr2 = my_malloc(40);
    
    int pos1 = ptr1-memory;
    int pos2 = ptr2-memory;
    int pos3 = ptr3-memory;
    int pos4 = ptr4-memory;
    int pos5 = ptr5-memory;

    printf("pos1: %d, pos2: %d, pos3: %d, pos4: %d, pos5: %d\n", pos1, pos2, pos3, pos4, pos5);
    return 0;
} 