#include "span.h"

ErrorCode initialize(span_t **head_sentinel, span_t **tail_sentinel) {
  *head_sentinel = malloc(sizeof(span_t));
  if (!*head_sentinel) {
    return k_out_of_memory;
  }
  *tail_sentinel = malloc(sizeof(span_t));
  if (!*head_sentinel) {
    return k_out_of_memory;
  }

  (*head_sentinel)->next = *tail_sentinel;
  (*head_sentinel)->prev = NULL;
  (*tail_sentinel)->next = NULL;
  (*tail_sentinel)->prev = *head_sentinel;

  (*head_sentinel)->offset = 0;
  (*head_sentinel)->length = 0;
  (*head_sentinel)->buffer = 0;
  (*tail_sentinel)->offset = 0;
  (*tail_sentinel)->length = 0;
  (*tail_sentinel)->buffer = 0;

  return k_ok;
}
