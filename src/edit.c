#include "edit.h"

int checkArgs(int argc, char **argv) {
  return 0;
}

int main(int argc, char **argv) {
  span_t *head_sentinel;
  span_t *tail_sentinel;

  ErrorCode status = initialize(&head_sentinel, &tail_sentinel);
  if (status != k_ok) {
    // Couldn't create new linked list!
    printf("%s\n", "Couldn't create new list");
    return status;
  }

  printf("Head is at: %p\nTail is at: %p\n", (void*)head_sentinel, (void*)tail_sentinel);
  printf("%s\n", "Edit!");
  return 0;
}
