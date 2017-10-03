#include "edit.h"

// Private Variables
static char *original;          // This buffer is read-only
static char *new;               // This buffer is append-only
static span_t *head_sentinel;
static span_t *tail_sentinel;
static uint8_t cursor_x;
static uint8_t cursor_y;
EditorMode mode;
static uint32_t top_line;

void printUsage() {
  printf("Usage: edit <filename>\n");
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

	/* Go to the end of the file. */
	if (fseek(fp, 0L, SEEK_END) == 0) {
		/* Get the size of the file. */
		long bufsize = ftell(fp);
		if (bufsize == -1) {
      return k_file_error;
    }

		/* Allocate our buffer to that size. */
    // TODO Size limiting for large files
    // To start: max of this and some constant?
		original = malloc(sizeof(char) * (bufsize + 1));
    if (original == NULL) {
      // Can't allocate that large
      fclose(fp);
      return k_out_of_memory;
    }

		/* Go back to the start of the file. */
		if (fseek(fp, 0L, SEEK_SET) != 0) {
      fclose(fp);
      return k_file_error;
    }

		/* Read the entire file into memory. */
		size_t newLen = fread(original, sizeof(char), bufsize, fp);
		if ( ferror( fp ) != 0 ) {
      fclose(fp);
      return k_file_error;
		} else {
      original[newLen++] = '\0'; /* Just to be safe. */
		}
	}
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

  return k_ok;
}

ErrorCode scroll_up() {
  // TODO
  if (top_line == 0) {
    return k_ok;
  }
  scrollok(stdscr, true); // Enable scrolling
  scrl(-1);
  top_line--;
  return k_ok;
}

ErrorCode scroll_down() {
  // TODO
  scrollok(stdscr, true); // Enable scrolling
  scrl(1);
  top_line++;
  return k_ok;
}

ErrorCode update_cursor(int8_t x, int8_t y) {
  if (x == -1) {
    if (cursor_x > 0) {
      cursor_x -= 1;
    }
  } else if (x == 1) {
    if (cursor_x < COLS-1) {
      cursor_x += 1;
    }
  }

  if (y == -1) {
    if (cursor_y > 0) {
      cursor_y -= 1;
    } else if (cursor_y == 0) {
      scroll_up();
    }
  } else if (y == 1) {
    if (cursor_y < LINES-1) {
      cursor_y += 1;
    } else if (cursor_y == LINES-1) {
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
  move(cursor_y, cursor_x);
  wchar_t c = '\0';
  while (c != 'q') {
    c = getch();
    if (mode == insert_mode) {
      // TODO Type characters into buffer
      continue;
    }
    switch(c) {
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
    move(cursor_y, cursor_x);
    refresh();
  }
  return k_ok;
}

ErrorCode init(char *filename) {
  // Read the file in to memory
  ErrorCode status = readFile(filename);
  if (status != k_ok) {
    printf("Something went wrong reading the file: %d\n", status);
    return status;
  }

  // Create a new buffer, for changes made
  new = calloc((MAX_BUF_LEN + 0), sizeof(char));
  if (new == NULL) {
    return k_out_of_memory;
  }
  new[MAX_BUF_LEN] = '\0';

  // Initialize the linked list
  status = initialize(&head_sentinel, &tail_sentinel);
  if (status != k_ok) {
    // Couldn't create new linked list!
    printf("%s\n", "Couldn't create new list");
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
