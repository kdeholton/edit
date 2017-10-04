#ifndef _DOUBLY_LINKED_LIST_H_
#define _DOUBLY_LINKED_LIST_H_

#include <stdlib.h>

#include "error.h"

typedef struct node {
  struct node *next;
  struct node *prev;
  char *line;
} node_t;

// PUBLIC FUNCTION DECLARATIONS
ErrorCode initialize(node_t **head_sentinel, node_t **tail_sentinel);
ErrorCode set_data(node_t *node, char* line);
ErrorCode insert_before(node_t *new_node, node_t *existing_node);
ErrorCode insert_after(node_t *new_node, node_t *existing_node);
ErrorCode insert_at_head(node_t *new_node, node_t *head_sentinel);
ErrorCode insert_at_tail(node_t *new_node, node_t *tail_sentinel);
ErrorCode last_node(node_t *node);

#endif
