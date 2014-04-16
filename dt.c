//
//  dt.c
//  
//
//  Created by Christian Nitz on 4/10/14.
//
//
#include "defs_itf.h"


Column* get_column();
Table* build_string_left_table();
Table* build_string_right_table();
Table* build_double_left_table();
Table* build_double_right_table();
Node* n_make();


void dt_build(Table *tbl, Tree *tree){
   // printf("\n");
    Node* n = n_make();
    int count = 0;
    int cols = tbl_column_count(tbl), index = 0;
    if(tbl_row_count(tbl) <= 1){
        n->leaf = 1;
        
        tbl_row_at(tbl, 0);
        n->class = (unsigned int)tbl_double_at(tbl_row_at(tbl, 0), cols-1);
        tree->data = n;
        return;
    }
    
    Table* left;
    Table* right;
   // tbl_print(tbl);
    double prev = 900, entropy = 0, split_d = 0;
    char* split_s;
    char type;
    
   //tbl_print(tbl);
  //  printf("%d", cols);
    for(int i = 0; i < cols-1; i++){
        
        if(tbl_row_type_at(tbl_row_at(tbl, 0), i) == 'S'){
            if(has_single_value(get_column(tbl, i))) {
                count++;
                continue;
            }
            entropy = find_string_split_entropy(get_column(tbl, i));
           // printf("%.2f\n", entropy);
        }
        
        if(tbl_row_type_at(tbl_row_at(tbl, 0), i) == 'D'){
            if(has_single_value(get_column(tbl, i))) {
                count++;
                continue;
            }
            entropy = find_double_split_entropy(get_column(tbl, i));
        }
        
     /*   if(i == 0){
            prev = entropy;
            index = i;
                   
            if(tbl_row_type_at(tbl_row_at(tbl, 0), index) == 'D'){
                       split_d = find_double_split_value(get_column(tbl, index));
                       type = 'D';
            }
            
            if(tbl_row_type_at(tbl_row_at(tbl, 0), index) == 'S'){
                split_s = find_string_split_value(get_column(tbl, index));
                type = 'S';
                
               printf("%s\n\n", split_s);
            }
        } */
        
        if(entropy < prev){
            
            prev = entropy;
            index = i;
            
            if(tbl_row_type_at(tbl_row_at(tbl, 0), index) == 'D'){
                split_d = find_double_split_value(get_column(tbl, index));
                type = 'D';
            }
            
            if(tbl_row_type_at(tbl_row_at(tbl, 0), index) == 'S'){
                split_s = find_string_split_value(get_column(tbl, index));
                type = 'S';
                
               // printf("%s\n", split_s);
            }
        }
        
    }
   

    if(count == tbl_column_count(tbl) -1){
        n->leaf = 1;
        n->class = (unsigned int)tbl_double_at(tbl_row_at(tbl, 0), cols-1);
        tree->data = n;
        return;
    }
    
    if(is_impossible_split(get_column(tbl, 0)) == 1){
        n->leaf = 1;
        n->class = (unsigned int)tbl_double_at(tbl_row_at(tbl, 0), cols-1);
        tree->data = n;
        return;
    }
    
    if(type == 'S'){
        
        n->entropy = prev;
        n->field.s = split_s;
        n->type = type;
        n->leaf = 0;
        n->column = index;
        tree->data = n;
       
        //build two seperate tables
        left = build_string_left_table(tbl, split_s, index);
        right = build_string_right_table(tbl, split_s, index);
       
        //recursion
        tree->left = t_make();
        tree->right = t_make();
        
        dt_build(left, tree->left);
        dt_build(right, tree->right);
        return;
    }
    
    if(type == 'D'){
        n->entropy = prev;
        n->field.d = split_d;
        n->type = type;
        n->leaf = 0;
        n->column = index;
        tree->data = n;
        
        //build two seperate tables
        left = build_double_left_table(tbl, split_d, index);
        right = build_double_right_table(tbl, split_d, index);
       
        //recursion
        tree->left = t_make();
        tree->right = t_make();
        dt_build(left, tree->left);
        dt_build(right, tree->right);
        return;
    }
    
    
    return;
}







void dt_free(void *data){
    return;
}







void dt_print(void *data){
    if(data == NULL) return;
    

    if(((Node*)data)->leaf == 1){
        printf("C:%d", ((Node*)data)->class);
    }
    if(((Node*)data)->type == 'S'){
        printf("%s", colnames[((Node*)data)->column]);
        printf("=");
        printf("%s\n", ((Node*)data)->field.s);
        
    }
    if(((Node*)data)->type == 'D'){
        printf("%s", colnames[((Node*)data)->column]);
        printf("=");
        printf("%.2f\n", ((Node*)data)->field.d);
    }

}

extern char **colnames;

Column* get_column(Table* tbl, int index){
    int rows = tbl_row_count(tbl);
    char type = tbl_row_type_at(tbl_row_at(tbl, 0), index);
    Column* column;
    char** strings;
    double* doubles;
    unsigned int* classes = malloc(rows*sizeof(unsigned int));
    int last = tbl_column_count(tbl)-1;
    
 
    for(int i = 0; i < rows; i++){
        classes[i] = (unsigned int)tbl_double_at(tbl_row_at(tbl, i), last);
        
    }
    
    if(type == 'S'){
        strings = calloc(rows, sizeof(char*));
        
        for(int i = 0; i < rows; i++){
            strings[i] = calloc(strlen(tbl_string_at(tbl_row_at(tbl, i), index))+1, sizeof(char));
            strcpy(strings[i], tbl_string_at(tbl_row_at(tbl, i), index));
        }
       column = make_string_column(strings, classes, rows);
        
        
        
    }
    if(type == 'D'){
        doubles = calloc(rows, sizeof(double));
        for(int i = 0; i < rows; i++){
            doubles[i] = tbl_double_at(tbl_row_at(tbl, i), index);
        }
       column = make_double_column(doubles, classes, rows);
    }
    
    return column;
}

