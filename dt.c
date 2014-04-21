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
    if(tbl == NULL) return;
   // printf("\n");
    Node* n = n_make();
	Column* c;
    int count = 0;
    int cols = tbl_column_count(tbl), index = 0;
    
    if(tbl_row_count(tbl) <= 1){
        n->leaf = 1;
        n->class = (unsigned int)tbl_double_at(tbl_row_at(tbl, 0), cols-1);
        t_set_data(tree, n);
        tree->left = NULL;
        tree->right = NULL;
        return;
    }
    
    
    Table* left;
    Table* right;
   // tbl_print(tbl);
    double prev = 0xFFFFFFFFFFFFFFFF, entropy = 0, split_d = 0;
    char* split_s;
    char type;
    
   //tbl_print(tbl);
  //  printf("%d", cols);
    for(int i = 0; i < cols-1; i++){
        
         c = get_column(tbl, i);
       // printf("%s\n", c->fields.f[i]);
        if(tbl_row_type_at(tbl_row_at(tbl, 0), i) == 'S'){
            if(has_single_value(c)) {
                count++;
		free_column(c);
                continue;
            }
            
            entropy = find_string_split_entropy(c);
           // printf("%.2f\n", entropy);
        }
        
        if(tbl_row_type_at(tbl_row_at(tbl, 0), i) == 'D'){
            if(has_single_value(c)) {
                count++;
		free_column(c);
                continue;
            }
            
            entropy = find_double_split_entropy(c);
        }
        
        
        if(entropy < prev){
            
            prev = entropy;
            index = i;
            
            if(tbl_row_type_at(tbl_row_at(tbl, 0), index) == 'D'){
                //split_d = find_double_split_value(c);
                type = 'D';
            }
            
            if(tbl_row_type_at(tbl_row_at(tbl, 0), index) == 'S'){
		
		//split_s = find_string_split_value(c);
               // strcpy(split_s, find_string_split_value(c));
                type = 'S';
                
              //  printf("%s\n", split_s);
            }
        }
        free_column(c);

    }
   

    if(count == tbl_column_count(tbl) -1){
        n->leaf = 1;
        n->class = (unsigned int)tbl_double_at(tbl_row_at(tbl, 0), cols-1);
        t_set_data(tree, n);
        tree->left = NULL;
        tree->right = NULL;
        return;
    }
    Column* first = get_column(tbl, 0);
    if(is_impossible_split(first) == 1){
        n->leaf = 1;
        n->class = (unsigned int)tbl_double_at(tbl_row_at(tbl, 0), cols-1);
        t_set_data(tree, n);
        tree->left = NULL;
        tree->right = NULL;
	free_column(first);
        return;
    }
	free_column(first);
   // printf("%s\n", split_s);
    
    if(type == 'S'){
        c = get_column(tbl, index);
        n->entropy = prev;
        n->field.s = calloc(strlen(find_string_split_value(c))+1, sizeof(char));
	
        strcpy(n->field.s, find_string_split_value(c));
	
	
        n->type = type;
        n->leaf = 0;
        n->column = index;
        t_set_data(tree, n);
       
        //build two seperate tables
        left = build_string_left_table(tbl, find_string_split_value(c), index);
        right = build_string_right_table(tbl, find_string_split_value(c), index);
        free_column(c);
        
        //recursion
        tree->left = t_make();
        tree->right = t_make();
	
        dt_build(left, tree->left);
        dt_build(right, tree->right);

	if(left != NULL)
        tbl_free(left);
	if(right != NULL)
        tbl_free(right);
        
        return;
    }
    
    if(type == 'D'){
	c = get_column(tbl, index);
        n->entropy = prev;
        n->field.d = find_double_split_value(c);
        n->type = type;
        n->leaf = 0;
        n->column = index;
        t_set_data(tree, n);
        
        //build two seperate tables
        left = build_double_left_table(tbl, find_double_split_value(c), index);
        right = build_double_right_table(tbl, find_double_split_value(c), index);
	free_column(c);       

        //recursion
        tree->left = t_make();
        tree->right = t_make();
        dt_build(left, tree->left);
        dt_build(right, tree->right);
        if(left != NULL)
        tbl_free(left);
	if(right != NULL)
        tbl_free(right);
        return;
    }
    
    
    return;
}







