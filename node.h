#ifndef _NODE
#define _NODE

typedef struct node {
  struct node *next;
  char *data;
} node;

#endif