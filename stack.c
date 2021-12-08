/********
 * Project 02 : Maze
 * COSC 208, Introduction to Computer Systems, Fall 2021
 * 
 * Part A: implement the stack_new, stack_push, and stack_pop functions
 * Part B: implement the stack_free function
 ********/

#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Return a pointer to a newly-heap-allocated struct stack. Don't forget to
 * initialize the contents of the struct!
 */
struct stack *stack_new() { // allocate a new stack and set the top to NULL

  struct stack *new = malloc(sizeof(struct stack));
  new->top = NULL;

  return new;
}

/*
 * Create a new heap-allocated struct node to store the square passed as a 
 * parameter. The square has already been allocated on the heap; do NOT create
 * another copy of the square on the heap.
 */
void stack_push(struct stack *stk, struct square *sq) { 
  /* Push a node to the top of the stack which stores a square, 
   * top points to the new node, 
   * and the new node points to what top did before.   
   */
  struct node *new_node = malloc(sizeof(struct node));

  new_node->val = sq; 
  new_node->next = stk->top; // push new node onto the top of the stack between top and the next node
  stk->top = new_node;
}

/*
 * Returns a reference to the square stored on the heap.
 * Deallocate the heap-allocated struct node that is popped from stk. 
 */
struct square *stack_pop(struct stack *stk) { // pop the top node of the stack and have top point to the next node
  if (stk->top == NULL) {
    return NULL;
  }

  struct square *ptr = stk->top->val;
  struct node *temp = stk->top; // temporary node to free the popped node after unlinking it from top
  
  stk->top = stk->top->next; // have top point to the node after the one to be popped
  free(temp);
  return ptr;
}


/*
 * Completely free the memory consumed by a struct stack, including each node 
 * and the struct stack itself. Do NOT deallocate the squares referenced by
 * the nodes on the stack; this will be handled elsewhere.
 */
void stack_free(struct stack *stk) { // use the stack_pop function to free each node starting from the top of the stack      
  while (stk->top != NULL) { // pop the top node as long as the stack is not empty
    stack_pop(stk);
  }
  free(stk);
}

/*
 * Print content of stack passed as a parameter from top to bottom.
 */
void stack_print(struct stack *s) {         
  if (s == NULL) {
      printf("Stack not allocated\n");
      return;
  } else {
    struct node *iter = s->top;

    if (iter == NULL) {
      printf("Empty stack\n");
    }

    while (iter != NULL) {
      struct square *sq = iter->val;
      printf("Square[state=%d,standing=%c,row=%d,col=%d,parent=%p]\n", 
        sq->state, sq->standing, sq->row, sq->col, sq->parent);
      iter = iter->next;
    }
  } 
}