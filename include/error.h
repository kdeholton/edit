#ifndef _ERROR_H_
#define _ERROR_H_

typedef enum ErrorCode {
  k_ok = 0,          // Everything is fine.
  k_out_of_memory,   // We ran out of memory
} ErrorCode;

#endif
