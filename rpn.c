#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "token.h"
#include "hash.h"

/* Local Function Declarations */
static int read_file(char *filename, char *line);
static int parse_token(Symtab *symtab, Stack_head *stack, Token *tok);
static int oper_val(Symtab *symtab, Stack_head *stack, Token *tok);
static int assign_val(Symtab *symtab, Stack_head *stack);
static int tok_print(Symtab *symtab, Stack_head *stack);

/* Pre-Written Local Function Declarations */
static void print_header(char *filename, int step);
static void print_step_header(int step);
static void print_step_footer(Symtab *symtab, Stack_head *stack);
static void print_step_output(int val);

/* Defines the largest line that can be read from a file */
#define MAX_LINE_LEN 255

/* This has been written for you.
 * Main function to run your program (written for you).
 * 1) Opens the file using the passed in filename. (your function)
 * -- If the file is not found (ie. fopen returns NULL), then exit(-1);
 * 2) Reads one line from the file.
 * -- The programs will all only be one line in size.
 * -- That line may be up to MAX_LINE_LEN long.
 * 3) Closes the file.
 * 4) Calls token_read_line(line, strlen(line))
 * -- This parses the line and prepares the tokens to be ready to get.
 * 5) While there are tokens remaining to parse: token_has_next() != 0
 * -- a) Get the next token: token_get_next()
 * 6) Parse the token (your function)
 * 7) Print out all of the relevant information
 */
int rpn(Stack_head *stack, Symtab *symtab, char *filename) {
  int step = 0; /* Used to track the program steps */
  int ret = 0;
  char line[MAX_LINE_LEN];
  Token *tok = NULL;

  /* Complete the read_file function that is defined later in this file. */
  ret = read_file(filename, line);
  if(ret != 0) {
    printf("Error: Cannot Read File %s.  Exiting\n", filename);
    exit(-1);
  }

  /* Pass the line into the tokenizer to initialize that system */
  token_read_line(line, strlen(line));

  /* Prints out the nice program output header */
  print_header(filename, step);

  /* Iterate through all tokens */
  while(token_has_next()) {
    /* Begin the next step of execution and print out the step header */
    step++; /* Begin the next step of execution */
    print_step_header(step);

    /* Get the next token */
    tok = token_get_next();
    /* Complete the implementation of this function later in this file. */
    ret = parse_token(symtab, stack, tok);
    if(ret != 0) {
      printf("Critical Error in Parsing.  Exiting Program!\n");
      exit(-1);
    }

    /* Prints out the end of step information */
    print_step_footer(symtab, stack);
  }
  return 0;
}

/* (IMPLEMENT THIS FUNCTION)
 * Local function to open a file or exit.
 * Follow the Directions in the Project Documentation for this Function
 * Open filename, read its contents (up to MAX_LINE_LEN) into line, then
 *   close the file and return 0.
 * On any file error, return -1.
 */
static int read_file(char *filename, char *line) {
  /* Implement This Function */
  FILE *fptr;
  fptr = fopen(filename, "r");
  if (fptr == NULL){
	return -1;
  }	
  fgets(line, MAX_LINE_LEN, fptr);
  printf("%s\n", line);
  fclose(fptr);
  return 0;
}

/* (IMPLEMENT THIS FUNCTION)
 * Parses the Token to implement the rpn calculator features
 * Follow the Directions in the Project Documentation for this Function
 * You may implement this how you like, but many small functions would be good!
 * If the token you are passed in is NULL, return -1.
 * If there are any memory errors, return -1.
 */
static int parse_token(Symtab *symtab, Stack_head *stack, Token *tok) {
  /* Implement This Function */
  int i = 0;
  if(symtab == NULL || stack == NULL || tok == NULL){
  	return -1;
  }
  //if token is a value, push to the stack
  if(tok->type == TYPE_VALUE){
  	i = stack_push(stack, tok);
  }
  //if token is a variable, push to the stack
  else if(tok->type == TYPE_VARIABLE){
  	i = stack_push(stack, tok);
  }
  //if token is a operator, do the necessary operation
  else if(tok->type == TYPE_OPERATOR){
  	i = oper_val(symtab, stack, tok);
  }
  //if token is an assignment, do the assignment
  else if(tok->type == TYPE_ASSIGNMENT){
  	i = assign_val(symtab, stack);
  	free(tok);
  	tok = NULL;
  }
  //if token is a print, print the output
  else if(tok->type == TYPE_PRINT){
  	i = tok_print(symtab, stack);
  	free(tok);
  	tok = NULL;
  }
  else{
  	return -1;
  }
  return i;
}

