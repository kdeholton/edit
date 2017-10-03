#ifndef _SPAN_H_
#define _SPAN_H_

#include <stdlib.h>

#include "error.h"

typedef struct span {
  struct span *next;
  struct span *prev;

  uint32_t offset;
  uint32_t length;
  int buffer;
} span_t;

// PUBLIC FUNCTION DECLARATIONS
ErrorCode initialize(span_t **head_sentinel, span_t **tail_sentinel);
/*ErrorCode insert(uint32_t index, char *buffer, uint32_t length);
ErrorCode replace(uint32_t index, char *buffer, uint32_t length);
ErrorCode erase(uint32_t index, uint32_t length);
ErrorCode undo();
ErrorCode redo();*/

#endif
