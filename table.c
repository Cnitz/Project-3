//
//  table.c
//  
//
//  Created by Christian Nitz on 2/17/14.
//
//

#include "defs_imp.h"



char* type_maker();
int find_max_fields();
void counter();
char* p;
char* t;
char* tbl_type();
void tbl_print();
char* type_maker();
Row* tbl_row_at();

int tablebuilt = 0, tableset = 0, count = 0, tmcount = 0;

/* If any of the rows or tables that are parameters to these functions are NULL,
 * it's undefined behavior. */

/*
 * Return a newly allocated table with all
 * members set to NULL/zero
 */
Table * tbl_make(){
    Table * ctb = (Table *)calloc(1, sizeof(Table));
    ctb->head = NULL;
    ctb->tail = NULL;
    ctb->rows = NULL;
    ctb->rowcount = 0;
    ctb->tablebuilt = 0;
    return ctb;
}

/*
 * Prepare the table for the addition of a row with num_field values.
 * This will add the row to the tail of the list.
 * Undefined behavior if tbl_done_building() has already been called.
 */
    void    tbl_start_row(Table* tbl, int num_fields){
        if(tbl->tablebuilt == 1 || tbl == NULL) return;
        tbl->rowcount++;

        Row* r = (Row*)calloc(1, sizeof(Row));
        r->fields = num_fields;
        Row* curr = tbl->head;
        
        if(curr == NULL){
            tbl->head = r;
        }
        while(curr != NULL){
            if(curr->next == NULL){
            curr->next = r;
                break;
            }
            curr=curr->next;

        }
        tbl->tail = r;
        
    }


/*
 * Add a NULL-terminated string as a field of the current row. Undefined
 * behavior if tbl_done_building() has been called or if the row's fields are already
 * full.
 */
void    tbl_add_string_to_row(Table* tbl, char * str) {
    if(tbl->tablebuilt == 1) return;
    if(tbl->tail->pos > tbl->tail->fields) return;
    
    L k;
    k.info.c = str;
    k.val = 'S';
    k.next = NULL;
   
    Row* curr = tbl->tail;
    curr->ctb[curr->pos] = k;
    curr->pos++;
   
}

/*
 * Add a double as a field of the current row. Undefined behavior if
 * tbl_done_building() has been called or if the row's fields are already full.
 */
void    tbl_add_double_to_row(Table* tbl, double d) {
    if(tbl->tablebuilt == 1) return;
    if(tbl->tail->pos > tbl->tail->fields) return;
    
    L k;
    k.info.e = d;
    k.val = 'D';
    k.next = NULL;
    
    Row* curr = tbl->tail;
    curr->ctb[curr->pos] = k;
    curr->pos++;
    
    

    
}

/*
 * Ends construction of the table:
 * Finds table type,
 * Removes all rows differing from table type
 * A completed array for Table is finished
 */
Table  *tbl_done_building(Table* tbl){
    tbl->tablebuilt = 1;
    char* key;
    //tbl_print(tbl);
    Table* temp = (Table*)calloc(1, sizeof(Table));
     Row* curr = tbl->head;
    
    if(tbl->rowcount < 2){
        temp->head = tbl->head;
        temp->masterkey = type_maker(curr);
        
       
        temp->cols = strlen(type_maker(curr));
        temp->rows = calloc(1, sizeof(Row*));
        temp->rows[0] = curr;
        tbl->rows = temp->rows;
        tbl->head = temp->head;
        tbl->masterkey = temp->masterkey;
        tbl->cols = temp->cols;
        return tbl;
    }
    
    int rows = find_max_fields(tbl);
    char* now = calloc(rows+1, sizeof(char));
    char* prev = calloc(rows+1, sizeof(char));
    char* master = calloc(rows+1, sizeof(char));
    Row* prevs;
   
    
    now = type_maker(curr);
   
    
    while(curr != NULL){
        if(curr->next == NULL) break;
        prev = now;
         now = type_maker(curr->next);
        if(strcmp(now, prev) == 0){
            master = now;
            
            tbl->masterkey = calloc(strlen(now)+1, sizeof(char));
            strcpy(tbl->masterkey, master);
            break;
        }
        curr=curr->next;
    }
    
    if(master == NULL) return NULL;
    else{
        curr = tbl->head;
       
        while(curr != NULL){
            now = type_maker(curr);
            if(strcmp(master, now)==0){
                
                if(temp->rowcount == 0){
                    temp->head = curr;
                    temp->rowcount++;
                }
                else if(temp->rowcount == 1){
                    temp->head->next = curr;
                    temp->tail = curr;
                    temp->rowcount++;
                }
                else {
                    temp->tail->next = curr;
                    temp->tail = curr;
                    temp->rowcount++;
                }
                    
                
            }
            
            if(curr->next == NULL)break;
            
            
            curr = curr->next;
        }
        
    }
    curr = temp->head;
    int i = 0;
    temp->rows = calloc(temp->rowcount, sizeof(Row*));
    while(curr != NULL){
        temp->rows[i] = curr;
        i++;
        if(curr->next == NULL) break;
        curr = curr->next;
    }
    tbl->head = temp->head;
    tbl->tail = temp->tail;
    tbl->rowcount = temp->rowcount;
    tbl->rows = temp->rows;
    return tbl;
        
}

