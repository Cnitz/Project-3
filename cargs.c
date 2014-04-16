#include "defs_itf.h"




#define YES 1
#define NO 0
#define FIRST_CHAR 0
int ca_location();

char end = '\0', eql[2] = {'='};
char *eq = eql;

/**
 * Definitions of Global Variables
 * DO NOT MODIFY THE VARIABLE NAMES
 * UPDATE THE VALUES CORRECTLY IN YOUR FUNCTIONS
 */
void test();
void addkey();
void addval();
void editval();
char *findkey();
char *findkeykv();
char *keyholder;
void initkv();
//the number of args in the table
int pos;

//the key-value table, both should store NULL-terminated strings
char **keys;
char **values;





/**
 *  Check if the null-terminated string key is defined. Answer 1 if either
 * "-key" or "-key=val" were passed at the command line, 0 otherwise.
 */
int ca_defined(char *p) {
	

	for(int i = 0; i < pos; i++){
        if(strcmp(p, keys[i]) == 0) return 1;
        else
            continue;
    }
    return 0;
}

/**
 * If "key" is associated to "val", returns "val". 
 * Undefined behavior, if "key" is not found.
 * You may need the helper function a2p.
 */
char *ca_str_value(char *p) {
    if(ca_defined(p) == 1){
        int hold = ca_location(p);
        return values[hold];
    }
    return 0;
	
   
}

/**
 * If "key" is associated to "val", returns atoi(val). If "val" is NULL,
 * returns -1.  Undefined behavior, if "key" is not found.
 */
int ca_int_value(char *p) {
    if(ca_defined(p) == 1){
        if(values[ca_location(p)] == NULL)
            return -1;
        return atoi(values[ca_location(p)]);
    }
    return -1;
}

/**
 * Parse string with length len and add it to the key-values table.
 * The string should be of format "-key=value" or "-key" where key and value are
 * non-empty. If the format does not match, do nothing.
 *
 * Note: update pos correctly. It is the place of key-value table.
 */
void process_string(int len, char string[]) {
    char *p = string;
    initkv(pos+1);
    if(p[1] == '=') return;
    
    
    if(len >= 2) {
    if(p[0] == '-'){
        
        if(strchr(p, '=') == NULL){
        if(strlen(findkey(p)) == (strlen(p)-1)){
            if(ca_defined(findkey(p)) == 0){
            addkey(p+1);
            addval(end);
            pos++;
            return;
            }
            if(ca_defined(findkey(p)) != 0){
                editval(end, ca_location(findkey(p)));
                return;
                
            }
        }
        }
       
        if(strcmp(strchr(p, '='), eq) == 0){
            return;
        }
        
        if(strlen(strchr(p, '=')) > 1){
           
            if(ca_defined(findkeykv(p)) == 0){
            addkey(findkeykv(p));
            addval(strchr(p, '=')+1);
            pos++;
                
                return;
            }
            if(ca_defined(findkeykv(p)) != 0){
                editval((strchr(p, '=')+1), ca_location(findkeykv(p)));
                
                return;
            }
        }
        
        
    }
    }

    
        return;
    }




int ca_location(char key[]){
	for(int i = 0; i < pos; i++){
        if(strcmp(key, keys[i]) == 0) return i;
        else
            continue;
    }
    return -1;
	
}

void addkey(char key[]){
    char *p = (char *) calloc(strlen(key)+1, sizeof(char));
    p = key;
    keys[pos] = p;
    
}



void addval(char key[]){
    char *p = (char *) calloc(strlen(key)+1, sizeof(char));
    p = key;
    values[pos] = p;
}

void editval(char key[], int loc){
    char *p = (char *) calloc(strlen(key)+1, sizeof(char));
    p = key;
    values[loc] = p;
    
}

char *findkey(char key[]){
    char *p = key;
    if(p[0] == '-'){
            return (p+1);
    }
    return 0;
}

char *findkeykv(char key[]){
    char *p = key;
    char *k;
    k = (char *) malloc(sizeof(char)*strlen(key));
    if(p[0] == '-'){
        for(int i = 0; i < strlen(key); i++){
            if(*(p+i) == '='){
                strdup(strncpy(k, (p+1), (i-1)));
                return k;
            }
        }
    }
    return 0;
}

void initkv(int arg){
    keys = realloc(keys, sizeof(char *)*arg);
    values = realloc(values, sizeof(char *)*arg);
    return;
}

void test(){
    for(int i = 0; i < pos; i++){
        printf("key: %s?\n", keys[i]);
        printf("val: %s?\n", values[i]);
    }
    printf("pos: %d\n", pos);
}


/**
 * Read and parse command line arguments. argv is an array of argc strings.
 * The format is "-key" or "-key=val" where "key" and "val" are sequences of
 * alpha-numeric characters. All other string are silently discarded.  For
 * any key, only its last definition is retained.  "-key" has a NULL value.
 */
/* NO NEED TO MODIFY. */
void ca_init(int argc, char **argv) {
	if (argc == 0 || argv == NULL) return;
	for (int i = 1; i < argc; i++)
		process_string(strlen(argv[i]), argv[i]);
}

void ca_free(){}
