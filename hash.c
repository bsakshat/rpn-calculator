#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "hash.h"

/* (IMPLEMENT THIS FUNCTION)
 * In this function, you will create a new Symtab struct.
 * Follow the Directions in the Project Documentation for this Function
 * Return the pointer to the new symtab.
 * On any memory errors, return NULL
 */
Symtab *hash_initialize() {
  /* Implement this function */
  int i = 0;
  //initialize a new table and a pointer to symtab
  Symbol **tab = malloc(sizeof(Symbol *) * HASH_TABLE_INITIAL);
  Symtab *new = malloc(sizeof(Symtab));
  if (new == NULL || tab == NULL){
  	return NULL;
  }
  //assigns all the indexes to NULL
  for (i = 0; i < HASH_TABLE_INITIAL; i++){
  	tab[i] = NULL;
  }
  //assign the new table inside of symtab and define the size and capacity
  new->size = 0;
  new->capacity = HASH_TABLE_INITIAL;
  new->table = tab;
  return new;
}

/* (IMPLEMENT THIS FUNCTION)
 * Destroy your Symbol Table.
 * Follow the Directions in the Project Documentation for this Function
 * Return on any memory errors.
 */
void hash_destroy(Symtab *symtab) {
  /* Implement this function */
  int i = 0;
  if(symtab == NULL){
  	return;
  }
  if(symtab->table == NULL){
  	free(symtab);
  	return;
  }
  //loops through all of the table
  for(i = 0; i < symtab->capacity; i++){
  	if(symtab->table[i] != NULL){
  		Symbol *walker = (symtab->table)[i];
  		Symbol *reaper = NULL;
  		while(walker != NULL){
  			reaper = walker;
  			walker = walker->next;
  			//frees the symbols in the table
  			free(reaper);
  			reaper = NULL;
  		}
  	}
  }
  //frees the table inside of symtab
  free(symtab->table);
  (symtab->table) = NULL;
  //frees the symtab
  free(symtab);
  symtab = NULL;
  return;
}

/* (IMPLEMENT THIS FUNCTION)
 * Return the capacity of the table inside of symtab.
 * If symtab is NULL, return -1;
 */
int hash_get_capacity(Symtab *symtab) {
  /* Implement this function */
  if(symtab == NULL || symtab->table == NULL){
  	return -1;
  }
  //returns the capacity from symtab
  return symtab->capacity;
}

/* (IMPLEMENT THIS FUNCTION)
 * Return the number of used indexes in the table (size) inside of symtab.
 * If symtab is NULL, return -1;
 */
int hash_get_size(Symtab *symtab) {
  /* Implement this function */
  if(symtab == NULL || symtab->table == NULL){
  	return -1;
  }
  //returns the size from symtab
  return symtab->size;
}

/* (IMPLEMENT THIS FUNCTION)
 * Adds a new Symbol to the symtab via Hashing.
 * Follow the Directions in the Project Documentation for this Function
 * If symtab is NULL, there are any malloc errors, or if any rehash fails, return -1;
 * Otherwise, return 0;
 */
int hash_put(Symtab *symtab, char *var, int val) {
  /* Implement this function */
  if(symtab == NULL|| symtab->table == NULL){
  	return -1;
  }
  Symbol *walker = NULL;
  //calculate the index of the variable to be put in the table
  int index = (int)(hash_code(var) % (symtab->capacity));
  //checks if the variable is already present in the table
  if(symtab->table[index] != NULL){
  	walker = symtab->table[index];
  	while(walker != NULL){
  		if(walker->variable == var){
  			//if present updates the value in the table
  			walker->val = val;
  			return 0;
  		}
  		walker = walker->next;
  	}
  }
  //calculates the load of the table
  float load = ((float)symtab->size) / symtab->capacity;
  //if load is greater than or equal to 2, doubles the table capacity and rehashes
  if(load >= 2.0){
  	hash_rehash(symtab, (symtab->capacity) * 2);
  	index = (int)(hash_code(var) % (symtab->capacity));
  }
  //creates a symbol from the variable and value  
  Symbol *sym = symbol_create(var, val);
  if(sym == NULL){
  	return -1;
  }
  //if the index in the table is empty, just put the symbol there
  if(symtab->table[index] == NULL){
  	symtab->table[index] = sym;
  	symtab->size++;
  	return 0;
  }
  //if the index is not empty, put the symbol in a linked list in the index
  else{
  	walker = symtab->table[index];
  	while(walker->next != NULL){
  		walker = walker->next;
  	}
  	walker->next = sym;
  	symtab->size++;
  	return 0;
  }
  return -1;
}

