#include "defs_itf.h"

/*
 * confmatrix API
 */


/*
 * Represents the confusion matrix. To be defined by the student.
 */
int find_expected();
int find_actual();


struct confmatrix {
    int classes;
    int total;
    int errors;
    int** table;
};




/* cm_make:
 *   creates and returns a ConfMatrix, where classes is the number of
 *   classes the confusion matrix will be considering.
 */
ConfMatrix* cm_make(int classes){
    ConfMatrix* cm = calloc(1, sizeof(ConfMatrix));
    cm->classes = classes;
    cm->total = 0;
    cm->errors = 0;
    cm->table = calloc(classes, sizeof(int));
    for(int i = 0; i < classes; i++){
        cm->table[i] = calloc(classes, sizeof(int));
    }
    return cm;
}

/*
 * cm_validate:
 *   This function takes a table [tbl], a "trained" decision tree [tree],
 *   and attempts to determine how well the tree predicts classes.
 *   It will take each row of the table, and compare the actual class
 *   in the table to the predicted class it finds from the tree.
 *   Each prediction will be recorded in the confusion matrix [cm].
 */
void cm_validate(ConfMatrix *cm, Table *tbl, Tree *tree){
    int expected = 0;
    int actual = 0;

    for(int i = 0; i < tbl_row_count(tbl); i++){
        expected = find_expected(tbl_row_at(tbl, i), tree);
        actual = find_actual(tbl_row_at(tbl, i), tbl_column_count(tbl)-1);
        if(actual != expected) cm->errors++;
        cm->table[actual][expected]++;
        cm->total++;
    }

    
}

/*
 * cm_print:
 *   prints ConfMatrix cm with the format:
 *
 *   Confusion matrix: total=X, errors=Y
 *    | 0 1
 *   0|a,b,
 *   1|c,d,
 *
 *   where the X is the number of predictions, Y is the number
 *   of wrong predictions, and a,b,c,d,etc. are the specific numbers
 *   for each class that was predicted. Note that there is a trailing
 *   new line after the last row of text printed.
 */
void cm_print(ConfMatrix* cm){
    printf("Confusion matrix: total=%d, errors=%d \n", cm->total, cm->errors);
    printf(" |");
    for(int i = 0; i < cm->classes; i++){
        printf(" %d", i);
    }
    
    printf("\n");
    
    for(int i = 0; i < cm->classes; i++){
        printf("%d|",i);
        for(int k = 0; k < cm->classes; k++){
            printf("%d,", cm->table[i][k]);
        }
        printf("\n");
    }
    
    
}

/*
 * cm_free():
 *   You know the drill!
 */
void cm_free(ConfMatrix* cm){
    if(cm == NULL) return;
    int classes = cm->classes;
    for(int i = 0; i < classes; i++){
        free(cm->table[i]);
    }
    free(cm->table);
    free(cm);
}

/*
 * tbl_classes_count:
 *   returns the number of classes in a table. You will need
 *   this to know how large to make the confusion matrix.
 */
int tbl_classes_count(Table *tbl){
    double max;
    int cols = tbl_column_count(tbl)-1;
    int count = 0;
    
    for(int i = 0; i < tbl_row_count(tbl); i++){
        if(i == 0){
            max = tbl_double_at(tbl_row_at(tbl,i), cols);
            continue;
        }
        if(tbl_double_at(tbl_row_at(tbl,i), cols) > (max + 0.001))
            max = tbl_double_at(tbl_row_at(tbl,i), cols);
        
    }
    
    for(int i = 0; i <= (int)max; i++){
        for(int j = 0; j < tbl_row_count(tbl); j++){
            if(i  == (unsigned int)tbl_double_at(tbl_row_at(tbl,j), cols)){
                count++;
                break;
            }
        }
    }
    return count;
    
}

int find_expected(Row* row, Tree* t){
    
    Node* n = t_data(t);
    if(n->leaf == 1){
        return n->class;
        
    }
    if(n->type == 'S'){
        if(strcmp(n->field.s, tbl_string_at(row, n->column)) == 0)
            return find_expected(row, t_left(t));
        else
            return find_expected(row, t_right(t));
    }
    if(n->type == 'D'){
        if(tbl_double_at(row, n->column) < n->field.d)
            return find_expected(row, t_left(t));
        else
            return find_expected(row, t_right(t));
    }
    return -1;
}

int find_actual(Row* row, int last_column){
    return (int)tbl_double_at(row, last_column);
}

