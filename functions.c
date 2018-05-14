#include "functions.h"

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
/*
int is_symbol(char symb) {
	char symbols[] = { "()+-/*=" };

	for(int i = 0; symbols[i]; i++) {
		if(symbols[i] == symb)
			return 1;
	}

	return 0;
}

int cat_str(char *to, char *from) {
	size_t size = BUFFER_SIZE;

	while((strlen(to) + strlen(from)) >= size) {
		size *= 2;
	}

	char *buffer = NULL;

	if(!(buffer = (char*)malloc(sizeof(char) * size))) {
		printf("[error]");

		return -1;
	}

	snprintf(buffer, size, "%s%s", to, from);
	free(to);
	to = buffer;

	return 0;
}

int symbols_pars(const char *str, int index, char *buf, my_queue *queue, int *flag) {
	char *symb[] = { "(", ")", "+", "-", "/", "*", "=", "" };

	for (int i = 0; i < sizeof(symb)/sizeof(char*) ; i++) {

			if (str[index] == symb[i][0]) {

				if (*flag) {
					queue->push(buf);
					*flag = 0;
				}

				if (index) {
					if ((str[index] == '-') && (str[index - 1] == '(')) {

						if(!(buf = (char*)malloc(sizeof(char) * BUFFER_SIZE))) {
							printf("[error]");

							return -1;
						}

						*flag = 1;
						snprintf(buf, BUFFER_SIZE, "%s", symb[i]);

						return 0;
					}
				}

				if ((str[index] == '-') && (index == 0)) {

					if(!(buf = (char*)malloc(sizeof(char) * BUFFER_SIZE))) {
						printf("[error]");

						return -1;
					}

					*flag = 1;
					snprintf(buf, BUFFER_SIZE, "%s", symb[i]);

					return 0;
				}
				else {
					queue.push(symb[i]);
				}
			}
			else
			{
				continue;
			}
	}

	return 0;
}

int number_pars(const char *str, int index, char *buf, my_queue *queue, int *flag) {

	if (is_digit(str[index])) {
		char *dig[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };

		for (int i = 0; i < sizeof(dig)/sizeof(char*); i++) {

			if (str[index] == dig[i][0]) {

				if (!(*flag)) {

					if(!(buf = (char*)malloc(sizeof(char) * BUFFER_SIZE))) {
						printf("[error]");

						return -1;
					}

					*flag = 1;
					snprintf(buf, BUFFER_SIZE, "%s", dig[i]);

					return 0;
				}
				else {
					cat_str(buf, dig[i]);

					return 0;
				}
			}
			else {
				continue;
			}
		}
	}

	return 0;
}


int pars(char *str, my_queue *queue)
{
	int flag = 0;
	char *buf;

	for (int i = 0; str[i] ; i++) {
		if(symbols_pars(str, i, buf, queue, &flag) < 0) {
			return -1;
		}

		if(number_pars(str, i, buf, queue, &flag) < 0) {
			return -1;
		}		
	}

	if (flag) {
		queue->push(buf);
	}

	return 0;
}
*/

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