Table* build_string_left_table(Table* tbl, char* split, int index){
    
    int n = 0, g = 0, j = 0;
    for(int i = 0; i < tbl_row_count(tbl); i++){
        if(strcmp(tbl_string_at(tbl_row_at(tbl, i), index), split) == 0){
            g++;
        }
    }
    int* loc = calloc(g, sizeof(int));
    
    for(int i = 0; i < tbl_row_count(tbl); i++){
        if(strcmp(tbl_string_at(tbl_row_at(tbl, i), index), split) == 0){
            loc[j] = i;
            j++;
        }
    }
    
    Table* ret = tbl_make();
    char* type = tbl_type(tbl);
    
    
    
    for(int i = 0; i < g; i++){
        
        tbl_start_row(ret, tbl_column_count(tbl));
        
        for(int k = 0; k < tbl_column_count(tbl); k++){
            
            if(type[k] == 'D'){
                double d = tbl_double_at(tbl_row_at(tbl, loc[i]), k);
                tbl_add_double_to_row(ret, d);
            }
            
            if(type[k] == 'S'){
                char *str = tbl_string_at(tbl_row_at(tbl, loc[i]), k);
                tbl_add_string_to_row(ret, str);
            }
        }
    }
    free(type);
    free(loc);
     
    tbl_done_building(ret);
    return ret;
}

Table* build_string_right_table(Table* tbl, char* split, int index){
    
    int n = 0, g = 0, j = 0;
    
    for(int i = 0; i < tbl_row_count(tbl); i++){
        if(strcmp(tbl_string_at(tbl_row_at(tbl, i), index), split) != 0){
            g++;
        }
    }
    int* loc = calloc(g, sizeof(int));
    
    for(int i = 0; i < tbl_row_count(tbl); i++){
        if(strcmp(tbl_string_at(tbl_row_at(tbl, i), index), split) != 0){
            loc[j] = i;
            j++;
        }
    }
    
    Table* ret = tbl_make();
    char* type = tbl_type(tbl);
    
    
    
    for(int i = 0; i < g; i++){
        
        tbl_start_row(ret, tbl_column_count(tbl));
        
        for(int k = 0; k < tbl_column_count(tbl); k++){
            
            if(type[k] == 'D'){
                double d = tbl_double_at(tbl_row_at(tbl, loc[i]), k);
                tbl_add_double_to_row(ret, d);
            }
            
            if(type[k] == 'S'){
                char *str = tbl_string_at(tbl_row_at(tbl, loc[i]), k);
                tbl_add_string_to_row(ret, str);
            }
        }
    }
    free(type);
    free(loc);
     
    tbl_done_building(ret);
    return ret;
}

Table* build_double_left_table(Table* tbl, double split, int index){
    
    int n = 0, g = 0, j = 0;
    for(int i = 0; i < tbl_row_count(tbl); i++){
        if(tbl_double_at(tbl_row_at(tbl, i), index) < split){
            g++;
        }
    }
    int* loc = calloc(g, sizeof(int));
    
    for(int i = 0; i < tbl_row_count(tbl); i++){
        if(tbl_double_at(tbl_row_at(tbl, i), index) < split){
            loc[j] = i;
            j++;
        }
    }
    
    Table* ret = tbl_make();
    char* type = tbl_type(tbl);
    
    
    
    for(int i = 0; i < g; i++){
        
        tbl_start_row(ret, tbl_column_count(tbl));
        
        for(int k = 0; k < tbl_column_count(tbl); k++){
            
            if(type[k] == 'D'){
                double d = tbl_double_at(tbl_row_at(tbl, loc[i]), k);
                tbl_add_double_to_row(ret, d);
            }
            
            if(type[k] == 'S'){
                char *str = tbl_string_at(tbl_row_at(tbl, loc[i]), k);
                tbl_add_string_to_row(ret, str);
            }
        }
    }
    free(type);
    free(loc);
     
    tbl_done_building(ret);
    return ret;

}

Table* build_double_right_table(Table* tbl, double split, int index){
    int n = 0, g = 0, j = 0;
    for(int i = 0; i < tbl_row_count(tbl); i++){
        if(tbl_double_at(tbl_row_at(tbl, i), index) > split){
            g++;
        }
    }
    int* loc = calloc(g, sizeof(int));
    
    for(int i = 0; i < tbl_row_count(tbl); i++){
        if(tbl_double_at(tbl_row_at(tbl, i), index) > split){
            loc[j] = i;
            j++;
        }
    }
    
    Table* ret = tbl_make();
    char* type = tbl_type(tbl);
    
    
    
    for(int i = 0; i < g; i++){
        
        tbl_start_row(ret, tbl_column_count(tbl));
        
        for(int k = 0; k < tbl_column_count(tbl); k++){
            
            if(type[k] == 'D'){
                double d = tbl_double_at(tbl_row_at(tbl, loc[i]), k);
                tbl_add_double_to_row(ret, d);
            }
            
            if(type[k] == 'S'){
                char *str = tbl_string_at(tbl_row_at(tbl, loc[i]), k);
                tbl_add_string_to_row(ret, str);
            }
        }
    }
    free(type);
    free(loc);
    tbl_done_building(ret);
    return ret;
}

Node* n_make(){
    Node* n = calloc(1, sizeof(Node));
    return n;
}


