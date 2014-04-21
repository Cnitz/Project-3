//
//  Column.c
//  
//
//  Created by Christian Nitz on 3/30/14.
//
//
#include "column.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ERROR .001

int find_lrg_class();
int count_classes();
double calc_left_double();
double calc_right_double();
double calc_left_str();
double calc_right_str();
int count_classes_left_double();
int count_classes_right_double();
int count_classes_left_str();
int count_classes_right_str();
void print_str();

//Represents a column. To be defined by the student.

struct column{
    union fields {
        char** f;
        double* d;
    } fields;
    unsigned int* class;
    unsigned int n;
    char type;
};

/* Initializes a new column from the given data. fields and classes are arrays of size n,
 * with the ith field corresponding to the ith class.
 */
Column *make_double_column(double *fields, unsigned int *classes, unsigned int n){
    Column* c = calloc(1, sizeof(Column));
    c->fields.d = calloc(n, sizeof(double));
    memcpy(c->fields.d, fields, sizeof(double)*n);
    c->class = calloc(n, sizeof(unsigned int));
    memcpy(c->class, classes, sizeof(unsigned int)*n);
    c->n = n;
    c->type = 'D';
    return c;
}

/* Initializes a new column from the given data. fields and classes are arrays of size n,
 * with the ith field corresponding to the ith class.
 */
Column *make_string_column(char **fields, unsigned int *classes, unsigned int n){
    Column* c = malloc(sizeof(Column));
    c->fields.f = calloc(n, sizeof(char*));
    c->class = calloc(n, sizeof(unsigned int));
	for(int i = 0; i < n; i++){
	c->fields.f[i] = calloc(strlen(fields[i])+1, sizeof(char));
	strcpy(c->fields.f[i], fields[i]);
	c->class[i] = classes[i];
	}
    c->n = n;
    c->type = 'S';
    return c;
}

/* Frees all memory associated with the given column.
 */
void free_column(Column *column){
    if(column == NULL) return;
    if(column->type == 'D')
        free(column->fields.d);
    else{
	for(int i = 0; i < column->n; i++){
        free(column->fields.f[i]);
		}
	free(column->fields.f);
	}

    free(column->class);
    free(column);
}

/* Determines if the rows contain only one unique field value.
 * Returns false (0) if not--true otherwise.
 */
int has_single_value(Column *column){
    if(column == NULL) return -1;
    if(column->n == 0) return 0;
    if(column->n <= 1) return 1;
    if(column->type == 'D'){
        for( int i = 0; i < column->n; i++){
            for(int k = 0; k < column->n; k++){
                if( fabs(column->fields.d[i]-column->fields.d[k]) <= .001)
                    continue;
                else
                    return 0;
                
            }
        }
    }
    if(column->type == 'S'){
        for( int i = 0; i < column->n; i++){
            for(int k = 0; k < column->n; k++){
                if(strcmp(column->fields.f[i], column->fields.f[k]) == 0)
                    continue;
                else
                    return 0;
                
            }
        }
    }
    
    return 1;
}

/* Determines if the rows contain only one unique class.
 * Returns false (0) if not--true otherwise.
 */
int is_impossible_split(Column *column){
    if(column == NULL) return -1;
    if(column->n == 0) return 0;
    if(column->n == 1) return 1;
    
        for( int i = 0; i < column->n; i++){
            for(int k = 0; k < column->n; k++){
                if(column->class[i] == column->class[k])
                    continue;
                else
                    return 0;
                
            }
        }
    
    return 1;
}

/* Calculates the entropy of the given column.
 * See the handout for the entropy formula.
 */
double calc_entropy(Column *column){
    if(column == NULL) return -1;
    double pi = 0, sum = 0, ni = 0;
    int m = find_lrg_class(column);
    //calc entropy
    for(int i = 0; i <= m; i++){
        if((ni = count_classes(column, i)) > 0)
            pi = ni/(double)column->n;
        else
            pi = 0;
        
        if(fabs(pi) > ERROR)
        sum += (pi*log2(pi));
       
    }
    return (-1.0)*sum;
}

