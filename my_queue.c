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
      return -1;
    }

    if (!(queue->head->data = (char *)malloc(sizeof(char) * (strlen(string) + 1)))) {
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
      return -1;
    }

    if (!(queue->tail->data = (char *)malloc(sizeof(char) * (strlen(string) + 1)))) {
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
    return -1;
  }

  if (!(temp->data = (char *)malloc(sizeof(char) * (strlen(string) + 1)))) {
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
    string = NULL;

    return -1;
  }

  if (!(*string = (char *)malloc(sizeof(char) * (strlen(queue->head->data) + 1)))) {
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
    return -1;
  }

  if(!(*buf = (char*)malloc(sizeof(char) * (strlen(queue->tail->data) + 1)))) {
    return -1;
  }

  snprintf(*buf, strlen(queue->tail->data) + 1, "%s", queue->tail->data);

  return 0;
}

int queue_get(my_queue *queue, int index, node **data) {
  if((index >= queue_getsize(queue)) || (index < 0)) {
    return -1;
  }

  node *current_node = queue->head;
  int i = 0;

  while(i != index) {
    current_node = current_node->next;
    i++;
  }

  *data = current_node;

  return 0;
}

int queue_remove(my_queue *queue, int index) {
  if(index >= queue_getsize(queue)) {
    return -1;
  }

  if(index == 0) {
    node *to_delete = queue->head;
    queue->head = queue->head->next;
    free(to_delete->data);
    free(to_delete);

    if(queue_getsize(queue) == 1) {
      queue->tail = queue->head = NULL;
    }
    
    (queue->size)--;

    return 0;
  }

  node *current_node = queue->head;
  int i = 0;

  while(i != index - 1) {
    current_node = current_node->next;
    i++;
  }

  if(i == (queue_getsize(queue) - 2)) {
    queue->tail = current_node;
  }

  node *to_delete = current_node->next;
  current_node->next = to_delete->next;
  free(to_delete->data);
  free(to_delete);
  (queue->size)--;

  return 0;
}