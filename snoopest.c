//
//  snoopest.c
//  
//
//  Created by Christian Nitz on 2/20/14.
//
//

#include "defs_itf.h"

char* handle = NULL;
char* query = NULL;
char* file = NULL;
int desc = 0, sorted = 1;
char buffer[1500];
int* err = 0;
void buffering();
void sorting();
int comp();
int row_checker();
void print_tbl();
char** first_row();
char** colnames;


int main(int argc, char *argv[]){
    
    ca_init(argc, argv);
    file = ca_str_value("file");
   // query = ca_str_value("query");
    rd_open(file);
    colnames = first_row();
    
    //printf("%s\n", query);
    //char c = rd_getchar();
    //printf("%c", c);
 

    Table* tbl = tbl_make();
    buffering(tbl);
   // tbl_print(tbl);
    Tree* t = t_make();
    dt_build(tbl, t);
    int space = 0;
    t_print(t, space, dt_print);
   // printf("%s\n",rd_parse_string("hello/hello",0, 12));
    
    
    //char* p = "SSDSDS";
    //Tree* t = q_parse(query, columns, p/*tbl_type(tbl)*/, tbl_column_count(tbl));
    //Tree* test = t_make();
   /* void* pt = malloc(sizeof(int));
    *((int*)pt) = 1;
    t_set_data(test, pt);
    printf("%d\n", q_get_type(t_data(test)));*/
    //for(int i = 0; i < tbl_row_count(tbl); i++){
    //if(row_checker(tbl_row_at(tbl, i), t, p) == 1)
    //    tbl_print_row(tbl_row_at(tbl, i));
   // }
    //tbl_print(tbl);

   // sorting(tbl);

//    print_tbl(tbl);

   // for(int i = 0; i < 6; i++){
  //      printf("%s\n", columns[i]);
 //   }
    
}

void buffering(Table *tbl){
    int i = 0, c, fields = 0, prev = 0, curr = 0, count = 0;
    char* hold = '\0';
    double dub;
for(int j = 0; j < 1500; j++){
buffer[j] = '\0';
}


err = calloc(1, sizeof(int));
if(ca_defined("file")){

	//rd_open(ca_str_value("file"));

    while((c = rd_getchar()) != EOF){
		i=0;
        while(c != '\n'){
            if(c == EOF) break;
            buffer[i] = c;
		
            c = rd_getchar();
            i++;
			
        }

buffer[i] = '\0';
	

curr = 0;
        count = 0;
	prev = 0;
        fields = rd_num_fields(buffer, i+1);

        tbl_start_row(tbl, fields);
        curr = rd_field_length(buffer, curr, i+1);
	

        rd_parse_number(buffer, 0, curr-1, err);

        if(*err == 1) {

            tbl_add_string_to_row(tbl, rd_parse_string(buffer, 0, curr));

        }
        else{

            tbl_add_double_to_row(tbl, rd_parse_number(buffer, 0, curr, err));
        }


        count++;
        prev = curr+1;

        while(count < fields){

            curr = rd_field_length(buffer, prev, i);
           /*
            printf("%s\n", buffer);
            printf("%d\n", curr);
            printf("%d\n", prev);
             */
            rd_parse_number(buffer, prev, (curr-1), err);
		
            if(*err == 1) {

               // printf("%s\n",rd_parse_string(buffer, prev, curr));
	
                tbl_add_string_to_row(tbl, rd_parse_string(buffer, prev, curr));
            }
            else{


                tbl_add_double_to_row(tbl, rd_parse_number(buffer, prev, curr, err));

            }

             prev = curr+1;
		count++;

        }





       

       
        for(int j = 0; j < 1500; j++){
            buffer[j] = '\0';
        }
        i = 0;
    }
}
    tbl_done_building(tbl);
}


void sorting(Table * tbl){
    
        qsort(tbl_rows(tbl), tbl_row_count(tbl), sizeof(Row*), comp);
    
    
    
}


int comp(const void * a, const void * b){
    
    Row * c = calloc(1, sizeof(Row*));
    Row * d = calloc(1, sizeof(Row*));
   c = *(Row**)a;
   d = *(Row**)b;
    
    if(tbl_row_type_at(c, sorted) == 'D'){
       
        double x = tbl_double_at(d, sorted);
        double y = tbl_double_at(c, sorted);
    if(ca_defined("descending"))
        return y-x;
    return x-y;
    }
   
    if(tbl_row_type_at(c, sorted) == 'S'){
        char* z = tbl_string_at(d, sorted);
        char* p = tbl_string_at(d, sorted);
        if(ca_defined("descending"))
            return p-z;
        return z-p;
    }
return 0;
    }

    
