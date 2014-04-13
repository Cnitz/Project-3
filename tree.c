#include "defs_itf.h"


/*set the data on the tree node */
void  t_set_data(Tree* t, void* data) {
    if(t == NULL) return;
    t->data = data;
}

/*let l be the left node of tree *t */
void  t_set_left(Tree* t, Tree* l){
    if(t == NULL) return;
    t->left = l;
}

/*let r be the left node of tree *t */
void  t_set_right(Tree* t, Tree* r){
    if(t == NULL) return;
    t->right = r;
}

/* simply return left node of the tree */
Tree* t_left(Tree* t){
    if(t == NULL) return NULL;
    return t->left;
}

/* simply return right node of the tree */
Tree* t_right(Tree* t){
    if(t == NULL) return NULL;
    return t->right;
}

/* simply return the data of the tree */
void* t_data(Tree* t){
    if(t == NULL) return NULL;
    return t->data;
}

/* make the node of the tree and allocate space for it*/
Tree* t_make(){
    Tree* t = (Tree *)calloc(1, sizeof(Tree));
    return t;
}

/*
 
 print the whole tree in the following format
 
 Root is printed with zero trailing spaces to the left
 Every node/subtree is printed with one additional space
 Below is an example for a tree with depth 2:
 
 Root
 <space>Left-Child
 <space><space>Left-Left-Child
 <space><space>Left-Right-Child
 <space>Right-Child
 .... and so on ...
 
 
 Use (void(* p))(function pointer) to print.
 
 */
void  t_print( Tree* t ,int space, void(* p)(void*) ){
    if(t == NULL) return;
    for(int i = 0; i < space; i++){
        printf(" ");
    }
    (*p)(t->data);
    ++space;
    t_print(t->left, space, (* p));
    t_print(t->right, space, (* p));
    
    
}

/*
 find a node using compare pointer (*p(d,t->data)) to navigate
 d is the data to be found
 do f at the node. return d if found, otherwise return NULL
 
 f is a function pointer which returns a void pointer.
 Hint: You could use it to print the node or perform other functions.
 Note: f is more useful for future projects. You may not need f for this lab.
 
 */

void* t_navigate( Tree* t , void* d,  int(*p)(void*, void*), void*(*f)(void*)){
    if(t == NULL) return NULL;
    int hold = (*p)(d, t->data);
    if(hold == 0){
        return (*f)(t->data);
    }
    else if(hold < 0)
        t_navigate(t->left, d, (*p), (*f));
    else if(hold > 0)
        t_navigate(t->right, d, (*p), (*f));
    return NULL;
}


