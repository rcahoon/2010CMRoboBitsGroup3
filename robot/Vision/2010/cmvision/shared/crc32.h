#ifndef ROBOCUP2010_CRC32_H
#define ROBOCUP2010_CRC32_H

#include <stdint.h>

namespace RoboCup2010 {

// extern const uint32_t crc32_table[256];

/* Return a 32-bit CRC of the contents of the buffer. */
uint32_t CalcCRC32(uint32_t val, const void *ss, int len);

} // end namespace

#endif