/* (IMPLEMENT THIS FUNCTION)
 * Gets the Symbol for a variable in the Hash Table.
 * Follow the Directions in the Project Documentation for this Function
 * On any NULL symtab or memory errors, return NULL
 */
Symbol *hash_get(Symtab *symtab, char *var) {
  /* Implement this function */
  if(symtab == NULL || symtab->table == NULL){
  	return NULL;
  }
  Symbol *walker = NULL;
  //gets the index of the variable
  int index = (int)(hash_code(var) % (symtab->capacity));
  if(symtab->table[index] == NULL){
  	return NULL;
  }
  else{
  	walker = symtab->table[index];
  	while(walker != NULL){
  		//checks for the variable in the index and returns a copy if present
  		if(*(walker->variable) == *var){
  			return symbol_copy(walker);
  		}
  		walker = walker->next;
  	}
  }
  return NULL;
}

/* (IMPLEMENT THIS FUNCTION)
 * Doubles the size of the Array in symtab and rehashes.
 * Follow the Directions in the Project Documentation for this Function
 * If there were any memory errors, set symtab->array to NULL
 * If symtab is NULL, return immediately.
 */
void hash_rehash(Symtab *symtab, int new_capacity) {
  /* Implement this function */
  if(symtab == NULL || symtab->table == NULL){
  	return;
  }
  int i = 0;
  int j = 0;
  Symbol *walker = NULL;
  Symbol *reaper = NULL;
  //initialize a new table of twice the capacity
  Symbol **new_table = malloc(sizeof(Symbol *) * new_capacity);
  if (new_table == NULL){
  	symtab->table = NULL;
  	return;
  }
  //assigns all the indexes to NULL
  for (j = 0; j < new_capacity; j++){
  	new_table[j] = NULL;
  }
  //loops through all of the old table
  for (i = 0; i < symtab->capacity; i++){
  	//checks if the index in the old table is empty
  	if(symtab->table[i] != NULL){
  	  //if not empty, assigns the variable to the new table
  	  reaper = symtab->table[i];  	  
  	  while(reaper != NULL){
  	  	//gets the new index of the variable for the new table
  	  	int index = (int)(hash_code(reaper->variable) % (new_capacity));
  	  	//if the index in the new table is empty, just put the symbol there
  		if(new_table[index] == NULL){
  		  new_table[index] = reaper;
  		}
  		//if the index is not empty, put the symbol in a linked list in the index
  		else{
  		  Symbol *walker_2 = new_table[index];
  		  while(walker_2->next != NULL){
  		    walker_2 = walker_2->next;
  		  }
  		  walker_2->next = reaper;
  		}
  		//moves to a new variable in the same index in the old table
  		walker = reaper->next;
  		reaper->next = NULL; 
  		reaper = walker; 		
  	  }
  	}
  }
  //frees the old table
  free(symtab->table);
  //assigns the new table and the new capacity to the symtab
  symtab->table = new_table;  
  symtab->capacity = new_capacity;
  return;
}

/* Implemented for you.
 * Provided function to print the symbol table */
void hash_print_symtab(Symtab *symtab) {
  if(symtab == NULL) {
    return;
  }
  printf("|-----Symbol Table [%d size/%d cap]\n", symtab->size, symtab->capacity);

  int i = 0;
  Symbol *walker = NULL;

  /* Iterate every index, looking for symbols to print */
  for(i = 0; i < symtab->capacity; i++) {
    walker = symtab->table[i];
    /* For each found linked list, print every symbol therein */
    while(walker != NULL) {
      printf("| %10s: %d \n", walker->variable, walker->val);
      walker = walker->next;
    }
  }
  return;
}

/* This function is written for you.
 * This computes the hash function for a String
 */
long hash_code(char *var) {
  long code = 0;
  int i;
  int size = strlen(var);

  for(i = 0; i < size; i++) {
    code = (code + var[i]);
    if(size == 1 || i < (size - 1)) {
      code *= 128;
    }
  }
  return code;
}
