#include "treemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode TreeNode;

struct TreeNode {
  Pair *pair;
  TreeNode *left;
  TreeNode *right;
  TreeNode *parent;
};

struct TreeMap {
  TreeNode *root;
  TreeNode *current;
  int (*lower_than)(void *key1, void *key2);
};

int is_equal(TreeMap *tree, void *key1, void *key2) {
  if (tree->lower_than(key1, key2) == 0 && tree->lower_than(key2, key1) == 0)
    return 1;
  else
    return 0;
}

TreeNode *createTreeNode(void *key, void *value) {
  TreeNode *new = (TreeNode *)malloc(sizeof(TreeNode));
  if (new == NULL)
    return NULL;
  new->pair = (Pair *)malloc(sizeof(Pair));
  new->pair->key = key;
  new->pair->value = value;
  new->parent = new->left = new->right = NULL;
  return new;
}

TreeMap *createTreeMap(int (*lower_than)(void *key1, void *key2)) {
  if (lower_than == NULL) return NULL;

  TreeMap* new = (TreeMap *) malloc(sizeof(TreeMap));
  if(new == NULL) return NULL;

  new -> root = NULL;
  new -> current = NULL;
  new->lower_than = lower_than;

  return new;
}

void insertTreeMap(TreeMap *tree, void *key, void *value) {
  if(tree == NULL) return;

  TreeNode* parent = NULL;
  TreeNode* current = tree -> root;
  TreeNode* newNodo = createTreeNode(key, value);
  if(newNodo == NULL) return;


  while (current != NULL){
    parent = current;
    if(is_equal(tree, current -> pair -> key, key)){
      return;
    }
    else{
      if(tree -> lower_than(key, current -> pair -> key)){
        current = current -> left;
      }
      else
      current = current -> right;
    }
  }
  newNodo -> parent = parent;
  if(parent == NULL){
    tree -> root = newNodo;
  }
  else{
    if(tree -> lower_than(key, parent -> pair -> key)){
    parent -> left = newNodo;
    }
    else
      parent -> right = newNodo;
  }
  tree -> current = newNodo;
}


TreeNode *minimum(TreeNode *x) { 
  if(x == NULL) return NULL;

  while(x -> left != NULL){
    x = x -> left;
  }
  return x; 
}

void removeNode(TreeMap *tree, TreeNode *node) {
  if(node -> left == NULL && node -> right == NULL){
    //nodo sin hijos
    if(node -> parent == NULL){
      tree -> root = NULL;
    }
    else{
      if(node -> parent -> left == node){
        node -> parent -> left = NULL;
      }
      else
        node -> parent -> right = NULL;
    }
  }
  //nodo con un hijo (izquierdo)
  else{
    if(node -> left == NULL){
      if(node -> parent == NULL){
        tree -> root = node -> right;
      }
      else{
        if(node -> parent -> left == node){
          node -> parent -> left = node -> right;
        }
        else
          node -> parent -> right = node -> right;
        node -> right -> parent = node -> parent;
      }
    }
   
    //nodo con un hijo (derecha)  
    else{
      if(node -> right == NULL){
        if(node -> parent == NULL){
          tree -> root = node -> left;
        }
        else{
          if(node -> parent -> left == node){
            node -> parent -> left = node -> left;
          }
          else
            node -> parent -> right = node -> left;
          
          node -> left -> parent = node -> parent;
        }
      }
    
  
      //caso con dos hijos
      else{
        TreeNode* min = minimum(node -> right);
    
        node -> pair -> key = min -> pair -> key;
        node -> pair -> value = min -> pair -> value;
    
        removeNode(tree, min);
      }
    }
  }
}

void eraseTreeMap(TreeMap *tree, void *key) {
  if (tree == NULL || tree->root == NULL) return;

  if (searchTreeMap(tree, key) == NULL)
    return;
  TreeNode *node = tree->current;
  removeNode(tree, node);
}

Pair *searchTreeMap(TreeMap *tree, void *key) { 
  if(tree == NULL || tree -> root == NULL) return NULL;

  TreeNode *current = tree -> root;
  
  while (current != NULL){
    if(is_equal(tree, current -> pair -> key, key)){
      tree -> current = current;
      return current -> pair;
    }
    else{
      if(tree -> lower_than(key, current -> pair -> key)){
        current = current -> left;
      }
      else
        current = current -> right;
    }
  }
  return NULL; 
}

Pair *upperBound(TreeMap *tree, void *key) { return NULL; }

Pair *firstTreeMap(TreeMap *tree) { 
  if(tree == NULL || tree -> root == NULL) return NULL;

  TreeNode* min = minimum(tree -> root);

  tree -> current = min;
  
  return min -> pair; 
}

Pair *nextTreeMap(TreeMap *tree) { return NULL; }