/* Returns the best field value to split the given column on.
 * Assumes that the column can be split and has more than one field value.
 *
 * The best split-value will produce two columns with the least weighted sum of their entropies.
 * When splitting by double, the first column contains all rows with fields less than or equal to the split-value,
 * and the second column contains the remaining fields.
 */
double find_double_split_value(Column *column){
    double split = 0, entropy = 0, left = 0, right = 0, prev = 0, ret = 0, avg = 0, max = 0;
    int count = 0;
    //entropy = calc_entropy(column);
    for(int i = 0; i < column->n; i++){
        left = calc_left_double(column, column->fields.d[i]);
        right = calc_right_double(column, column->fields.d[i]);
        
        
        entropy = left + right;
        if(i == 0){
            prev = entropy;
            ret = column->fields.d[i];
            max = column->fields.d[i];
        }
      
        if(column->fields.d[i] > max + ERROR)
            max = column->fields.d[i];
        
        if(prev > entropy){
            ret = column->fields.d[i];
            prev = entropy;
        }
        
        if(fabs(entropy - prev) < ERROR){
            if(ret - ERROR < column->fields.d[i])
                ret = column->fields.d[i];
        }
        
    }
    
    for(int i = 0; i < column->n; i++){
     if(column->fields.d[i] > (ret + ERROR))
         if(column->fields.d[i] < max - ERROR)
             max = column->fields.d[i];
    }
    avg = (max+ret)/2.0;
    //printf("%.2f", ret);
    return avg;
}

/* Returns the best field value to split the given column on.
 * Assumes that the column can be split and has more than one field value.
 *
 * The best split-value will produce two columns with the least weighted sum of their entropies.
 * When splitting by string, the first column contains all rows with fields equal to the split-value,
 * and the second column contains the remaining fields.
 */
char *find_string_split_value(Column *column){
    double split = 0, entropy = 0, left = 0, right = 0, prev = 0;
    int count = 0;
    char* ret;
   
    for(int i = 0; i < column->n; i++){
        left = calc_left_str(column, column->fields.f[i]);
        right = calc_right_str(column, column->fields.f[i]);
        
        
        entropy = left + right;
        //printf("%.2f//%s\n", entropy, column->fields.f[i]);
        if(i == 0){
            prev = entropy;
            ret = column->fields.f[i];
        }
        if(fabs(prev-entropy) <ERROR){
            if(strcmp(column->fields.f[i], ret) > 0)
                ret = column->fields.f[i];
        }
      
        if(prev < entropy){
            ret = column->fields.f[i];
            prev = entropy;
        }
    }
    
    
    return ret;
}

double find_string_split_entropy(Column* column){
    double split = 0, entropy = 0, left = 0, right = 0, prev = 0;
    int count = 0;
    char* ret;
    
    for(int i = 0; i < column->n; i++){
        left = calc_left_str(column, column->fields.f[i]);
        right = calc_right_str(column, column->fields.f[i]);
        
        
        entropy = left + right;
        if(i == 0){
            prev = entropy;
            ret = column->fields.f[i];
        }
        
        if(prev < entropy){
            ret = column->fields.f[i];
            prev = entropy;
        }
    }
    
    
    return (-1.0)*prev;
    
}

double find_double_split_entropy(Column* column){
    double split = 0, entropy = 0, left = 0, right = 0, prev = 0, ret = 0, avg = 0, max = 0;
    int count = 0;
    //entropy = calc_entropy(column);
    for(int i = 0; i < column->n; i++){
        left = calc_left_double(column, column->fields.d[i]);
        right = calc_right_double(column, column->fields.d[i]);
        
        
        entropy = left + right;
        if(i == 0){
            prev = entropy;
            ret = column->fields.d[i];
            max = column->fields.d[i];
        }
        
        if(prev > entropy){
            ret = column->fields.d[i];
            prev = entropy;
        }
    }
    // printf("%.2f", avg);
    return (-1.0)*prev;
    
}



int find_lrg_class(Column* c){
    if(c == NULL) return -1;
    int ret = 0;
    
    for(int i = 0; i < c->n; i++){
        if(c->class[i] > ret)
            ret = (int)c->class[i];
    }
    return ret;
    
    
}

int count_classes(Column* c, int n){
    if(c == NULL) return -1;
    int count = 0;
    for(int i = 0; i < c->n; i++){
        if(c->class[i] == n)
            count++;
    }
    return count;
}

