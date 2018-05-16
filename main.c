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
	
	/*queue_to_array(&pole_note, &pole_arr, &arr_size);

	for(int i = 0; i < arr_size; i++) {
		printf("%s\n", pole_arr[i]);
	}
	*/

	calculate(&pole_note);
	char *buff = NULL;
	queue_get_last(&pole_note, &buff);
	printf("%s\n", buff);
	free(buff);
	queue_clear(&queue);
	queue_clear(&pole_note);

	return 0;
}