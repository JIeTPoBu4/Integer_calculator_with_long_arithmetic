#include "functions.h"
#include "node.h"

#define BUFFER_SIZE 16
#define OP_BUFFER_SIZE 2

int is_digit(char symb) {
	char dig[] = { "0123456789" };

	for(int i = 0; dig[i]; i++) {
		if(dig[i] == symb)
			return 1;
	}

	return 0;
}

int is_operator(char symb) {
	return (symb == '+' || symb == '-' || symb == '*' || symb == '/' || symb == '~');
}

int is_scope(char symb) {
	return (symb == '(' || symb == ')');
}

int is_space(char symb) {
	return (symb == ' ');
}



int push_number(const char *str, int *index, my_queue *queue) {
	int start = *index;
	int end = *index;
	int ptr = *index;

	while(is_digit(str[ptr])) {
		ptr++;
	}

	if(!is_operator(str[ptr]) && !is_scope(str[ptr]) && !is_space(str[ptr]) && str[ptr]) {
		printf("[error]");

		return -1;
	}

	char *buffer = NULL;
	end = ptr;

	if(!(buffer = (char*)malloc(sizeof(char) * (end - start + 1)))) {
		printf("[error]");

		return -1;
	}

	int j = 0;

	for(ptr = *index; ptr < end; ptr++) {
		buffer[j] = str[ptr];
		j++;
	}

	buffer[j] = '\0';
	
	if(queue_push(queue, buffer)) {
		printf("[error]");
		free(buffer);

		return -1;
	}

	free(buffer);
	*index = end - 1;

	return 0;
}

int push_negativ_number(const char *str, int *index, my_queue *queue) {
	int ptr = *index + 1;

	while(str[ptr] == ' ') {
		ptr++;
	}

	int start = ptr;
	int end = ptr;

	while(is_digit(str[ptr])) {
		ptr++;
	}

	if(!is_operator(str[ptr]) && !is_scope(str[ptr]) && str[ptr]) {
		printf("[error]");

		return -1;
	}

	char *buffer = NULL;
	end = ptr;

	if(!(buffer = (char*)malloc(sizeof(char) * (end - start + 2)))) {
		printf("[error]");

		return -1;
	}

	buffer[0] = '-';
	int j = 1;

	for(ptr = *index; ptr < end; ptr++) {
		buffer[j] = str[ptr];
		j++;
	}

	buffer[j] = '\0';
	
	if(queue_push(queue, buffer)) {
		printf("[error]");
		free(buffer);

		return -1;
	}

	free(buffer);
	*index = end - 1;

	return 0;
}

int push_operator(char op, my_queue *queue) {
	char *buffer = NULL;

	if(!(buffer = (char*)malloc(sizeof(char) * OP_BUFFER_SIZE))) {
		printf("[error]");

		return -1;
	}

	buffer[0] = op;
	buffer[1] = '\0';

	if(queue_push(queue, buffer) < 0) {
		free(buffer);

		return -1;
	}

	free(buffer);

	return 0;
}

int new_pars(const char *str, my_queue *queue) {
	int open_scope_num = 0;
	int close_scope_num = 0;

	for(int i = 0; str[i]; i++) {
		if(is_space(str[i])) {
			continue;
		}

		if(is_digit(str[i])) {
			if(push_number(str, &i, queue)) {
				return -1;
			}

			continue;
		}

		if(is_scope(str[i])) {

			switch(str[i]) {
				case '(':
					open_scope_num++;
					break;
				case ')':
					close_scope_num++;
			}

			if(push_operator(str[i], queue) < 0) {
				return -1;
			}

			continue;
		}

		if(is_operator(str[i])) {

			if(str[i] == '-' && !queue_getsize(queue)) {
				if(push_operator('~', queue) < 0) {			//~ is unary minus
					return -1;
				}

				continue;
			}

			char *tmp = NULL;

			if(queue_get_last(queue, &tmp) < 0) {
				return -1;
			}

			if(str[i] == '-' && tmp[strlen(tmp) - 1] == '(') {
				if(push_operator('~', queue) < 0) {
					return -1;
				}

				free(tmp);

				continue;
			}

			if(is_digit(tmp[strlen(tmp) - 1]) || tmp[strlen(tmp) - 1] == ')') {
				if(push_operator(str[i], queue) < 0) {
					free(tmp);

					return -1;
				}

				free(tmp);

				continue;
			}

			if(is_operator(tmp[strlen(tmp) - 1])) {
				printf("[error]");
				free(tmp);

				return -1;
			}

			return -1;
		}

		printf("[error]");

		return -1;
	}

	if(open_scope_num - close_scope_num) {
		printf("[error]");

		return -1;
	}

	return 0;
}


