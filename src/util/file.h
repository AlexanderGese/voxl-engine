#ifndef UTIL_FILE_H
#define UTIL_FILE_H

#include <stddef.h>

// reads whole file into a malloc'd buffer. null terminated. caller frees.
// returns NULL on failure.
char *file_read_all(const char *path, size_t *out_size);

// writes buffer to file. returns 0 on success.
int file_write_all(const char *path, const void *data, size_t size);

// returns 1 if path exists
int file_exists(const char *path);

#endif
