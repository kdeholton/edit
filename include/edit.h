#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <math.h>
#include "doubly_linked_list.h"
#include "error.h"

#define MAX_BUF_LEN (1000000)

#define NUM_CHARS(x) (strlen(x->line)-1)
#define NUM_LINES(x) (NUM_CHARS(x) > 0 ? ((int)(floor(NUM_CHARS(x)/((double)COLS)))+1) : 1)

typedef enum EditorMode {
  command_mode = 0,
  insert_mode = 1,
} EditorMode;

void handle_resize(int i);
ErrorCode update_screen();
