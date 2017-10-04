#include "doubly_linked_list.h"

ErrorCode initialize(node_t **head_sentinel, node_t **tail_sentinel) {
  *head_sentinel = malloc(sizeof(node_t));
  if (!*head_sentinel) {
    return k_out_of_memory;
  }
  *tail_sentinel = malloc(sizeof(node_t));
  if (!*head_sentinel) {
    return k_out_of_memory;
  }

  (*head_sentinel)->next = *tail_sentinel;
  (*head_sentinel)->prev = NULL;
  (*tail_sentinel)->next = NULL;
  (*tail_sentinel)->prev = *head_sentinel;

  (*head_sentinel)->line = 0;
  (*tail_sentinel)->line = NULL;

  return k_ok;
}

ErrorCode set_data(node_t *node, char* line) {
  if (node == NULL) {
    return k_null_ptr;
  }

  node->line = line;
  return k_ok;
}

ErrorCode insert_before(node_t *new_node, node_t *existing_node) {
  if (new_node == NULL || existing_node == NULL) {
    return k_null_ptr;
  }
  node_t *prev = existing_node->prev;
  prev->next = new_node;
  new_node->prev = prev;
  new_node->next = existing_node;
  existing_node->prev = new_node;
  return k_ok;
}

ErrorCode insert_after(node_t *new_node, node_t *existing_node) {
  if (new_node == NULL || existing_node == NULL) {
    return k_null_ptr;
  }
  return insert_before(new_node, existing_node->next);
}

ErrorCode insert_at_head(node_t *new_node, node_t *head_sentinel) {
  if (head_sentinel == NULL || new_node == NULL) {
    return k_null_ptr;
  }
  return insert_after(new_node, head_sentinel);
}

ErrorCode insert_at_tail(node_t *new_node, node_t *tail_sentinel) {
  if (tail_sentinel == NULL || new_node == NULL) {
    return k_null_ptr;
  }
  return insert_before(new_node, tail_sentinel);
}

ErrorCode last_node(node_t *node) {
  return (node->next->next == NULL);
}

ErrorCode string_to_linked_list(char *data, node_t *head_sentinel) {
  return k_ok;
}
