// TODO refactor!
#include "edit.h"

// Private Variables
static char *original;          // This buffer is read-only
static char *new;               // This buffer is append-only
static node_t *head_sentinel;
static node_t *tail_sentinel;
static uint8_t cursor_x;
static uint8_t cursor_y;
static EditorMode mode;
static uint32_t top_line;
static node_t *top_line_node;
static node_t *current_line_node;
static uint8_t current_line_index;

void printUsage() {
  printf("Usage: edit <filename>\n");
}

void signal_registration() {
  // TODO be careful with what is called here
  signal(SIGWINCH, handle_resize);
}

ErrorCode checkArgs(int argc, char **argv, char **filename) {
  if (argc != 2) {
    printf("edit requires exactly 1 argument.\n");
    printUsage();
    return k_bad_param;
  }
  *filename = argv[1];
  return k_ok;
}

// TODO currently this reads, then closes. So have to open to write.
// Should I leave the file open while I'm editing it?
ErrorCode readFile(char* filename) {
  FILE *fp = fopen(filename, "r+");
  if (fp == NULL) {
    return k_file_open_error;
  }

  char *line = NULL;
  size_t len = 0;
  node_t *curr = head_sentinel;
  node_t *new;
  while (getline(&line, &len, fp) != -1) {
    new = malloc(sizeof(node_t));
    set_data(new, line);
    insert_after(new, curr);
    curr = new;
    line = NULL;
  }

  top_line_node = head_sentinel->next;
  current_line_node = head_sentinel->next;

	fclose(fp);
  return k_ok;
}

ErrorCode ncurses_init() {
  initscr();              // Start curses mode
  raw();                  // Disable line buffering
  keypad(stdscr, true);   // Enable fn keys, arrow keys, etc
  noecho();               // Don't echo keystrokes
  cursor_x = 0;
  cursor_y = 0;

  mode = command_mode;
  top_line = 0;
  current_line_index = 0;

  return k_ok;
}

void handle_resize(int i) {
 update_screen(); 
}

ErrorCode update_status_bar() {
  char *mode_str;
  switch (mode) {
    case command_mode:
    mode_str = "COMMAND MODE     (%d, %d)     line_top: %d        node: %p     curr Line: %d";
    break;
  case insert_mode:
    mode_str = "INSERT MODE";
    break;
  }

  mvprintw(LINES-1, 0, mode_str, cursor_x, cursor_y, top_line, (void*)top_line_node, current_line_index);
  move(cursor_y, cursor_x);
  refresh();

  return k_ok;
}

ErrorCode update_screen() {
  int y;
  node_t *node = top_line_node;
  for (y = 0; y < LINES-1;) {
    if (node != tail_sentinel) {
      int num_lines = NUM_LINES(node);
      int i = 0;
      while(i < num_lines) {
        mvprintw(y, 0, node->line+(COLS*i));
        i++;
        y++;
      }
      node = node->next;
    } else {
      mvprintw(y, 0, "");
    }
  }
  update_status_bar();
  refresh();
  move(cursor_y, cursor_x);
  return k_ok;
}

ErrorCode scroll_up() {
  // TODO
  if (top_line == 0) {
    return k_ok;
  }
  top_line--;
  top_line_node = top_line_node->prev;
  current_line_node = current_line_node->prev;
  return k_ok;
}

ErrorCode scroll_down() {
  // TODO
  if (last_node(current_line_node)) {
    return k_ok;
  }
  top_line++;
  top_line_node = top_line_node->next;
  current_line_node = current_line_node->next;
  return k_ok;
}

ErrorCode update_cursor(int8_t x, int8_t y) {
  if (x == -1) {
    if (cursor_x > 0) {
      current_line_index--;
      cursor_x -= 1;
    } else if (cursor_x == 0) {
      if (NUM_LINES(current_line_node) > 1 && current_line_index > 0) {
        // Gotta pop back up to the previous screen line!
        cursor_x = COLS-1;
        cursor_y -= 1;
      }
    }
  } else if (x == 1) {
    if (cursor_x < COLS-1) {
      if (current_line_index > NUM_CHARS(current_line_node)) {
      } else {
        cursor_x += 1;
        current_line_index++;
      }
    } else if (cursor_x == COLS-1) {
      if (NUM_LINES(current_line_node) > 1) {
        // Need to drop down to the next screen line!
        cursor_x = 0;
        cursor_y += 1;
      }
    }
  }

  if (y == -1) {
    if (cursor_y > 0) {
      cursor_y -= 1;
      if (NUM_LINES(current_line_node) > 0 && current_line_index >= COLS) {
        current_line_index -= COLS;
      } else {
        current_line_node = current_line_node->prev;
        current_line_index = (int)fmin(NUM_CHARS(current_line_node), current_line_index);
      }
    } else if (cursor_y == 0) {
      scroll_up();
    }
  } else if (y == 1) {
    if (cursor_y < LINES-1-1) {
      cursor_y += 1;
      if (NUM_LINES(current_line_node) > 1 && current_line_index < (COLS * (NUM_LINES(current_line_node)-1))) {
        current_line_index = (int)fmin(NUM_CHARS(current_line_node), current_line_index+COLS);
      } else {
        current_line_node = current_line_node->next;
        current_line_index = (int)fmin(NUM_CHARS(current_line_node), current_line_index);
      }
    } else if (cursor_y == LINES-1-1) {
      scroll_down();
    }
  }

  return k_ok;
}

ErrorCode enter_insert_mode() {
  mode = insert_mode;
  return k_ok;
}

ErrorCode ncurses_process_input() {
  update_screen();
  wchar_t c = '\0';
  while (c != 'q') {
    c = getch();
    if (mode == insert_mode) {
      // TODO Type characters into buffer
      continue;
    }
    switch(c) {
      case 'c':
        break;
      case 'i': // Enter insert mode
        enter_insert_mode();
      case KEY_LEFT:
      case 'h': // Left
        update_cursor(-1, 0);
        break;
      case KEY_RIGHT:
      case 'l': // Right
        update_cursor(1, 0);
        break;
      case KEY_UP:
      case 'k': // Up
        update_cursor(0, -1);
        break;
      case KEY_DOWN:
      case 'j': // Down
        update_cursor(0, 1);
        break;
      default:
        break;
    }
    update_screen();
  }
  return k_ok;
}

ErrorCode init(char *filename) {
  // Create a new buffer, for changes made
  new = calloc((MAX_BUF_LEN + 0), sizeof(char));
  if (new == NULL) {
    return k_out_of_memory;
  }
  new[MAX_BUF_LEN] = '\0';

  // Initialize the linked list
  ErrorCode status = initialize(&head_sentinel, &tail_sentinel);
  if (status != k_ok) {
    // Couldn't create new linked list!
    printf("%s\n", "Couldn't create new list");
    return status;
  }

  // Read the file in to memory
  status = readFile(filename);
  if (status != k_ok) {
    printf("Something went wrong reading the file: %d\n", status);
    return status;
  }

  // Start curses mode
  status = ncurses_init();
  if (status != k_ok) {
    return status;
  }

  return k_ok;
}

int main(int argc, char **argv) {
  char *filename;
  ErrorCode status = checkArgs(argc, argv, &filename);
  if (status != k_ok) {
    return status;
  }

  status = init(filename);
  if (status != k_ok) {
    return status;
  }


  printw(original);  /* Print Hello World      */
  refresh();      /* Print it on to the real screen */
  ncurses_process_input();
  endwin();     /* End curses mode      */

  (void)new;

  return 0;
}