/*
int pole_notation(my_queue *in, char ***out) {
	if(!(*out = (char**)malloc((queue_getsize(in) + 1) * sizeof(char*)))) {
		printf("[error]");

		return -1;
	}

	my_stack stack;
	stack_init(&stack);

	int ptr = 0;

	while(queue_getsize(in)) {
		char *buffer = NULL;

		if(!queue_pop(in, &buffer)) {
			printf("[error]");
			stack_clear(&stack);

			return -1;
		}

		if(is_digit(buffer[strlen(buffer) - 1])) 	{
			if(!((*out)[ptr] = (char*)malloc(sizeof(char) * (strlen(buffer) + 1)))) {
				printf("[error]");
				stack_clear(&stack);

				return -1;
			}

			snprintf((*out)[ptr], strlen(buffer) + 1, "%s", buffer);
			free(buffer);
			ptr++;

			continue;
		}

		if(is_operator(buffer[strlen(buffer) - 1])) {

		}

		if(buffer[strlen(buffer) - 1] == '(') {
			if(!stack_push(&stack, buffer)) {
				printf("[error]");
				stack_clear(&stack);

				return -1;
			}

			free(buffer);

			continue;
		}

		if(buffer[strlen(buffer) - 1] == ')') {
			char *buf = NULL;

			if(stack_pop(&stack, &buf)) {
				printf("[error]");
				stack_clear(&stack);
				free(buffer);

				return -1;
			}

			while(strcmp(buf, "(")) {
				if(!((*out)[ptr] = (char*)malloc(sizeof(char) * (strlen(buf) + 1)))) {
					printf("[error]");

					return -1;
				}

				snprintf((*out)[ptr], strlen(buf) + 1, "%s", buf);
				free(buf);
				ptr++;

				if(stack_pop(&stack, &buf)) {
					printf("[error]");
					stack_clear(&stack);
					free(buffer);

					return -1;
				}
			}
			

			free(buf);
			free(buffer);

			continue;
		}



	}


}
*/

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
	} 
	else if((*first_operand)[0] == '-') {
		if(unary_minus(first_operand)) {
			return -1;
		}

		return sub(second_operand, first_operand);
	}
	else if((*second_operand)[0] == '-') {
		if(unary_minus(second_operand)) {
			return -1;
		}

		return sub(first_operand, second_operand);
	}

	if(reverse(&(*first_operand)[reverse_pos]) || reverse(&(*second_operand)[reverse_pos])) {
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
	free(*second_operand);

	if(reverse(&result[reverse_pos])) {
		return -1;
	}

	*first_operand = *second_operand = result;

	return 0;
}

int sub(char **first_operand, char **second_operand) {
	if(!(*first_operand) || !(*second_operand)) {
		return -1;
	}

	char *result = NULL;
	int first_size = strlen(*first_operand);
	int second_size = strlen(*second_operand);
	int result_size = 0;

	if(((*first_operand)[0] == '-') && ((*second_operand)[0] == '-')) {
		/*
		char *new_first = NULL;
		char *new_second = NULL;

		if(!(new_first = (char*)malloc(sizeof(char) * (strlen(&(*first_operand)[1]) + 1)))) {
			return -1;
		}

		if(!(new_second = (char*)malloc(sizeof(char) * (strlen(&(*second_operand)[1]) + 1)))) {
			return -1;
		}

		snprintf(new_first, strlen(&(*first_operand)[1]) + 1, "%s", &(*first_operand)[1]);
		snprintf(new_second, strlen(&(*second_operand)[1]) + 1, "%s", &(*second_operand)[1]);
		free((*second_operand));
		free((*first_operand));
		*first_operand = new_first;
		*second_operand = new_second;
		*/
		if(unary_minus(first_operand) || unary_minus(second_operand)) {
			return -1;
		}

		return sub(second_operand, first_operand);
	} 
	else if((*first_operand)[0] == '-') {
		if(unary_minus(second_operand)) {
			return -1;
		}

		return sum(first_operand, second_operand);
	}
	else if((*second_operand)[0] == '-') {
		/*
		char *new_second = NULL;

		if(!(new_second = (char*)malloc(sizeof(char) * (strlen(&(*second_operand)[1]) + 1)))) {
			return -1;
		}

		snprintf(new_second, strlen(&(*second_operand)[1]) + 1, "%s", &(*second_operand)[1]);
		free((*second_operand));
		*second_operand = new_second;
		*/
		if(unary_minus(second_operand)) {
			return -1;
		}

		return sum(first_operand, second_operand);
	}


}

int calculate(char **pole_note, int arr_size) {
	for(int i = 0; i < arr_size; i++) {
		switch(pole_note[i][0]) {
			case '~':
				if(unary_minus(&pole_note[i-1])) {
					printf("[error]");

					return -1;
				}

				free(pole_note[i]);
				pole_note[i] = pole_note[i-1];
				break;
			case '+':
				if(sum(&pole_note[i-1], &pole_note[i-2])){
					printf("[error]");

					return -1;
				}
printf("here: %s\n", pole_note[i-1]);
				free(pole_note[i]);
				pole_note[i] = pole_note[i-1];
				break;
			case '-':
				if(sub(&pole_note[i-2], &pole_note[i-1])){
					printf("[error]");

					return -1;
				}

				free(pole_note[i]);
				pole_note[i] = pole_note[i-1];
				break;
		}
	}

	return 0;
}