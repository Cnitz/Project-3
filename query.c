//
// query.c
//
//
// Created by Christian Nitz on 3/19/14.
//
//

#include "q_defs.h"

void print_data();
int num_cons();
void(*f)(void*) = print_data;
void testing();
int find_space();
int find_conn();
int grammar_checker();
int find_conditional();
int find_column_name();
char* column_name();
Tree* build_tree();
char* str_at();
char **columns;
char* types;
int cc;


Tree *q_parse(char *query_text, char **column_names, char *column_types, int num_columns){
    if(grammar_checker(query_text) == 0) {
        return NULL;}
    
    
    columns = column_names;
    cc = num_columns;
    types = column_types;
    
    Tree* ret = build_tree(query_text);
    
    
    return ret;
}

//TODO: make this work
void q_free(Tree *query){
    
    if(t_left(query) != NULL)
        q_free(t_left(query));
    if(t_right(query) != NULL)
        q_free(t_right(query));
    if(query->data != NULL)
        free(query->data);
    if(query != NULL){
        
        free(query);
    }
}


void q_print(Tree *data){
    if(data == NULL) return;
    int space = 0;
    t_print(data, space, f);
}

//TODO: FIX
int q_get_type(void *query_data){
    
    if(query_data == NULL) return -1;
    
    // printf("%d", *((int*)query_data));
    if(*((int*)query_data) == 0) return 0;
    else if (*((int*)query_data) == 1) return 1;
    else if (strstr(query_data, "<=") != NULL) return 5;
    else if (strstr(query_data, ">=") != NULL) return 6;
    else if (strstr(query_data, "<") != NULL) return 3;
    else if (strstr(query_data, ">") != NULL) return 4;
    else if (strstr(query_data, "=") != NULL) return 2;
    else return -1;
    
}


int q_get_col_index(void *query_data){
	char* name = column_name(query_data);
    for(int i = 0; i < cc; i++){
        
        if(strcmp(name, columns[i]) == 0){
            free(name);
            return i;
        }
    }
    return -1;
}


double q_get_double(void *query_data){
    int* err = calloc(1, sizeof(int));
    double r = rd_parse_number((char*)query_data, find_conditional(query_data), strlen(query_data), err);
    free(err);
    return r;
}


char *q_get_str(void *query_data){
	char* p = rd_parse_string((char*)query_data, find_conditional(query_data), strlen(query_data));
    return p;
}

//TODO: MAKE DOUBLES PRINT "%.2f"
void print_data(void* v){
    if(v == NULL) return;
    int* err = calloc(1, sizeof(int));
    int n = find_conditional((char*)v);
    *err =1;
    //if(((char*)v)[n] > '0' && ((char*)v)[n] < '9')
    if(n>0)
        rd_parse_number(v, n, strlen(v), err);
    if(*err == 0){
        char* p = calloc(n+1, sizeof(char));
        strncpy(p, v, n);
        p[n] = '\0';
        printf("%s", p);
        printf("%.2f\n", rd_parse_number(v, n, strlen(v), err));
        free(p);
    }
    else if (*err == 1){
        if (*((int*)v) == 1) printf("AND\n");
        else if(*((int*)v) == 0) printf("OR\n");
        else printf("%s\n", (char *)v);
    }
    free(err);
}

int num_cons(char* text){
    int count = 0;
    for(int i = 0; i < strlen(text); i++){
        if(*(text+i) == '&')
            if(*(text+i+1) == '&') count++;
        if(*(text+i) == '|')
            if(*(text+i+1) == '|') count++;
    }
    return count;
}

int find_space(char* p, int start){
    for(int i = start; i < strlen(p); i++){
        if( p[i] == ' ')
            if( p[i+1] != ' ')
                return i;
    }
    return 0;
}

// returns index of the connective
int find_conn(char* p, int n){
    int index = 0, cc = 0;
    for(int i = 0; i < strlen(p); i++){
        if(cc == n) return index;
        if(p[i] == '&' && p[i+1] == '&'){
            index = i;
            cc++;
        }
        if(p[i] == '|' && p[i+1] == '|'){
            index = i;
            cc++;
        }
        
    }
    return 0;
}



Tree* build_tree(char* query){
    static int space = -1;
    space++;
    int conc = num_cons(query);
    int loc_conn = 0;
    
    loc_conn = find_conn(query, conc/2+1);
    Tree* t = t_make();
    
    if(conc == 0) {
        char* hold = str_at(query, 0);
        t_set_data(t, hold);
        
    }
    if(query[loc_conn] == '&'){
        int* p = calloc(1,sizeof(int));
        *p = 1;
        t_set_data(t, p);
    }
    if(query[loc_conn] == '|'){
        int* o = calloc(1, sizeof(int));
        *o = 0;
        t_set_data(t, o);
    }
    if(conc != 0){
        
        
        char left[loc_conn];
        strncpy(left, query, loc_conn);
        left[loc_conn] = '\0';
        
        
        char* right = calloc(strlen(query)-loc_conn,sizeof(char));
        strcpy(right, query+loc_conn+2);
        
        t_set_left(t, build_tree(left));
        t_set_right(t, build_tree(right));
        
        
        
        
        free(right);
    }
    return t;
}

