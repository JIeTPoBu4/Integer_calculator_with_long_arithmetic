#include "my_stack.h"

int stack_init(my_stack *stack) {
  stack->head = NULL;
  stack->size = 0;

  return 0;
}

int stack_push(my_stack *stack, const char *string) {
  node *temp = NULL;

  if(!(temp = (node*)malloc(sizeof(node)))) {
    return -1;
  }

  if(!(temp->data = (char*)malloc(sizeof(char) * (strlen(string) + 1)))) {
    free(temp);

    return -1;
  }

  snprintf(temp->data, strlen(string) + 1, "%s", string);
  temp->next = stack->head;
  stack->head = temp;
  (stack->size)++;

  return 0;
}

int stack_pop(my_stack *stack, char **string) {
  if (!(stack->head)) {
    string = NULL;

    return -1;
  }

  if (!(*string = (char *)malloc(sizeof(char) * (strlen(stack->head->data) + 1)))) {
    return 1;
  }

  snprintf(*string, strlen(stack->head->data) + 1, "%s", stack->head->data);
  free(stack->head->data);
  node *temp = stack->head;
  stack->head = stack->head->next;

  (stack->size)--;
  free(temp);
  
  return 0;
}

int stack_clear(my_stack *stack) {
  while (stack->head) {
    free(stack->head->data);
    node *temp = stack->head->next;
    free(stack->head);
    stack->head = temp;
  }

  stack->size = 0;

  return 0;
}

int stack_getsize(const my_stack *stack) { return stack->size; }

int stack_get_last(my_stack *stack, char **buf) {
  if(!(stack->head->data)) {
    return -1;
  }

  if(!(*buf = (char*)malloc(sizeof(char) * (strlen(stack->head->data) + 1)))) {
    return -1;
  }

  snprintf(*buf, strlen(stack->head->data) + 1, "%s", stack->head->data);

  return 0;
}

int stack_delete_last(my_stack *stack) {
   if (!(stack->head)) {
    return -1;
  }

  free(stack->head->data);
  node *temp = stack->head;
  stack->head = stack->head->next;

  (stack->size)--;
  free(temp);
  
  return 0;
}