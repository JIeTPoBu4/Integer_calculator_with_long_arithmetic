#include "functions.h"

#define BUFFER_SIZE 256

int main() {
	char buf[BUFFER_SIZE];

	fgets(buf, BUFFER_SIZE, stdin);

	if(buf[strlen(buf) - 1] == '\n') {
		buf[strlen(buf) - 1] = '\0';
	}
	
	my_queue queue;
	my_queue pole_note;
	queue_init(&queue);
	queue_init(&pole_note);

	if(pars(buf, &queue)) {
		queue_clear(&queue);
		queue_clear(&pole_note);

		return 0;
	}

	if(pole_notation(&queue, &pole_note)) {
		queue_clear(&queue);
		queue_clear(&pole_note);

		return 0;
	}

	if(calculate(&pole_note)) {
		queue_clear(&queue);
		queue_clear(&pole_note);

		return 0;
	}
	
	char *buff = NULL;
	queue_get_last(&pole_note, &buff);
	printf("%s\n", buff);
	free(buff);
	queue_clear(&queue);
	queue_clear(&pole_note);

	return 0;
}