/*
 * Return the number of columns/fields in the Table.
 */
int tbl_column_count( Table * tbl ){
    if(tbl->tablebuilt == 0) {
        return 0;
    }
    return strlen(tbl->masterkey);
}

/*
 * Return the Row at index at either S or D
 * Undefined behavior if at is out of bounds
 * Undefined behavior if called before tbl_done_building();
 */
Row *   tbl_row_at( Table * tbl, int at ){
    if(tbl->tablebuilt == 0) return NULL;

    return tbl->rows[at];
    
}

/*
 * Return the string value from the fields/columns of row in the at-th position
 * Undefined behavior if at is out of bounds or at-th field is not a char *.
 */
char *  tbl_string_at( Row * row, int at){
    if(row->fields <= at) return NULL;
    if(row->ctb[at].val == 'S')
    return row->ctb[at].info.c;
    return NULL;
}

/*
 * Return the double value from the fields/columns of row in the at-th position
 * Undefined behavior if at is out of bounds or at-th field is not a double.
 */
double  tbl_double_at( Row * row, int at){
     if(row->fields <= at) return -1;
    if(row->ctb[at].val == 'D')
return row->ctb[at].info.e;
    return -1;
}

/*
 * Print each field of r separated by commas and ending with a newline
 */
void    tbl_print_row( Row * row ){
    for(int i = 0; i < row->fields; i++){
        
        if(i == row->fields-1){
            if(row->ctb[i].val == 'S')
                printf("\"%s\"\n", row->ctb[i].info.c);
            if(row->ctb[i].val == 'D')
                printf("%.1f\n", row->ctb[i].info.e);
            break;
        }
        if(row->ctb[i].val == 'S')
            printf("\"%s\",", row->ctb[i].info.c);
        if(row->ctb[i].val == 'D')
            printf("%.1f,", row->ctb[i].info.e);
    }
}

/*
 * Print the fields of each row belonging to tbl.
 * Hint use tbl_print_row() for correct formating
 * Undefined behavior if called before tbl_done_building()
 */
void    tbl_print(Table* tbl){
    if(tbl->tablebuilt == 0) return;
    Row* curr = tbl->head;
    for(int i = 0; i < tbl->rowcount; i++){
        tbl_print_row(curr);
        if(curr->next == NULL)break;
        curr = curr->next;
    }
}

/*
 * Return the type of a column, either S or D.
 * Undefined behavior if column is out of bounds
 * Undefined behavior if called before tbl_done_building()
 */
char    tbl_row_type_at( Row * row , int column ){
    //if(tablebuilt == 0) return -1;
    return row->ctb[column].val;
}

/*
 * Return the number of rows in the table
 */
int     tbl_row_count( Table * tbl ){
    counter();
    return tbl->rowcount;
}

void counter(Table  * tbl){
    int counter = 0;
    if(tbl == NULL) {
        return;
    }
    Row* curr = tbl->head;
    while(curr != NULL){
        counter++;
        if(curr->next == NULL) break;
        curr=curr->next;
    }
    tbl->rowcount = counter;
}

/*
 * Return an array with all rows of this table.
 * Undefined behavior if called before tbl_done_building()
 */
Row ** tbl_rows(Table *tbl){
    if(tbl->tablebuilt == 0) return NULL;
    
    return tbl->rows;
}






char* type_maker(Row* r){
    //if(tmcount == 0){
    p = calloc(r->fields+1, sizeof(char));
    t = calloc(1, sizeof(char));
    //}
    
    
    
    
    for(int i = 0; i < r->fields; i++){
        *t = r->ctb[i].val;
        strcat(p, t);
    }
    
    tmcount++;
    return p;
}

int find_max_fields(Table* tbl){
    if(tbl == NULL) return -1;
    int max = 0;
    Row* curr = tbl->head;
    while(curr != NULL){
        if(curr->fields > max)
        max = curr->fields;
        if(curr->next == NULL) break;
        curr = curr->next;
    }
    return max;
}

char* tbl_type(Table* tbl){
    if(tbl->tablebuilt == 0) return NULL;
    char* p = calloc(tbl_column_count(tbl)+1, sizeof(char));
    char* t = malloc(2*sizeof(char));
    
    t[1] = '\0';
    
    for(int i = 0; i < tbl_column_count(tbl); i++){
        
        t[0] = tbl_row_type_at(tbl_row_at(tbl, 0), i);
        strcat(p, t);
    }
    
    free(t);
    
    return p;
}

void tbl_free(Table* tbl){};

