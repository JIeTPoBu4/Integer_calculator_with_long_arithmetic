#include "functions.h"

int main() {
	char *buf = NULL;

	if(input(&buf)) {
		return 0;
	}

	my_queue queue;
	my_queue pole_note;
	queue_init(&queue);
	queue_init(&pole_note);

	if(pars(buf, &queue)) {
		queue_clear(&queue);
		queue_clear(&pole_note);
		free(buf);

		return 0;
	}

	free(buf);

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