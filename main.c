//
//  main.c
//  
//
//  Created by Christian Nitz on 4/12/14.
//
//

#include "defs_itf.h"

Table* build_string_left_table();
Table* build_string_right_table();
Table* build_double_left_table();
Table* build_double_right_table();

int main(int argc, char* argv[]){
    char* p1 = "rainy";
    char* p2 = "overcast";
    char* p3 = "sunny";

    char* t1 = "cool";
    char* t2 = "mild";
    char* t3 = "hot";
    
    char* h1 = "normal";
    char* h2 = "high";
    
    char* w1 = "true";
    char* w2 = "false";
    
   
    
    Table* tbl = tbl_make();
    tbl_start_row(tbl, 5);
    tbl_add_string_to_row(tbl, p1);
    tbl_add_string_to_row(tbl, t1);
    tbl_add_string_to_row(tbl, h1);
    tbl_add_string_to_row(tbl, w2);
    tbl_add_double_to_row(tbl, 1.0);
    tbl_start_row(tbl, 5);
    tbl_add_string_to_row(tbl, p1);
    tbl_add_string_to_row(tbl, t1);
    tbl_add_string_to_row(tbl, h1);
    tbl_add_string_to_row(tbl, w1);
    tbl_add_double_to_row(tbl, 0.0);
    tbl_start_row(tbl, 5);
    tbl_add_string_to_row(tbl, p2);
    tbl_add_string_to_row(tbl, t3);
    tbl_add_string_to_row(tbl, h2);
    tbl_add_string_to_row(tbl, w2);
    tbl_add_double_to_row(tbl, 1.0);
    tbl_start_row(tbl, 5);
    tbl_add_string_to_row(tbl, p3);
    tbl_add_string_to_row(tbl, t2);
    tbl_add_string_to_row(tbl, h2);
    tbl_add_string_to_row(tbl, w2);
    tbl_add_double_to_row(tbl, 0.0);
    tbl_start_row(tbl, 5);
    tbl_add_string_to_row(tbl, p1);
    tbl_add_string_to_row(tbl, t1);
    tbl_add_string_to_row(tbl, h1);
    tbl_add_string_to_row(tbl, w2);
    tbl_add_double_to_row(tbl, 1.0);
    tbl_start_row(tbl, 5);
    tbl_add_string_to_row(tbl, p3);
    tbl_add_string_to_row(tbl, t1);
    tbl_add_string_to_row(tbl, h1);
    tbl_add_string_to_row(tbl, w2);
    tbl_add_double_to_row(tbl, 1.0);
    tbl_start_row(tbl, 5);
    tbl_add_string_to_row(tbl, p1);
    tbl_add_string_to_row(tbl, t1);
    tbl_add_string_to_row(tbl, h1);
    tbl_add_string_to_row(tbl, w2);
    tbl_add_double_to_row(tbl, 1.0);
    tbl_start_row(tbl, 5);
    tbl_add_string_to_row(tbl, p3);
    tbl_add_string_to_row(tbl, t3);
    tbl_add_string_to_row(tbl, h1);
    tbl_add_string_to_row(tbl, w2);
    tbl_add_double_to_row(tbl, 1.0);
    tbl_start_row(tbl, 5);
    tbl_add_string_to_row(tbl, p2);
    tbl_add_string_to_row(tbl, t2);
    tbl_add_string_to_row(tbl, h2);
    tbl_add_string_to_row(tbl, w1);
    tbl_add_double_to_row(tbl, 1.0);
    tbl_start_row(tbl, 5);
    tbl_add_string_to_row(tbl, p3);
    tbl_add_string_to_row(tbl, t2);
    tbl_add_string_to_row(tbl, h2);
    tbl_add_string_to_row(tbl, w1);
    tbl_add_double_to_row(tbl, 0.0);
    tbl_start_row(tbl, 5);
    tbl_add_string_to_row(tbl, p2);
    tbl_add_string_to_row(tbl, t3);
    tbl_add_string_to_row(tbl, h1);
    tbl_add_string_to_row(tbl, w2);
    tbl_add_double_to_row(tbl, 0.0);
    tbl_done_building(tbl);
    //tbl_print(tbl);
    char* split = "mild";
    
   // Table* ret = build_string_left_table(tbl, split, 1);
   // tbl_print(ret);






    Tree* t = t_make();
    
    dt_build(tbl, t);
      printf("%s\n", ((Node*)t->data)->field.s);
      printf("%s\n", ((Node*)t->left->data)->field.s);
      printf("%s\n", ((Node*)t->right->data)->field.s);
      printf("%s\n", ((Node*)t->right->left->data)->field.s);
      printf("%s\n", ((Node*)t->right->right->data)->field.s);
}



