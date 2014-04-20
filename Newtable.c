//
//  table.c
//
//
//  Created by Christian Nitz on 2/17/14.
//
//

#include "new_defs_imp.h"



char* type_maker();
int find_max_fields();
void counter();
char* p;
char* t;
char* tbl_type();
void tbl_print();
char* type_maker();
Row* tbl_row_at();
void tbl_free_row();
void tbl_print_row();
void tester();

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
    ctb->cols = 0;
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
   
    
    Row* r = calloc(1, sizeof(Row));
    r->fields = num_fields;
    r->type = calloc(num_fields+1, sizeof(char));
    r->pos = 0;
    r->data = calloc(num_fields, sizeof(Data));
    r->next = NULL;
    
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
    tbl->tail->next = NULL;
    
}


/*
 * Add a NULL-terminated string as a field of the current row. Undefined
 * behavior if tbl_done_building() has been called or if the row's fields are already
 * full.
 */
void    tbl_add_string_to_row(Table* tbl, char * str) {
    //check for valid information
    if(tbl->tablebuilt == 1) return;
    if(tbl->tail->pos >= tbl->tail->fields) return;
    
    //make room for str
    tbl->tail->data[tbl->tail->pos].c = calloc(strlen(str)+1, sizeof(char));
    
    //add type and str to row
    strcat(tbl->tail->type, "S");
    strcpy(tbl->tail->data[tbl->tail->pos].c, str);
    
    //increment pos
    tbl->tail->pos++;
    
}

/*
 * Add a double as a field of the current row. Undefined behavior if
 * tbl_done_building() has been called or if the row's fields are already full.
 */
void    tbl_add_double_to_row(Table* tbl, double d) {
    //check for valid information
    if(tbl->tablebuilt == 1) return;
    if(tbl->tail->pos >= tbl->tail->fields) return;
    
    //add type and str to row
    strcat(tbl->tail->type, "D");
    tbl->tail->data[tbl->tail->pos].d = d;
  
    //increment pos
    tbl->tail->pos++;
    
    
    
    
}

/*
 * Ends construction of the table:
 * Finds table type,
 * Removes all rows differing from table type
 * A completed array for Table is finished
 */
Table  *tbl_done_building(Table* tbl){
    if(tbl == NULL || tbl->rowcount == 0) return NULL;
    tbl->tablebuilt = 1;
    int row_count = tbl->rowcount;
    //tbl_print(tbl);
    Row* curr = tbl->head;
    Row* hold;
    Row* head;
    Row* tail;
    char* now;
    char* prev;
    
   
    
    if(tbl->rowcount < 2){
        tbl->rows = calloc(tbl->rowcount, sizeof(Row*));
        tbl->rows[0] = curr;
        tbl->masterkey = calloc(strlen(curr->type)+1, sizeof(char));
        strcpy(tbl->masterkey, curr->type);
        tbl->cols = strlen(tbl->masterkey);
        return tbl;
        }
    
    
    int rows = find_max_fields(tbl);
    
    Row* prevs;
    

    
    now = curr->type;
    
    
    while(curr != NULL){
        if(curr->next == NULL) break;
        
        prev = now;
        now = curr->type;
        
        if(strcmp(now, prev) == 0){
            tbl->masterkey = calloc(strlen(now)+1, sizeof(char));
            strcpy(tbl->masterkey, now);
            break;
        }
        curr=curr->next;
    }
    
    tbl->rowcount = 0;
    Table* temp = calloc(1, sizeof(*tbl));
    
    if(tbl->masterkey == NULL) return NULL;
    else{
        curr = tbl->head;
        int k = 0;
        //TODO: fix infinite loop
        for(int i = 0; i< row_count; i++){
            now = curr->type;
            

            if(strcmp(tbl->masterkey, now)==0){
                if(tbl->rowcount == 0){
                    temp->head = curr;
                    tbl->rowcount++;
                }
                else if(tbl->rowcount == 1){
                    temp->head->next = curr;
                    temp->tail = curr;
                    tbl->rowcount++;
                    
                }
                else {
                    temp->tail->next = curr;
                    temp->tail = curr;
                    tbl->rowcount++;
                }
                
                
            }
            else if(strcmp(tbl->masterkey, now) != 0){
                if(curr->next == NULL)break;
                hold = curr;
                curr = curr->next;
                tbl_free_row(hold);
                continue;
                
            }
            
            // tbl_print_row(curr);
            
            if(curr->next == NULL) break;
            
           
            curr = curr->next;
           
        }
        
    }
    
    tbl->head = temp->head;
    tbl->tail = temp->tail;
    free(temp);
    
    curr = tbl->head;
    int i = 0;
    tbl->rows = calloc(tbl->rowcount, sizeof(Row*));
   
    while(curr != NULL){
        tbl->rows[i] = curr;
        i++;
        if(curr->next == NULL) break;
        curr = curr->next;
    }
tbl->cols = strlen(tbl->masterkey);
    return tbl;
    
}

/*
 * Return the number of columns/fields in the Table.
 */
int tbl_column_count( Table * tbl ){
    if(tbl->tablebuilt == 0) return 0;
    
    return tbl->cols;
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
    if(row->type[at] == 'S')
        return row->data[at].c;
    return NULL;
}

/*
 * Return the double value from the fields/columns of row in the at-th position
 * Undefined behavior if at is out of bounds or at-th field is not a double.
 */
double  tbl_double_at( Row * row, int at){
    if(row->fields <= at) return -1;
    if(row->type[at] == 'D')
        return row->data[at].d;
    return -1;
}

/*
 * Print each field of r separated by commas and ending with a newline
 */
void    tbl_print_row( Row * row ){
    for(int i = 0; i < row->fields; i++){
        
        if(i == row->fields-1){
            if(row->type[i] == 'S')
                printf("\"%s\"\n", row->data[i].c);
            if(row->type[i] == 'D')
                printf("%.2f\n", row->data[i].d);
            break;
        }
        if(row->type[i] == 'S')
            printf("\"%s\",", row->data[i].c);
        if(row->type[i] == 'D')
            printf("%.2f,", row->data[i].d);
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
    return row->type[column];
}

/*
 * Return the number of rows in the table
 */
int     tbl_row_count( Table * tbl ){
    if(tbl == NULL) return -1;
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
    
    return tbl->masterkey;
}


void tbl_free_row(Row* r){
    if(r == NULL) return;
    for(int i = 0; i < r->fields; i++){
        if(r->type[i] == 'S')
            if(r->data[i].c != NULL)
                free(r->data[i].c);
    }
    if(r->data != NULL)
        free(r->data);
    free(r->type);
    free(r);
}
                       
void tbl_free(Table* tbl){
    if(tbl == NULL) return;
    for(int i = 0; i < tbl->rowcount; i++){
        tbl_free_row(tbl->rows[i]);
    }
    if(tbl->rows != NULL)
    free(tbl->rows);
    if(tbl->masterkey != NULL)
        free(tbl->masterkey);
    free(tbl);
    
}

void tester(Row* r){
    printf("%c", r->type[0]);
}
                       
                       
                       