int priority(char first_op, char second_op) {
	if(!is_operator(first_op) || !is_operator(second_op)) {
		return -1;
	}

	if(first_op == '+' || first_op == '-') {
		return 0;
	}
	else{
		switch(second_op) {
			case '+':
		return 1;
			case '-':
		return 1;
			case '*':
		return 0;
			case '/':
		return 0;
		}
	}
}

int calc_operator(my_queue *out, my_stack *stack, const char *buffer) {
	if(!stack_getsize(stack)) {
		return stack_push(stack, buffer);
	}

	char *buf = NULL;
	stack_get_last(stack, &buf);

	if(priority(buffer[0], buf[0])) {
		free(buf);

		return stack_push(stack, buffer);
	}
	
	if(queue_push(out, buf)) {
		free(buf);

		return -1;
	}

	free(buf);
	stack_delete_last(stack);

	while(stack_getsize(stack)) {
		if(stack_get_last(stack, &buf)) {
			return -1;
		}

		if(((priority(buffer[0], buf[0])) > 0) || is_scope(buf[0])) {
			free(buf);

			return stack_push(stack, buffer);
		}

		if(queue_push(out, buf)) {
			free(buf);

			return -1;
		}

		free(buf);
		stack_delete_last(stack);
	}

	return stack_push(stack, buffer);
}

int calc_scope(my_queue *out, my_stack *stack) {
	char *buf = NULL;

	while(stack_getsize(stack)) {
		if(stack_pop(stack, &buf)) {
			return -1;
		} 

		if(buf[0] != '(') {
			if(queue_push(out, buf)) {
				free(buf);

				return -1;
			}
		}
		else {
			free(buf);

			return 0;
		}

		free(buf);
	}

	return -1;
}

int pole_notation(my_queue *in, my_queue *out) {
	my_stack stack;
	stack_init(&stack);

	while(queue_getsize(in)) {
		char *buffer = NULL;

		if(queue_pop(in, &buffer)) {
			printf("[error]");
			stack_clear(&stack);

			return -1;
		}

		if(is_digit(buffer[0])) {
			if(queue_push(out, buffer)) {
				printf("[error]");

				free(buffer);
				stack_clear(&stack);

				return -1;
			}
		}
		else if(is_operator(buffer[0])) {
			if(calc_operator(out, &stack, buffer)) {
				printf("[error]");

				free(buffer);
				stack_clear(&stack);

				return -1;
			}
		}
		else if(is_scope(buffer[0])) {
			switch(buffer[0]) {
				case '(':
					if(stack_push(&stack, buffer)) {
						printf("[error]");

						free(buffer);
						stack_clear(&stack);

						return -1;
					}

					break;
				case ')':
					if(calc_scope(out, &stack)) {
						printf("[error]");

						free(buffer);
						stack_clear(&stack);

						return -1;
					}

					break;
			}
		}
		else {
			printf("[error]");

			free(buffer);
			stack_clear(&stack);

			return -1;
		}

		free(buffer);
	}

	while(stack_getsize(&stack)) {
		char *buffer = NULL;

		if(stack_pop(&stack, &buffer)) {
			printf("[error]");
			stack_clear(&stack);

			return -1;
		}

		if(queue_push(out, buffer)) {
			printf("[error]");

			free(buffer);
			stack_clear(&stack);

			return -1;
		}

		free(buffer);
	}

	return 0;
}

