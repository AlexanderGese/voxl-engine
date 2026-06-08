#include "file.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

char *file_read_all(const char *path, size_t *out_size) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        LOGW("file_read_all: cant open %s", path);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (n < 0) { fclose(f); return NULL; }

    char *buf = malloc((size_t)n + 1);
    if (!buf) { fclose(f); return NULL; }

    size_t got = fread(buf, 1, (size_t)n, f);
    fclose(f);
    if (got != (size_t)n) {
        free(buf);
        return NULL;
    }
    buf[n] = 0;
    if (out_size) *out_size = (size_t)n;
    return buf;
}

int file_write_all(const char *path, const void *data, size_t size) {
    FILE *f = fopen(path, "wb");
    if (!f) return -1;
    size_t w = fwrite(data, 1, size, f);
    fclose(f);
    return (w == size) ? 0 : -1;
}

int file_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0;
}