//this function executes the process for an operator type
static int oper_val(Symtab *symtab, Stack_head *stack, Token *tok){
  //pop two tokens off the stack
  Token *first = stack_pop(stack);
  Token *second = stack_pop(stack);
  if(first == NULL || second == NULL){
  return -1;
  }
  int i = 0;
  int res_val = 0;
  Token *result = NULL;
  //if any of the tokens are variables, get their value from the symtab
  if(first->type == TYPE_VARIABLE){
  	first->value = (hash_get(symtab, first->variable))->val;
  }
  if(second->type == TYPE_VARIABLE){
  	second->value = (hash_get(symtab, second->variable))->val;
  }
  //if operator is '+', perform addition
  if (tok->oper == OPERATOR_PLUS){
  	res_val = first->value + second->value;
  	result = token_create_value(res_val);
  	i = stack_push(stack, result);
  }
  //if operator is '-', perform subtraction
  else if (tok->oper == OPERATOR_MINUS){
  	res_val = second->value - first->value;
  	result = token_create_value(res_val);
  	i = stack_push(stack, result);
  }
  //if operator is '*', perform multiplication
  else if (tok->oper == OPERATOR_MULT){
  	res_val = first->value * second->value;
  	result = token_create_value(res_val);
  	i = stack_push(stack, result);
  }
  //if operator is '/', perform division
  else if (tok->oper == OPERATOR_DIV){
  	res_val = second->value / first->value;
  	result = token_create_value(res_val);
  	i = stack_push(stack, result);
  }
  else{
  	return -1;
  }
  free(first);
  free(second);
  first = NULL;
  second = NULL;
  return i;
}


static int assign_val(Symtab *symtab, Stack_head *stack){
  //pop two tokens off the stack
  Token *first = stack_pop(stack);
  Token *second = stack_pop(stack);
  int i = 0;
  if(first == NULL || second == NULL){
  	return -1;
  }
  //if first token is variable and second is value
  if(first->type == TYPE_VARIABLE && second->type == TYPE_VALUE){
  	i = hash_put(symtab, first->variable, second->value);
  }
  //if first is value and second is variable
  else if(first->type == TYPE_VALUE && second->type == TYPE_VARIABLE){
  	i = hash_put(symtab, second->variable, first->value);
  }
  //if both are variables, assign value of second to first
  else if(first->type == TYPE_VARIABLE && second->type == TYPE_VARIABLE){
  	int v = hash_get(symtab, second->variable)->val;
  	i = hash_put(symtab, first->variable, v);
  }
  else{
  	return -1;
  }
  free(first);
  free(second);
  return i;
}

//this function prints the token value
static int tok_print(Symtab *symtab, Stack_head *stack){
  Token *print_val = stack_pop(stack);
  Symbol *print_v = NULL;
  //if the token is a variable, get its value from symtab
  if(print_val->type == TYPE_VARIABLE){
  	print_v = hash_get(symtab, print_val->variable);
  	if(print_v == NULL){
    	return -1;
  	}
  	print_val->value = print_v->val;
  }
  //call the printing function with the value
  print_step_output(print_val->value);
  return 0;
}

/* This has been written for you.
 * Prints out the main output header
 */
static void print_header(char *filename, int step) {
  printf("######### Beginning Program (%s) ###########\n", filename);
  printf("\n.-------------------\n");
  printf("| Program Step = %2d\n", step);
  token_print_remaining();
  printf("o-------------------\n");
}

/* This has been written for you.
 * Prints out the information at the top of each step
 */
static void print_step_header(int step) {
  printf("\n.-------------------\n");
  printf("| Program Step = %2d\n", step++);
}

/* This has been written for you.
 * Prints out the output value (print token) nicely
 */
static void print_step_output(int val) {
  printf("|-----Program Output\n");
  printf("| %d\n", val);
}

/* This has been written for you.
 * Prints out the information at the bottom of each step
 */
static void print_step_footer(Symtab *symtab, Stack_head *stack) {
  hash_print_symtab(symtab);
  stack_print(stack);
  token_print_remaining();
  printf("o-------------------\n");
}