int queue_to_array(my_queue *queue, char ***array, int *arr_size) {
	if(!(*array = (char **)malloc(sizeof(char **) * queue_getsize(queue)))) {
		printf("[error]");

		return -1;
	}

	*arr_size = queue_getsize(queue);
	int index = 0;

	while(queue_getsize(queue)) {
		char *buf = NULL;

		if(queue_pop(queue, &((*array)[index]))) {
			for(int j = 0; j < index; j++) {
				free((*array)[j]);
			}

			free(*array);

			return -1;
		}

		index++;
	}

	return 0;
}

int reverse(char *string) {
	if(!string || !strlen(string)) {
		return -1;
	}

	char buffer[strlen(string) + 1];

	int str_index = strlen(string) - 1;
	int buf_index = 0;

	while(&string[str_index] != string) {
		buffer[buf_index] = string[str_index];
		buf_index++;
		str_index--;
	}

	buffer[buf_index] = string[str_index];
	buffer[++buf_index] = '\0';

	for(int i = 0; i < strlen(string); i++) {
		string[i] = buffer[i];
	}

	return 0;
}

int compare(char *first_operand, char *second_operand) {
	if(strlen(first_operand) > strlen(second_operand)) {
		return 1;
	}

	if(strlen(first_operand) < strlen(second_operand)) {
		return -1;
	}

	for(int i = 0; first_operand[i]; i++) {
		if(first_operand[i] > second_operand[i]) {
			return 1;
		}

		if(first_operand[i] < second_operand[i]) {
			return -1;
		}
	}

	return 0;
}

int delete_zeros(char **number) {
	if(strlen(*number) == 1) {
		return 0;
	}

	if(reverse(*number)) {
		return -1;
	}

	int index = strlen(*number);

	while((*number)[index - 1] == '0' && (index != 1)) {
		index--;
	}

	(*number)[index] = '\0';
	char *new_number = NULL;

	if(!(new_number = (char *)malloc(sizeof(char) * (index + 1)))) {
		return -1;
	}

	snprintf(new_number, index + 1, "%s", *number);
	free(*number);
	*number = new_number;

	if(reverse(*number)) {
		return -1;
	}

	return 0;
}

int convert_to_digit(char symb) {
	switch(symb) {
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		default:
			return -1;
	}
}

char convert_to_char(int digit) {
	switch(digit) {
		case 0:
			return '0';
		case 1:
			return '1';
		case 2:
			return '2';
		case 3:
			return '3';
		case 4:
			return '4';
		case 5:
			return '5';
		case 6:
			return '6';
		case 7:
			return '7';
		case 8:
			return '8';
		case 9:
			return '9';
		default:
			return '\0';
	}
}

int unary_minus(char **first_operand) {
	if(!(*first_operand)) {
		return -1;
	}

	if((strlen(*first_operand) == 1) && (*first_operand)[0] == '0') {
		return 0;
	}

	if((*first_operand)[0] == '-') {
		char *new_first = NULL;

		if(!(new_first = (char*)malloc(sizeof(char) * (strlen(&(*first_operand)[1]) + 1)))) {
			return -1;
		}

		snprintf(new_first, strlen(&(*first_operand)[1]) + 1, "%s", &(*first_operand)[1]);
		free((*first_operand));
		*first_operand = new_first;
	}
	else {
		char *new_first = NULL;

		if(!(new_first = (char*)malloc(sizeof(char) * (strlen((*first_operand)) + 2)))) {
			return -1;
		}

		snprintf(new_first, strlen((*first_operand)) + 2, "-%s", *first_operand);
		free((*first_operand));
		*first_operand = new_first;
	}

	return 0;
}

