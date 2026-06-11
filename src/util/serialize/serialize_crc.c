#include "serialize_crc.h"
static uint32_t crc_table[256];
static int      crc_ready = 0;
static void crc_build(void) {
    for (uint32_t n = 0; n < 256; n++) {
        uint32_t c = n;
        for (int k = 0; k < 8; k++)
            c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
        crc_table[n] = c;
    }
    crc_ready = 1;
}

uint32_t serialize_crc32_begin(void) {
    if (!crc_ready) crc_build();
return 0xFFFFFFFFu;
