#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed

  t->nil = (node_t *) calloc(1, sizeof(node_t));

  t->root = t->nil;
  t->nil->color = RBTREE_BLACK;
  t->nil->left = t->nil;
  t->nil->right = t->nil;
  
  return t;
}

void delete_node(rbtree *t, node_t *p){
  if (p->left != t->nil)
    delete_node(t, p->left);
  if (p->right != t->nil)
    delete_node(t, p->right);
  if (p != t->nil)
    free(p);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_node(t,t->root);
  free(t->nil);
  free(t);
}

node_t *new_node(int key){
  node_t *new = (node_t *) calloc(1, sizeof(node_t));
  new->color = RBTREE_RED;
  new->key = key;
  new->left = new->right = new->parent = NULL;
  
  return new;
}


void left_rotate(rbtree *t, node_t *x){
  node_t *y = x->right;
  x->right = y->left; // move y's left sub tree to x's right sub tree
  if (y->left != t->nil){ // if right node is not nil, make right node's left sub tree's parent to x
    y->left->parent = x;
  }
  y->parent = x->parent; // make right node's parent to x's parent
  if(x->parent == t->nil){ //if x is root node
    t->root = y;
  }
  else if (x == x->parent->left){ // if x is parent's left node
    x->parent->left = y;
  }
  else{ //if x is parent's right node
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *y)
{
  node_t *x = y->left;
  y->left = x->right;
  if(x->right != t->nil){
    x->right->parent = y;
  }
  x->parent = y->parent;
  if(y->parent == t->nil){
    t->root = x;
  }
  else if(y == y->parent->left){
    y->parent->left = x;
  }
  else{
    y->parent->right = x;
  }
  x->right = y;
  y->parent = x;
}

void rbtree_insert_fixup(rbtree *t, node_t *p){
  while(p->parent->color == RBTREE_RED){ // while parent's color is red
    if(p->parent == p->parent->parent->left){ // if parent is grand parent's left node
      node_t *y = p->parent->parent->right;
      if(y->color == RBTREE_RED){ // case 1: when uncle's color is red
        // Make parent's and uncle's color red. And make grand parent's color red.
        p->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        p->parent->parent->color = RBTREE_RED;
        p = p->parent->parent; // Now grand parent's color is red. so need to check grand parent's case.
      }
      else {
        if(p == p->parent->right){ // case 2: uncle's color is black and p is parent's right node.(not straight case)
        // p become parent's left node
          p = p->parent;
          left_rotate(t, p);
        }
        // case3: uncle's color is black and p in parent's left node. (straight case)
        // Parent's color become black, grand parent's color become red. And do right_rotate. 
        p->parent->color = RBTREE_BLACK;
        p->parent->parent->color = RBTREE_RED;
        right_rotate(t,p->parent->parent);
      }
    }
    else{
      node_t *y = p->parent->parent->left;
      if(y->color == RBTREE_RED){ // case 1: when uncle's color is red.
        p->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        p->parent->parent->color = RBTREE_RED;
        p = p->parent->parent;
      }
      else{
        if(p == p->parent->left){ // case 2: uncle's color is black and p is parent's left node. (not straight case)
          p = p->parent;
          right_rotate(t,p);
        }
        // case 3: uncle's color is black and p is parent's right node. (straight case)
        p->parent->color = RBTREE_BLACK;
        p->parent->parent->color = RBTREE_RED;
        left_rotate(t,p->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *new = new_node(key);
  node_t *tmp = t->root;
  node_t *p = t->nil;
  while(tmp != t->nil){
    p = tmp; // when while end, p has new node's parent's position
    if(new->key < tmp->key){
      tmp = tmp->left;
    }
    else{
      tmp = tmp->right;
    }
  }
  new->parent = p;
  if(p == t->nil){
    t->root = new;
  }
  else if(new->key < p->key){
    p->left = new;
  }
  else{
    p->right = new;
  }
  new->left = t->nil;
  new->right = t->nil;
  new->color = RBTREE_RED;
  rbtree_insert_fixup(t, new);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  // if there's key. return key's pointer.
  // if there isn't key. return NULL.
  
  node_t *p = t->root;
  while(p != t->nil && p->key != key){
    if(p->key > key){
      p = p->left;
    }
    else{
      p = p->right;
    }
  }
  if(p->key == key){
    return p;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  // return min value's pointer.
  node_t *p = t->root;
  while(p->left != t->nil){
    p = p->left;
  }
  return p;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  // return max value's pointer.
  node_t *p = t->root;
  while(p->right != t->nil){
    p = p->right;
  }
  return p;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
  
  if(u->parent == t->nil){
    t->root = v;
  }
  else if(u == u->parent->left){
    u->parent->left = v;
  }
  else{
    u->parent->right = v;
  }
  v->parent = u->parent;
}

node_t *rbtree_minimum(rbtree* t, node_t *p){
  node_t *x = p;
  while(x->left != t->nil){
    x = x->left;
  }
  
  return x;
}

void rbtree_delete_fixup(rbtree *t, node_t *x){
  while(x != t->root && x->color == RBTREE_BLACK){
    if(x == x ->parent->left){ // if node x is parent's left node.
      node_t *w = x->parent->right; //w node is x's brother node.
      if(w->color == RBTREE_RED){ // case 1: brother's color is red.
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t,x->parent);
        w = x->parent->right;
      }
      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){ // case 2: brother's color is black and brother's child nodes' color are black.
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else{
        if(w->right->color == RBTREE_BLACK){ // case 3: brother's color is black and brother's left child node's color is red. (non straight case)
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t,w);
          w = x->parent->right;
        }
        w->color = x->parent->color; // case 4: brother's color is black and brother's right childe node's color is red. (straight case)
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t,x->parent);
        x = t->root;
      }
    }
    else{ //if node x is parent's right node.
      node_t *w = x->parent->left; // w node is x's brother node.
      if(w->color == RBTREE_RED){ // case 1: brother's color is red.
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t,x->parent);
        w = x->parent->left;
      }
      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){ // case 2: brother's color is black and brother's child nodes' color are black.
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else{
        if(w->left->color == RBTREE_BLACK){ // case 3: brother's color is black and brother's right child node's color is red.(straight case)
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t,w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t,x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  node_t *x = NULL;
  node_t *y = z;
  int y_original_color = y->color;
  if(z->left == t->nil){ // when z has only right child node
    x = z->right;
    rbtree_transplant(t, z, z->right);
  }
  else if(z->right == t->nil){ // when z has only left child node
    x = z->left;
    rbtree_transplant(t, z, z->left);
  }
  else{ // when z has two child node
    y  = rbtree_minimum(t, z->right);
    y_original_color = y->color;
    x = y->right; 
    if(y->parent == z){
      x->parent = y;
    }
    else{ 
      rbtree_transplant(t,y,y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    rbtree_transplant(t,z,y);
    y->left = z->left; 
    y->left->parent = y;
    y->color = z->color;
  }

  if(y_original_color == RBTREE_BLACK){
    rbtree_delete_fixup(t,x);
  }

  free(z); // free z's memory.
  return 0;
}

void inorder(const rbtree *t, key_t *arr, node_t *p,int *x, int n){
  if(p == t->nil){
    return;
  }
  inorder(t, arr, p->left, x, n);
  arr[*x] = p->key;
  (*x)++;
  inorder(t, arr, p->right, x, n);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if(t->root == t->nil) // if tree is empty, return.
    return 0;
  int i = 0;
  int* x = &i;
  inorder(t, arr, t->root, x, n);
  return 0;
}