int sum(char **first_operand, char **second_operand) {
	if(!(*first_operand) || !(*second_operand)) {
		return -1;
	}

	char *result = NULL;
	int first_size = strlen(*first_operand);
	int second_size = strlen(*second_operand);
	int result_size = 0;

	if(first_size >= second_size) {
		result_size = first_size + 2;
	}
	else {
		result_size = second_size + 2;
	}
	
	if(!(result = (char *)malloc(sizeof(char) * result_size))) {
		return -1;
	}

	int reverse_pos = 0;

	if(((*first_operand)[0] == '-') && ((*second_operand)[0] == '-')) {
		reverse_pos = 1;
		result[0] = '-';

		if(unary_minus(first_operand) || unary_minus(second_operand)) {
			return -1;
		}

		first_size = strlen(*first_operand);
		second_size = strlen(*second_operand);
	} 
	else if((*first_operand)[0] == '-') {
		if(unary_minus(first_operand)) {
			return -1;
		}

		if(sub(first_operand, second_operand)) {
			return -1;
		}

		free(result);

		return unary_minus(first_operand);
	}
	else if((*second_operand)[0] == '-') {
		if(unary_minus(second_operand)) {
			return -1;
		}

		free(result);

		return sub(first_operand, second_operand);
	}

	if(reverse(*first_operand) || reverse(*second_operand)) {
		return -1;
	}

	int add = 0;
	int last = 0;

	for(int i = 0; (*first_operand)[i] && (*second_operand)[i]; i++) {
		int res = 0;

		if(add) {
			res +=add;
			add = 0;
		}

		int first = convert_to_digit((*first_operand)[i]);
		int second = convert_to_digit((*second_operand)[i]);
		res += first + second;
		add = res / 10;
		res %= 10;
		result[i + reverse_pos] = convert_to_char(res);
		last = i + reverse_pos;
	}

	if(first_size > second_size) {
		for(int i = second_size; (*first_operand)[i]; i++) {
			int res = 0;

			if(add) {
				res +=add;
				add = 0;
			}

			int first = convert_to_digit((*first_operand)[i]);
			res += first;
			add = res / 10;
			res %= 10;
			result[i + reverse_pos] = convert_to_char(res);
			last = i + reverse_pos;
		}
	}
	else if(second_size > first_size) {
		for(int i = first_size; (*second_operand)[i]; i++) {
			int res = 0;

			if(add) {
				res +=add;
				add = 0;
			}

			int second = convert_to_digit((*second_operand)[i]);
			res += second;
			add = res / 10;
			res %= 10;
			result[i + reverse_pos] = convert_to_char(res);
			last = i + reverse_pos;
		}
	}

	if(add) {
		result[last + 1] = convert_to_char(add);
		result[last + 2] = '\0';
	}
	else{
		result[last + 1] = '\0';
	}
	
	free(*first_operand);

	if(reverse(&result[reverse_pos])) {
		return -1;
	}

	*first_operand  = result;

	return 0;
}

int add_sub(char *first_operand, char *second_operand) {
	if(!first_operand || !second_operand) {
		return -1;
	}

	for(int i = strlen(second_operand) - 1; i >= 0; i--) {
		int first = convert_to_digit(first_operand[i]);
		int second = convert_to_digit(second_operand[i]);

		if(first >= second) {
			first -= second;
		}
		else {
			first += 10;

			if(first_operand[i+1] != '0') {
				int digit = convert_to_digit(first_operand[i+1]);
				digit--;
				first_operand[i+1] = convert_to_char(digit);
			}
			else {
				for(int j = i + 1; j < strlen(first_operand); j++) {
					int digit = convert_to_digit(first_operand[j]);

					if(digit == 0) {
						first_operand[j] = '9';

						continue;
					}

					digit--;
					first_operand[j] = convert_to_char(digit);
				}
			}

			first -= second;
		}

		first_operand[i] = convert_to_char(first);
	}

	return 0;
}

