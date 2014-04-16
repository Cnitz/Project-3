#include "defs_itf.h"

FILE * fs;

/*
 * Opens a file identified by NULL-terminated string fn for reading.  Has
 * undefined behavior if called more than once or if the file can not be
 * opened.
 */
void   rd_open(char* fn){
   fs = fopen(fn, "r");
}

void rd_close(){
    fclose(fs);
}

/*
 * Returns the next character in the input file or EOF. Has undefined
 * behavior if the file is not open.
 */
int    rd_getchar(){
    if(fs != NULL)
    return fgetc(fs);
    return -1;
}

/*
 * Parse the character array buf (from "from" index to "to" index, inclusive) 
 * to extract a string discarding leading and trailing spaces and well-formed 
 * double quotes. The format is:
 *
 *   <space>*  '"' <not-quote> '"' <space>*  |
 *   <space>* <not-space-quote-comma> ( <not-quote-comma>* <not-space-quote-comma> )? <space>*
 *
 * where <space> is a space character, <not-quote> is any ASCII character
 * other a double quote, <not-quote-comma> is any ASCII character other the 
 * double quote or comma, <not-space-quote-comma> excludes spaces,
 * commas, and double quotes. A \C{*} means zero or more occurences, \C{?}
 * means zero or one.  If the string does not conform to this format return
 * NULL. The string returned is a newly allocated that is zero terminated.
 */
char*  rd_parse_string(char buf[], int from, int to){
    int qio = 0, sc = 0, in = 0, fd = -1, ld = -1;
    char *p = calloc((to+2), sizeof(char));
    if(strlen(buf) < 1) return NULL;
    if(buf == NULL) return NULL;
    if(from-to == 0) return NULL;
    
    for(int i = from; i <= to-1; i++){
        if(qio > 2) return NULL;
        if(buf[i] == ',' && buf[i-1] != ' ') {ld = i+1; continue;}
        
        if(buf[i] == ' '){
            if(in == 0)
            if (fd > -1 && ld == -1){
                ld = i-1;
                
            }
            sc++;
            continue;
        }
       
        
        
        
        
        else if(in == 0){
        if(buf[i] == '"'){
            in = 1;
            qio++;
            
            fd = i+1;
            continue;
        }
        
        
        else{
            if(qio == 2) return NULL;
            if(fd == -1)
            fd = i;
            in = 1;
        }
       
           }
        
        else if(in == 1){
            if(buf[i] == '"'){
                if(qio == 0) return NULL;
                in = 0;
                qio++;
                ld = i;
                if(buf[i+1] == '\0') break;
            }
            
            
            
            else{
                if(buf[i] == '\0') break;
                ld = i+1;
                continue;
            }
            }
        
    }
    if(to == from+1)
        if(buf[from] != ' ' && buf[from] != '"')
            return strncpy(p, buf+from, 1);
    if(qio == 1) return NULL;
    if(sc == to-from+1) return NULL;
    if(strlen(buf) == 1) return buf;
    if(fd > ld) {
        p = "";
        return p;
    }
    
    return strcat(strncpy(p, buf+fd, (ld-fd)), "");
    }

/*
 * Parse the character array buf (from "from" index to "to" index, inclusive) 
 * to extract a double discarding leading and trailing spaces. 
 * The format for a number is:
 *  <space>* ('+' | '-')?  (<digit>* '.')? <digit>+ <space>*
 * where <space> is a space character, <digit> is a single digit.  If the
 * buf does not contain a number, err is set to 1 and the return value is
 * undefined. Otherwise, the value of the number is returned and err is set
 * to 0.
 */
