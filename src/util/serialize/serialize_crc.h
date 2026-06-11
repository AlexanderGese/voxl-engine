#ifndef UTIL_SERIALIZE_CRC_H
#define UTIL_SERIALIZE_CRC_H

#include <stddef.h>
#include <stdint.h>

// crc32, the ieee/zlib polynomial (0xEDB88320 reflected). same one zlib uses
// so anything external can validate our blobs without our table.
//
// the table is built lazily on first use. not thread safe but neither is the
// rest of the save path, and the build is idempotent so a race just does the
// work twice and lands on the same bytes.

uint32_t serialize_crc32(const void *data, size_t len);

// streaming variant. seed with serialize_crc32_begin(), feed chunks, finish.
uint32_t serialize_crc32_begin(void);
uint32_t serialize_crc32_update(uint32_t crc, const void *data, size_t len);
uint32_t serialize_crc32_final(uint32_t crc);

#endif