int sub(char **first_operand, char **second_operand) {
	if(!(*first_operand) || !(*second_operand)) {
		return -1;
	}

	int first_size = strlen(*first_operand);
	int second_size = strlen(*second_operand);
	int result_size = 0;

	if(((*first_operand)[0] == '-') && ((*second_operand)[0] == '-')) {

		if(unary_minus(second_operand)) {
			return -1;
		}

		return sum(first_operand, second_operand);
	} 
	else if((*first_operand)[0] == '-') {

		if(unary_minus(second_operand)) {
			return -1;
		}

		return sum(first_operand, second_operand);
	}
	else if((*second_operand)[0] == '-') {

		if(unary_minus(second_operand)) {
			return -1;
		}

		return sum(first_operand, second_operand);
	}

	char *result = NULL;

	switch(compare(*first_operand, *second_operand)) {
		case 0:

			if(!(result = (char *)malloc(sizeof(char) * 2))) {
				return -1;
			} 

			snprintf(result, 2, "0");
			free(*first_operand);
			*first_operand = result;

			break;
		case 1:
			if(reverse(*first_operand) || reverse(*second_operand)) {
				return -1;
			}

			if(add_sub((*first_operand), (*second_operand))) {
				return -1;
			}

			if(reverse(*first_operand)) {
				return -1;
			}

			if(delete_zeros(first_operand)) {
				return -1;
			}

			break;
		case -1:
			if(reverse(*first_operand) || reverse(*second_operand)) {
				return -1;
			}

			if(add_sub((*second_operand), (*first_operand))) {
				return -1;
			}

			free(*first_operand);
			*first_operand = *second_operand;
			*second_operand = NULL;

			if(reverse(*first_operand)) {
				return -1;
			}

			if(delete_zeros(first_operand)) {
				return -1;
			}

			if(unary_minus(first_operand)) {
				return -1;
			}

			break;

	}

	return 0;

}

int mul(char **first_operand, char **second_operand) {
	if(!(*first_operand) || !(*second_operand)) {
		return -1;
	}

	int minus_flag = 0;

	if(((*first_operand)[0] == '-') && (*second_operand)[0] == '-') {
		if(unary_minus(first_operand) || unary_minus(second_operand)) {
			return -1;
		}
	}
	else if((*first_operand)[0] == '-') {
		minus_flag = 1;

		if(unary_minus(first_operand)) {
			return -1;
		}
	}
	else if((*second_operand)[0] == '-') {
		minus_flag = 1;

		if(unary_minus(second_operand)) {
			return -1;
		}
	}

	if(reverse(*first_operand) || reverse(*second_operand)) {
		return -1;
	}

	int result_size = 0;

	switch(compare(*first_operand, (*second_operand))) {
		case 0:
			result_size = strlen(*first_operand) + 1;
			break;
		case 1:
			result_size = strlen(*first_operand) + 1;
			break;
		case -1:
			result_size = strlen(*second_operand) + 1;
			break;
	}

	int add = 0;
	char *result = NULL;
	my_queue list;
	queue_init(&list);

	for(int i = 0; i < strlen(*second_operand); i++) {
		int second = convert_to_digit((*second_operand)[i]);

		if(!(result = (char *)malloc(sizeof(char) * (result_size + i + 1)))) {
			queue_clear(&list);

			return -1;
		}

		for(int k = 0; k < i; k++) {
			result[k] = '0';
		}

		for(int j = 0; j < strlen(*first_operand); j++) {
			int first = convert_to_digit((*first_operand)[j]);
			int res = add;

			res += (first * second);
			add = res / 10;
			res %= 10;
			result[j + i] = convert_to_char(res);
		}

		if(add) {
			result[strlen(*first_operand) + i] = convert_to_char(add);
			result[strlen(*first_operand) + i + 1] = '\0';
		}
		else {
			result[strlen(*first_operand) + i] = '\0';
		}

		add = 0;
		if(reverse(result)) {
			queue_clear(&list);

			return -1;
		}

		if(queue_push(&list, result)) {
			queue_clear(&list);

			return -1;
		}

		free(result);
	}

	node *first_op = NULL;
	node *second_op = NULL;

	if(queue_getsize(&list) == 1) {

		if(queue_get(&list, 0, &first_op)) {
			queue_clear(&list);

			return -1;
		}
	}
	else {

		while(queue_getsize(&list) != 1) {


			if(queue_get(&list, 0, &first_op) || queue_get(&list, 1, &second_op)) {
				queue_clear(&list);

				return -1;
			}

			if(sum(&(first_op->data), &(second_op->data))){
				queue_clear(&list);

				return -1;
			}

			queue_remove(&list, 1);
		}
	}

	if(delete_zeros(&(first_op->data))) {
		queue_clear(&list);

		return -1;
	}

	if(minus_flag) {
		if(unary_minus(&(first_op->data))) {
			queue_clear(&list);

			return -1;
		}
	}

	free(*first_operand);
	*first_operand = NULL;

	if(!(*first_operand = (char *)malloc(sizeof(char) * (strlen(first_op->data) + 1)))) {
		queue_clear(&list);

		return -1;
	}

	snprintf(*first_operand, strlen(first_op->data) + 1, "%s", first_op->data);
	queue_clear(&list);

	if(reverse(*second_operand)) {
		return -1;
	}

	return 0;
}

