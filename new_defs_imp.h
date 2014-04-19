#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "reader.h"
#include "cargs.h"








union data{char* c; double d;};
typedef union data Data;


struct rows{
    Data* data;
    struct rows* next;
    char* type;
    int fields;
    int pos;
};
typedef struct rows Row;


struct table{
    Row* head;
    Row* tail;
    Row** rows;
    int rowcount;
    char* masterkey;
    int tablebuilt;
    int cols;
};
typedef struct table Table;









Table *r_read(FILE *in);

