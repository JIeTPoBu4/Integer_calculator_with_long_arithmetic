#include "my_queue.h"

int queue_init(my_queue *queue) {
  queue->head = NULL;
  queue->tail = NULL;
  queue->size = 0;

  return 0;
}

int queue_push(my_queue *queue, const char *string) {
  if (!(queue->head)) {
    if (!(queue->head = (node *)malloc(sizeof(node)))) {
      perror("[error]");

      return -1;
    }

    if (!(queue->head->data = (char *)malloc(sizeof(char) * (strlen(string) + 1)))) {
      perror("[error]");
      free(queue->head);
      queue->head = NULL;

      return -1;
    }

    snprintf(queue->head->data, strlen(string) + 1, "%s", string);
    queue->head->next = NULL;
    queue->tail = queue->head;
    (queue->size)++;

    return 0;
  }

  if (queue->head == queue->tail) {
    if (!(queue->tail = (node *)malloc(sizeof(node)))) {
      perror("[error]");

      return -1;
    }

    if (!(queue->tail->data = (char *)malloc(sizeof(char) * (strlen(string) + 1)))) {
      perror("[error]");
      free(queue->tail);
      queue->tail = queue->head;

      return -1;
    }

    snprintf(queue->tail->data, strlen(string) + 1, "%s", string);
    queue->head->next = queue->tail;
    queue->tail->next = NULL;
    (queue->size)++;

    return 0;
  }

  node *temp = NULL;

  if (!(temp = (node *)malloc(sizeof(node)))) {
    perror("[error]");

    return -1;
  }

  if (!(temp->data = (char *)malloc(sizeof(char) * (strlen(string) + 1)))) {
    perror("[error]");
    free(temp);

    return -1;
  }

  snprintf(temp->data, strlen(string) + 1, "%s", string);
  temp->next = NULL;
  queue->tail->next = temp;
  queue->tail = temp;
  (queue->size)++;

  return 0;
}

int queue_pop(my_queue *queue, char **string) {
  if (!(queue->head)) {
    perror("[error]");
    string = NULL;

    return -1;
  }

  if (!(*string = (char *)malloc(sizeof(char) * (strlen(queue->head->data) + 1)))) {
    perror("[error]");

    return 1;
  }

  snprintf(*string, strlen(queue->head->data) + 1, "%s", queue->head->data);
  free(queue->head->data);
  node *temp = queue->head;

  if (queue->head == queue->tail) {
    queue->head = NULL;
    queue->tail = NULL;
  } else {
    queue->head = queue->head->next;
  }

  (queue->size)--;
  free(temp);
  
  return 0;
}

int queue_clear(my_queue *queue) {
  while (queue->head) {
    free(queue->head->data);
    node *temp = queue->head->next;
    free(queue->head);
    queue->head = temp;
  }

  queue->tail = NULL;
  queue->size = 0;
  return 0;
}

int queue_getsize(const my_queue *queue) { return queue->size; }

int queue_get_last(my_queue *queue, char **buf) {
  if(!(queue->tail->data)) {
    printf("[error]");

    return -1;
  }

  if(!(*buf = (char*)malloc(sizeof(char) * (strlen(queue->tail->data) + 1)))) {
    printf("[error]");

    return -1;
  }

  snprintf(*buf, strlen(queue->tail->data) + 1, "%s", queue->tail->data);

  return 0;
}