int division(char **first_operand, char **second_operand) {

	if((*second_operand)[0] == 0) {
		return -1;
	}

	int minus_flag = 0;

	if(((*first_operand)[0] == '-') && (*second_operand)[0] == '-') {
		if(unary_minus(first_operand) || unary_minus(second_operand)) {
			return -1;
		}
	}
	else if((*first_operand)[0] == '-') {
		minus_flag = 1;

		if(unary_minus(first_operand)) {
			return -1;
		}
	}
	else if((*second_operand)[0] == '-') {
		minus_flag = 1;

		if(unary_minus(second_operand)) {
			return -1;
		}
	}

	switch(compare(*first_operand, *second_operand)) {
		case -1:
			free(*first_operand);
			*first_operand = NULL;

			if(!(*first_operand = (char *)malloc(sizeof(char) * 2))) {
				return -1;
			}

			snprintf(*first_operand, 2, "0");

			return 0;
		case 0:
			free(*first_operand);
			*first_operand = NULL;

			if(!(*first_operand = (char *)malloc(sizeof(char) * 2))) {
				return -1;
			}

			snprintf(*first_operand, 2, "1");

			if(minus_flag) {
				if(unary_minus(first_operand)) {
					return -1;
				}
			}

			return 0;
	}

	char *dividend = NULL;
	char *divider = NULL;

	if(!(dividend = (char *)malloc(sizeof(char) * (strlen(*first_operand) + 1)))) {
		return -1;
	}

	if(!(divider = (char *)malloc(sizeof(char) * (strlen(*second_operand) + 1)))) {
		free(dividend);

		return -1;
	}

	dividend[0] = '\0';
	snprintf(divider, strlen(*second_operand) + 1, "%s", *second_operand);

	char *result = NULL;

	if(!(result = (char *)malloc(sizeof(char) * (strlen(*first_operand) + 1)))) {
		free(divider);
		free(dividend);

		return -1;
	}

	int result_index = 0;
	int index = 0;

	while(index != strlen(*first_operand)) {
		int str_end = strlen(dividend);
		int cur_index = 0;

		while(((compare(dividend, divider) < 0) && (index != strlen(*first_operand)))/* && (index != strlen(divider))*/) {			
			dividend[cur_index + str_end] = (*first_operand)[index];
			dividend[cur_index + str_end + 1] = '\0';
printf("dividend:%s index:%d first:%c\n", dividend, index, (*first_operand)[index]);
			index++;

			if(dividend[0] == '0') {
				break;
			}

			cur_index++;			
		}
printf("dividend_s:%s divider_s:%s\n", dividend, divider);
		char *backup = NULL;

		if(!(backup = (char *)malloc(sizeof(char) * (strlen(dividend) + 1)))) {
			free(divider);
			free(dividend);
			free(result);

			return -1;
		}

		snprintf(backup, strlen(dividend) + 1, "%s", dividend);

		for(int i = 0; i <= 10; i++) {
			char *cur_i = NULL;

			if(!(cur_i = (char *)malloc(sizeof(char) * 3))) {
				free(divider);
				free(dividend);
				free(result);
				free(backup);

				return -1;
			}

			snprintf(cur_i, 3, "%d", i);

			if(mul(&cur_i, &divider)) {
				free(divider);
				free(dividend);
				free(cur_i);
				free(result);
				free(backup);

				return -1;
			}
printf("dividend:%s divider:%s cur_i:%s\n", dividend, divider, cur_i);
			if(sub(&dividend, &cur_i)) {
				free(divider);
				free(dividend);
				free(cur_i);
				free(result);
				free(backup);

				return -1;
			}
printf("ostatok:%s\n", dividend);
			if(dividend[0] == '0') {
				result[result_index] = convert_to_char(i);
				result_index++;
				free(cur_i);
				free(dividend);

				if(!(dividend = (char *)malloc(sizeof(char) * (strlen(*first_operand) + 1)))) {
					free(divider);
					free(result);
					free(backup);

					return -1;
				}

				dividend[0] = '\0';

				break;
			}

			if(dividend[0] == '-') {
				result[result_index] = convert_to_char(i - 1);
				result_index++;
				free(cur_i);
				free(dividend);

				if(!(dividend = (char *)malloc(sizeof(char) * (strlen(*first_operand) + 1)))) {
					free(divider);
					free(result);
					free(backup);

					return -1;
				}

				//snprintf(dividend, strlen(*first_operand) + 1, "%s", backup);

				if(!(cur_i = (char *)malloc(sizeof(char) * 3))) {
					free(divider);
					free(dividend);
					free(backup);
					free(result);

					return -1;
				}


				snprintf(cur_i, 2, "%d", --i);

				if(mul(&cur_i, &divider)) {
					free(divider);
					free(dividend);
					free(backup);
					free(result);
					free(cur_i);

					return -1;
				}

				if(sub(&backup, &cur_i)) {
					free(divider);
					free(dividend);
					free(backup);
					free(result);
					free(cur_i);

					return -1;
				}

				snprintf(dividend, strlen(*first_operand) + 1, "%s", backup);
				free(cur_i);

				break;
			}

			free(dividend);

			if(!(dividend = (char *)malloc(sizeof(char) * (strlen(*first_operand) + 1)))) {
				free(divider);
				free(dividend);
				free(backup);
				free(result);
				free(cur_i);

				return -1;
			}

			snprintf(dividend, strlen(*first_operand) + 1, "%s", backup);
			free(cur_i);
		}
		free(backup);
	}

	result[result_index] = '\0';
	free(*first_operand);
	*first_operand = result;

	free(dividend);
	free(divider);

	if(minus_flag) {
		if(unary_minus(first_operand)) {
			return -1;
		}
	}

	return 0;	
}