char* str_at(char* p, int n){
    int end = 0, in = 0;
    for(int i = n; i < strlen(p); i++){
        if(in == 0 && p[i] != ' ') n = i;
        if(p[i] != ' '){
            in = 1;
        }
        
        if(p[i] == ' ' && in == 1){
            end = i;
            break;
        }
        end = (i+1);
        if(p[i] == '\0') {
            
            break;
        }
        
    }
    char* ret = calloc((end-n+1),sizeof(char));
    strncpy(ret, p+n, end-n);
    ret[end-n] = '\0';
    return ret;
}

int grammar_checker(char* text){
    if(text == NULL) return 0;
    if(strlen(text) < 3) return 0;
    if(find_conditional(text) == -1) return 0;
    int in = 0, con = 0, in2 = 0, cc = 0, waiter = 0, conn_find = 0;
    if(text[0] == '&' || text[0] == '|' || text[0] == '>' ||
       text[0] == '<' || text[0] == '=' ) return 0;
    
    for(int i = 0; i < strlen(text)+1; i++){
        
        //bad cases
        if((text[i] == '<' || text[i] == '>' || text[i] == '=') && in == 0) return 0;
        if(((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z')) || ((text[i] >= '0' && text[i] <= '9')))
            if(in == 1 && con > 0)
                in2 = 1;
        //check column name
        if((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z')){
            if(in == 0 && con == 0 && in2 == 0){
                in = 1;
                cc = 0;
            }
            if(waiter == 1) return 0;
        }
        
        //check <<, >>, <>, ><, <&, <|, >&, >|
        if((text[i] == '<' || text[i] == '>') && (text[i+1] == '<' || text[i+1] == '>' || text[i+1] == '&' || text[i+1] == '|')) return 0;
        //checks for ==, =>, =<, =&, =|
        if(text[i] == '=' && (text[i+1] == '=' || text[i+1] == '<' || text[i+1] == '>' || text[i+1] == '&' || text[i+1] == '|')) return 0;
        //check for valid connectives
        if(text[i] == '=' || text[i] == '<' || text[i] == '>') con++;
        if(con > 2) return 0;
        //************* uhh rewrite this
        /* if(text[i] != ' ' && text[i] != '|' && text[i] != '&' && text[i] == '<'
         && text[i] == '>' && text[i] == '='){
         if(con > 0 && in == 1)
         in2 = 1;
         }*/
        //check for valid connectives
        if((text[i] == '&' && text[i+1] == '&') || (text[i] == '|' && text[i+1] == '|')) {cc++;
            
            conn_find = 1;
        }
        if((text[i] == '&' && text[i+1] != '&') || (text[i] == '|' && text[i+1] != '|')){
            waiter = 0;
            cc++;
            
            if(cc == 2) continue;
            
            return 0;
        }
        
        if((text[i] == ' ' || text[i] == '\0') && in == 1 && con > 0 && in2 == 1){
            in = 0;
            con = 0;
            in2 = 0;
            cc = 0;
            waiter = 1;
            conn_find = 0;
            continue;
        }
        else if ((text[i] == '\0' || text[i] == ' ') && (in == 1 || in2 == 1 || con > 0)){
            return 0;
        }
        
        if (text[i] == '&' && in == 1 && in2 == 1 && con > 0){
            in = 0;
            in2 = 0;
            con = 0;
            waiter = 0;
        }
        
        if (text[i] == '|' && in == 1 && in2 == 1 && con > 0){
            in = 0;
            in2 = 0;
            con = 0;
            waiter = 0;
            
        }
        
        if(text[i] == '\0' && conn_find == 1) {return 0;
            
        }
        
    }
    
    return 1;
    
    
}
//returns index after conditional
int find_conditional(char* text){
    for(int i = 0; i < strlen(text); i++){
        if(text[i] == '=') return i+1;
        if(text[i] == '<' && text[i+1] != '=') return i+1;
        if(text[i] == '>' && text[i+1] != '=') return i+1;
    }
    return -1;
    
}

int find_column_name(char* query_data){
    for(int i = 0; i < strlen(query_data)+1; i++){
        if(query_data[i] == '<' || query_data[i] == '>' || query_data[i] == '=')
            return i;
    }
    return -1;
}

char* column_name(char* query_data){
    char* p = calloc(find_column_name(query_data)+1, sizeof(char));
    strncpy(p, query_data, find_column_name(query_data));
    p[find_column_name(query_data)] = '\0';
    return p;
}

void testing(){
    for(int i = 0; i < cc; i++){
        printf("%s\n", columns[i]);
    }
}
