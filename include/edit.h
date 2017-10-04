#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "doubly_linked_list.h"
#include "error.h"

#define MAX_BUF_LEN (1000000)

typedef enum EditorMode {
  command_mode = 0,
  insert_mode = 1,
} EditorMode;
