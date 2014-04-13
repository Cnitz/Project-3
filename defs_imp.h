#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "reader.h"
#include "cargs.h"







struct List{
    union foo
    { double e;
        char* c;
    }info;
    char val;
    struct list* next;
};
typedef struct List L;



struct rows{
    L ctb[100];
    struct rows* next;
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