int calculate(my_queue *pole_note) {
	int index = 0;

	while(queue_getsize(pole_note) != 1) {
		node *current_node = NULL;

		if(queue_get(pole_note, index, &current_node)) {
			printf("[error]");

			return -1;
		}

		node *first_operand = NULL;
		node *second_operand = NULL;

		switch((current_node->data)[0]) {
			case '~':

				if(queue_get(pole_note, index-1, &first_operand)) {
					printf("[error]");

					return -1;
				}

				if(unary_minus(&(first_operand->data))) {
					printf("[error]");

					return -1;
				}

				queue_remove(pole_note, index--);

				break;
			case '+':

				if(queue_get(pole_note, index-2, &first_operand) || queue_get(pole_note, index-1, &second_operand)) {
					printf("[error]");

					return -1;
				}

				if(sum(&(first_operand->data), &(second_operand->data))){
					printf("[error]");

					return -1;
				}

				queue_remove(pole_note, index--);
				queue_remove(pole_note, index--);

				break;
			case '-':

				if(queue_get(pole_note, index-2, &first_operand) || queue_get(pole_note, index-1, &second_operand)) {
					printf("[error]");

					return -1;
				}

				if(sub(&(first_operand->data), &(second_operand->data))){
					printf("[error]");

					return -1;
				}

				queue_remove(pole_note, index--);
				queue_remove(pole_note, index--);

				break;
			case '*':
				if(queue_get(pole_note, index-2, &first_operand) || queue_get(pole_note, index-1, &second_operand)) {
					printf("[error]");

					return -1;
				}

				if(mul(&(first_operand->data), &(second_operand->data))){
					printf("[error]");

					return -1;
				}

				queue_remove(pole_note, index--);
				queue_remove(pole_note, index--);

				break;
			case '/':
				if(queue_get(pole_note, index-2, &first_operand) || queue_get(pole_note, index-1, &second_operand)) {
					printf("[error]");

					return -1;
				}

				if(division(&(first_operand->data), &(second_operand->data))){
					printf("[error]");

					return -1;
				}

				queue_remove(pole_note, index--);
				queue_remove(pole_note, index--);

				break;
		}

		index++;
	}

	return 0;
}
