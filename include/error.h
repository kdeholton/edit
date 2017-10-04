#ifndef _ERROR_H_
#define _ERROR_H_

typedef enum ErrorCode {
  k_ok = 0,          // Everything is fine.
  k_null_ptr,        // Null pointer
  k_bad_param,       // Illegal argument to edit
  k_out_of_memory,   // We ran out of memory
  k_file_open_error, // Couldn't open the file
  k_file_error,      // Generic file error
} ErrorCode;

#endif