char** first_row(){
    int c, i = 0, from = 0, to = 0;
    char** columns;
    while((c = rd_getchar()) != '\n'){
        buffer[i] = c;
        i++;
    }
    buffer[i+1] = '\0';
    columns = calloc(rd_num_fields(buffer, i), sizeof(char*));
    for(int k = 0;  k < rd_num_fields(buffer, i); k++){
        to = rd_field_length(buffer, from+1, i);
        columns[k] = rd_parse_string(buffer, from, to);
        from = rd_field_length(buffer, from+1, i);
    }
    return columns;
}



int row_checker(Row* r, Tree* t, char* tbl_type){
    int type = 0, index = -1;
    char types;
printf("%d\n", q_get_type(t_data(t)));
    if(q_get_type(t_data(t)) == 0){
        //printf("%d\n", k);
        //k++;
        if(row_checker(r, t_left(t), tbl_type) == 1 || row_checker(r, t_right(t), tbl_type) == 1)
            return 1;
    }
    if(q_get_type(t_data(t)) == 1){
        if(row_checker(r, t_left(t), tbl_type) == 1 && row_checker(r, t_right(t), tbl_type) == 1)
            return 1;
    }
    if((type = q_get_type(t_data(t))) >= 2){
      
        index = q_get_col_index(t_data(t));
printf("here\n");
        types = tbl_type[index];
        
        if(type == 2){
            printf("%c\n", types);
            if(types == 'S'){
                printf("%d\n", index);
                printf("%s\n",tbl_string_at(r, index));

                if(strcmp(q_get_str(t_data(t)), tbl_string_at(r, index)) == 0){

                    return 1;
                }
                return 0;
            }
            if(types == 'D'){
                if( tbl_double_at(r, index) >= (q_get_double(t_data(t))-0.001) && tbl_double_at(r, index) <= (q_get_double(t_data(t))+0.001))
                    return 1;
                return 0;
            }
        }
        if(type == 3){
            if(types == 'S'){
                if(strcmp(tbl_string_at(r, index), q_get_str(t_data(t))) < 0)
                    return 1;
                return 0;
            }
            if(types == 'D'){
                if( tbl_double_at(r, index) < (q_get_double(t_data(t))))
                    return 1;
                return 0;
            }
        }
        if(type == 4){
            if(types == 'S'){
                if(strcmp(tbl_string_at(r, index), q_get_str(t_data(t))) > 0)
                    return 1;
                return 0;
            }
            if(types == 'D'){
                if( tbl_double_at(r, index) > (q_get_double(t_data(t))))
                    return 1;
                return 0;
            }
        }
        if(type == 5){
            if(types == 'S'){
                if(strcmp(tbl_string_at(r, index), q_get_str(t_data(t))) <= 0)
                    return 1;
                return 0;
            }
            if(types == 'D'){
                if( tbl_double_at(r, index) <= (q_get_double(t_data(t))+0.001))
                    return 1;
                return 0;
            }
        }
        if(type == 6){
            if(types == 'S'){
                if(strcmp(tbl_string_at(r, index), q_get_str(t_data(t))) >= 0)
                    return 1;
                return 0;
            }
            if(types == 'D'){
                if( tbl_double_at(r, index) >= (q_get_double(t_data(t))-0.001))
                    return 1;
                return 0;
            }
        }
    }
    return 0;

}














/*void print_tbl(Table * tbl){
    if(handle == NULL && security == NULL){ tbl_print(tbl);
       
        return;
    }
    for(int i = 0; i < tbl_row_count(tbl); i++){
        tbl_row_at(tbl, i);
        for(int k = 0; k < tbl_column_count(tbl); k++){
            if(handle != NULL){
                if(tbl_row_type_at(tbl_row_at(tbl, i), k)=='S')
                if(strcmp(handle, tbl_string_at(tbl_row_at(tbl, i), k)) == 0){
                    if(security == NULL)
                        tbl_print_row(tbl_row_at(tbl, i));
                    else{
                        for(int g = 0; g <  tbl_column_count(tbl); g++){
                            if(tbl_row_type_at(tbl_row_at(tbl, i), k)=='S')
                            if(strcmp(security, tbl_string_at(tbl_row_at(tbl, i), g)) == 0)
                                tbl_print_row(tbl_row_at(tbl, i));
                        }
                        
                    }
                }
            }
        }
    }
}
*/