void dt_free(void *data){
	if(data == NULL) return;
	if(((Node*)data)->type == 'S'){
		free(((Node*)data)->field.s);
}
free(((Node*)data));
    return;
}







void dt_print(void *data){
    if(data == NULL) return;
    

    if(((Node*)data)->leaf == 1){
        printf("C:%d\n", ((Node*)data)->class);
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
  //  printf("%c\n", tbl_row_type_at(tbl_row_at(tbl, 0), index));
    char type = tbl_row_type_at(tbl_row_at(tbl, 0), index);
    Column* column;
    char** strings;
    double* doubles;
    unsigned int* classes = calloc(rows, sizeof(unsigned int));
    int last = tbl_column_count(tbl)-1;
    
   // printf("%.2f\n", tbl_double_at(tbl_row_at(tbl, 0), last));
   
    for(int i = 0; i < rows; i++){
        classes[i] = (unsigned int)tbl_double_at(tbl_row_at(tbl, i), last);
        
    }
    
    if(type == 'S'){
        
        strings = calloc(rows, sizeof(char*));
        
        for(int i = 0; i < rows; i++){
            strings[i] = tbl_string_at(tbl_row_at(tbl, i), index);
        }
        for(int i = 0; i < rows; i++){
           // printf("%s\n", strings[i]);
        }
       column = make_string_column(strings, classes, rows);
	free(strings);
        
    }
    
    if(type == 'D'){
        doubles = calloc(rows, sizeof(double));
        for(int i = 0; i < rows; i++){
            doubles[i] = tbl_double_at(tbl_row_at(tbl, i), index);
        }
       column = make_double_column(doubles, classes, rows);
	free(doubles);
    }
   

    free(classes);
    return column;
}

Table* build_string_left_table(Table* tbl, char* split, int index){
    
    int n = 0, g = 0, j = 0;
    for(int i = 0; i < tbl_row_count(tbl); i++){
        if(strcmp(tbl_string_at(tbl_row_at(tbl, i), index), split) == 0){
            g++;
        }
    }
if(g == 0) return NULL;
    
    int* loc = calloc(g, sizeof(int));
    
    for(int i = 0; i < tbl_row_count(tbl); i++){
        if(strcmp(tbl_string_at(tbl_row_at(tbl, i), index), split) == 0){
            loc[j] = i;
            j++;
        }
    }
    
    Table* ret = tbl_make();
    char* type = tbl_type(tbl);
    
    int cols = tbl_column_count(tbl);
    
    for(int i = 0; i < g; i++){
        
        tbl_start_row(ret, cols);
        for(int k = 0; k < cols; k++){
            
            if(type[k] == 'D'){
                double d = tbl_double_at(tbl_row_at(tbl, loc[i]), k);
                tbl_add_double_to_row(ret, d);
            }
            
            if(type[k] == 'S'){
                
                char* str = calloc(strlen(tbl_string_at(tbl_row_at(tbl, loc[i]), k))+1, sizeof(char));
                strcpy(str, tbl_string_at(tbl_row_at(tbl, loc[i]), k));
                tbl_add_string_to_row(ret, str);
		free(str);
            }
        }
    }
   
 
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
if(g == 0) return NULL;
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
                char* str = calloc(strlen(tbl_string_at(tbl_row_at(tbl, loc[i]), k))+1, sizeof(char));
                strcpy(str, tbl_string_at(tbl_row_at(tbl, loc[i]), k));
                tbl_add_string_to_row(ret, str);
		free(str);
            }
        }
    }
   
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
if(g == 0) return NULL;
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
                
               // strcpy(str, tbl_string_at(tbl_row_at(tbl, loc[i]), k));
                tbl_add_string_to_row(ret, tbl_string_at(tbl_row_at(tbl, loc[i]), k));
            }
        }
    }
    
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
	if(g == 0) return NULL;
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
               
                tbl_add_string_to_row(ret, tbl_string_at(tbl_row_at(tbl, loc[i]), k));
            }
        }
    }
    
    free(loc);
    tbl_done_building(ret);
    return ret;
}

Node* n_make(){
    Node* n = calloc(1, sizeof(Node));
    n->entropy = 0.0;
    n->type = '\0';
    n->leaf = 0;
    n->class = 0;
    n->column = 0;
    return n;
}


