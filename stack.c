#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "stack.h"

 /* Create a new Stack_head struct on the Heap and return a pointer to it.
 * Follow the Directions in the Project Documentation for this Function
 * On any malloc errors, return NULL
 */
Stack_head *stack_initialize() {
  /* Implement this function */
  //creates a pointer to stack_head
  Stack_head *head = malloc(sizeof(Stack_head));
  if (head == NULL){
    return NULL;
  }
  head->count = 0;
  head->top = NULL;
  return head;
}

/* (IMPLEMENT THIS FUNCTION)
 * Destroy a stack.
 * Follow the Directions in the Project Documentation for this Function
 * If head is NULL, you can just return.
 */
void stack_destroy(Stack_head *head) {
  /* Implement this function */
  Node *walker = NULL;
  Node *walker_1 = NULL;
  if (head == NULL){
    return;
  }
  walker = head->top;
  //frees the stack one by one by going to next tokens in the stack list
  while(walker!=NULL){
    walker_1 = walker;
    walker = walker_1->next;
    token_free(walker_1->tok);
    node_free(walker_1);
    walker_1 = NULL;
  }
  free(head);
  head = NULL;
  return;
}

/* (IMPLEMENT THIS FUNCTION)
 * Push a new Token on to the Stack.
 * Follow the Directions in the Project Documentation for this Function
 * On any malloc errors, return -1.
 * If there are no errors, return 0.
 */
int stack_push(Stack_head *stack, Token *tok) {
  /* Implement this function */
  Node *node = NULL;
  if(stack == NULL){
    return -1;
  }
  node = node_create(tok);
  if (node != NULL){
    //if stack is empty, just inserts the node and assigns count as 1
    if(!stack_is_empty(stack)){
      stack->top = node;
      stack->count = 1;
    }
    //if stack is not empty, inserts the node at the front and increases the count
    else{    
      node->next = stack->top;
      stack->top = node;
      stack->count++;
    }
    return 0;
  }
  return -1;
}

/* (IMPLEMENT THIS FUNCTION)
 * Pop a Token off of the Stack.
 * Follow the Directions in the Project Documentation for this Function
 * If the stack was empty, return NULL.
 */
Token *stack_pop(Stack_head *stack) {
  /* Implement this function */
  Node *node = NULL;
  Token *token = NULL;
  if(stack == NULL || !stack_is_empty(stack)){
    return NULL;
  }
  else {
    //returns the token inside node at the front and decreases the count
    node = stack->top;
    token = node->tok;
    stack->top = stack->top->next;
    stack->count--;
    node_free(node);
    node = NULL;
    return token;
  }
}

/* (IMPLEMENT THIS FUNCTION)
 * Return the token in the stack node on the top of the stack
 * Follow the Directions in the Project Documentation for this Function
 * If the stack is NULL, return NULL.
 * If the stack is empty, return NULL.
 */
Token *stack_peek(Stack_head *stack) {
  /* Implement this function */
  if(stack == NULL || stack_is_empty(stack)){
    return NULL;
  }
  else{
    //returns the token inside the node at the front
    return stack->top->tok;
  }
}

/* (IMPLEMENT THIS FUNCTION)
 * Return the number of nodes in the stack.
 * Follow the Directions in the Project Documentation for this Function
 * If stack is NULL, return -1.
 * Return 0 if the stack is empty.
 */
int stack_is_empty(Stack_head *stack) {
  /* Implement this function */
  if(stack == NULL){
    return -1;
  }
  //if stack is empty, returns 0
  else if(stack->top == NULL){
    return 0;
  }
  //if stack is not empty, returns the count
  else{
    return stack->count;
  }
}

/* These two functions are written for you.
 * It recurses the stack and prints out the tokens in reverse order
 * eg. top->2->4->1->8 will print at Stack: 8 1 4 2
 * eg. stack_push(5) will then print Stack: 8 1 4 2 5
 */

/* This is implemented for you.
 * Recursive print. (Local function)
 * Base Case: node == NULL, return
 * Recursive Case: call print_node(node->next, print_data), then print node.
 */
static void print_node(Node *node) {
  if(node == NULL) {
    return;
  }
  token_print(node->tok);
  print_node(node->next);
  return;
}

/* This is implemented for you.
 * Setup function for the recursive calls.  Starts printing with stack->top
 */
void stack_print(Stack_head *stack) {
  if(stack == NULL) {
    return;
  }
  printf("|-----Program Stack\n");
  printf("| ");
  print_node(stack->top);
  printf("\n");
  return;
}