//*********** calc entropy for DOUBLE columns of left and right *********
double calc_left_double(Column* c, double value){
    int m = 0, count = 0;
    double pi = 0, ni = 0, sum = 0;
    for(int i = 0; i < c->n ; i++){
        if(c->fields.d[i] <= value + ERROR){
            count++;
            if(c->class[i] > m)
                m = c->class[i];
        }
    }
   
    for(int i = 0; i <= m; i++){
       if((ni = count_classes_left_double(c, i, value)) > 0)
            pi = ni/(double)count;
        else
            pi = 0;
        
        if(fabs(pi) > ERROR)
        sum += (pi*log2(pi));
        
    }
    return (-1.0)*sum*count;
    
}

double calc_right_double(Column* c, double value){
    int m = 0, count = 0;
    double pi = 0, ni = 0, sum = 0;
    for(int i = 0; i < c->n ; i++){
        if(c->fields.d[i] > value + ERROR){
            count++;
            if(c->class[i] > m)
                m = c->class[i];
        }
    }
    for(int i = 0; i <= m; i++){
        if((ni = count_classes_right_double(c, i, value)) > 0)
            pi = ni/(double)count;
        else
            pi = 0;
        if(fabs(pi) > ERROR)
        sum += (pi*log2(pi));
        
    }
    return (-1.0)*sum*count;
}
//*********** calc tropy for STRING columns of left and right *********
double calc_left_str(Column* c, char* value){
    int m = 0, count = 0;
    double pi = 0, ni = 0, sum = 0;
    for(int i = 0; i < c->n ; i++){
        if(strcmp(c->fields.f[i], value) == 0){
            count++;
            if(c->class[i] > m)
                m = c->class[i];
        }
    }
    
    
    for(int i = 0; i <= m; i++){
        if((ni = count_classes_left_str(c, i, value)) > 0)
            pi = ni/(double)count;
        else
            pi = 0;
        if(fabs(pi) > ERROR)
        sum += (pi*log2(pi));
        
    }
    return sum*count;
}

double calc_right_str(Column* c, char* value){
    int m = 0, count = 0;
    double pi = 0, ni = 0, sum = 0;
    for(int i = 0; i < c->n ; i++){
        if(strcmp(c->fields.f[i], value) != 0){
            count++;
            if(c->class[i] > m)
                m = c->class[i];
        }
    }
    for(int i = 0; i <= m; i++){
        
        if((ni = (double)count_classes_right_str(c, i, value)) > 0){
            //printf("%d\n", count);
            pi = ni/(double)count;
        }
        else
            pi = 0;
        if(fabs(pi) > ERROR)
        sum += (pi*log2(pi));
       //  printf("%f\n",sum);
        
    }
    
    return sum*count;
}
//****** counts columns class in right and left DOUBLE columns *****
int count_classes_left_double(Column* c, int n, double value){
    if(c == NULL) return -1;
    int count = 0;
    for(int i = 0; i < c->n; i++){
        if(c->fields.d[i] <= (value + ERROR)){
        if(c->class[i] == n)
            count++;
        }
    }
    return count;
}

int count_classes_right_double(Column* c, int n, double value){
    if(c == NULL) return -1;
    int count = 0;
    for(int i = 0; i < c->n; i++){
        if(c->fields.d[i] > value + ERROR){
            if(c->class[i] == n)
                count++;
        }
    }
    return count;
}
//***** counts columns class in right and left STRING columns *****

int count_classes_left_str(Column* c, int n, char* value){
    if(c == NULL) return -1;
    int count = 0;
    for(int i = 0; i < c->n; i++){
        if(strcmp(c->fields.f[i], value) == 0){
            if(c->class[i] == n)
                count++;
        }
    }
    return count;
}

int count_classes_right_str(Column* c, int n, char* value){
    if(c == NULL) return -1;
    int count = 0;
    for(int i = 0; i < c->n; i++){
        if(strcmp(c->fields.f[i], value) != 0){
            if(c->class[i] == n)
                count++;
        }
    }
    
    return count;
}
//for testing purposes.
void print_str(Column* c, int i){
    printf("%s\n", c->fields.f[i]);
    printf("working\n");
    
}

