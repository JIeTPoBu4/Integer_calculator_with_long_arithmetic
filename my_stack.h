#ifndef _MY_STACK
#define _MY_STACK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"

typedef struct my_stack {
  node *head;
  int size;
} my_stack;

int stack_init(my_stack *stack);
int stack_push(my_stack *stack, const char *string);
int stack_pop(my_stack *stack, char **string);
int stack_clear(my_stack *stack);
int stack_getsize(const my_stack *stack);
int stack_get_last(my_stack *stack, char **buf);
int stack_delete_last(my_stack *stack);

#endif