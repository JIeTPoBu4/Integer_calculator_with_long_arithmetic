#ifndef _FUNCTIONS
#define _FUNCTIONS

#include "my_queue.h"
#include "my_stack.h"

int is_digit(char symb);
int is_operator(char symb);
int is_scope(char symb);
int push_number(const char *str, int *index, my_queue *queue);
int push_negativ_number(const char *str, int *index, my_queue *queue);
int push_operator(char op, my_queue *queue);
int new_pars(const char *str, my_queue *queue);
int pole_notation(my_queue *in, my_queue *out);
int queue_to_array(my_queue *queue, char ***array, int *arr_size);
int reverse(char *string);
int convert_to_digit(char symb);
char convert_to_char(int digit);
int calculate(char **pole_note, int arr_size);
int unary_minus(char **first_operand);
int sum(char **first_operand, char **second_operand);
int sub(char **first_operand, char **second_operand);


#endif