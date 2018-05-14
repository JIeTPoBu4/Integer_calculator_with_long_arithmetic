#include "my_queue.h"
#include "my_stack.h"
#include "functions.h"

#define BUFFER_SIZE 256

int main() {
	char buf[BUFFER_SIZE];

	scanf("%s", buf);

	my_queue queue;
	my_queue pole_note;
	queue_init(&queue);
	queue_init(&pole_note);
	new_pars(buf, &queue);
	printf("%d\n", queue_getsize(&queue));
	pole_notation(&queue, &pole_note);
	/*
	printf("%d\n", queue_getsize(&pole_note));

	while(queue_getsize(&pole_note)) {
		char *buf = NULL;
		queue_pop(&pole_note, &buf);
		printf("%s\n", buf);

		free(buf);
	}
	*/
	char **pole_arr = NULL;
	int arr_size = 0;
	queue_to_array(&pole_note, &pole_arr, &arr_size);

	for(int i = 0; i < arr_size; i++) {
		printf("%s\n", pole_arr[i]);
	}

	calculate(pole_arr, arr_size);
	printf("%s\n", pole_arr[arr_size - 1]);

	queue_clear(&queue);
	queue_clear(&pole_note);

	return 0;
}