double rd_parse_number(char buf[], int from, int to, int* err){
    int in = 0, pos = 0, neg = 0, dc = 0, num = 0, fd = -1, ld = -1;
    double ret = 0.0;
    *err = 0;
    char *p = malloc(sizeof(char)*(to-from));
    
    for(int i = from; i <= to; i++){
        if(fd > -1){
            strncpy(p, buf+fd, strlen(buf)-fd);
            ret = atof(p);
        }
        if(ld > -1){
            strncpy(p, buf+fd, (ld-fd));
            ret = atof(p);
        }
        
        if(buf[i] == '.' && fd == -1){
            fd = i;
            dc++;
            in = 1;
            continue;
        }
       if(buf[i] == '.' && fd > -1 ){
           
           if(ld > -1 && in == 0)
               *err = 1;
           if (ld < 0 && dc == 0)
               continue;
           
       }
        
       if (buf[i-1] == '.' && (buf[i] == '.' || buf[i] == ' ' || buf[i] == '\0')) continue;
       else if(buf[i-1] == '.' && (buf[i] < '0' || buf[i] > '9')){
          
            *err = 1;
            
        }
        else if(buf[i-1] == '.' && (buf[i] >= '0' && buf[i] <= '9'))
            continue;
        
        
         if((buf[i] == ' ' || buf[i] == '\0') && fd > -1){
             if(ld == -1){
            ld = i-1;
            in = 0;
             }
             else
                 continue;
           
            
        }
        else if(buf[i] == ' ') continue;
        else if(buf[i] == '\t' || buf[i] == '\n'){
            *err = 1;
            continue;
        }
        else if(fd == -1 && (buf[i] == '+' || buf[i] == '-')) {
            fd = i;
            in = 1;
            if(buf[i] == '+')
                pos = 1;
            if (buf[i] == '-')
                neg = 1;
        }
        else if(fd > -1 && (buf[i] == '+' || buf[i] == '-'))
            *err = 1;
        else if ((buf[i] >= '0' && buf[i] <= '9') && (fd == -1)){
                fd = i;
                in = 1;
        }
        else if (buf[i] >= '0' && buf[i] <= '9' && fd > -1){
            continue;
        }
        else if (buf[i] >= '0' && buf[i] <= '9' && ld > -1) {
                *err = 1;
            continue;
        }
        
        
        else if(buf[i] > '9' || buf[i] < '0') {
            *err = 1;
            
                        return ret;
        }
    }
    
    if(ld > -1){
                if(ld == fd)
           strncpy(p, buf+fd, (fd));
        else
            strncpy(p, buf+fd, (ld-fd));
        ret = atof(p);
    }
    
    if(pos == 1){
        
        
        strncpy(p, buf+fd+1, (ld-fd));
        
        return atof(p);
    }
    
    
    return ret;
    
    
    
}

/*
 * Return the number of comma separated values in the array buf.  end has
 * the length of the buffer. Returns -1 if it cannot calculate due to
 * odd number of double quotes.
 */
int rd_num_fields(char buf[], int end ){
    int qc = 0, cc = 0, qio = 0;
    if(strlen(buf) == 0)
        return 0;
    for(int i = 0; i <= end; i++){
        if(buf[i] == '"'){
            qc++;
            if(qio == 1)
                qio = 0;
            else if(qio == 0)
                qio = 1;
            
        }
        if(buf[i] == ','){
            if(qio == 0){
                cc++;
            }
            
        }
    }
    if(qc == 0 || qc % 2 == 0){
        if(buf[end-1] == ',')
            return cc;
        else
            return cc+1;
    }
    return -1;
}

/*
 * Scan a sequence of characters from cur to end in buf[] and return the
 * position in the array of the first comma encountered. Return -1 if there 
 * is an uneven number of double quotes.
 */
int    rd_field_length(char buf[], int cur, int end ){
    int qc = 0, cc = 0, qio = 0, i = 0, ret = 0, j = 0;
    if(strlen(buf) == 0)
        return 0;
    for(j = cur; j <= end; j++){
        if(buf[j] == '"'){
            qc++;
            
        }
    }
    for(j = cur; j <= end; j++){
        if(buf[j] == '"') qc++;
        if(qc % 2 == 1) continue;
        if(buf[j] == ',') cc++;
    }
    
    
    if(qc % 2 == 0)
    for(i = cur; i <= end; i++){
        
        if(buf[i] == '"') qc++;
        if(qc % 2 == 1) continue;
        if(buf[i] == ','){
                return i;
        }
        if(buf[i] == '\0')
            return strlen(buf);
            

        
    
    
}
    return -1;
}
