/* CRC-32 (IEEE 802.3, reflected, polynomial 0xEDB88320) in plain C.
 *
 * Intended use: start-up check of the flash image against a stored value.
 * Bitwise, no lookup table, so it costs no RAM and very little flash on a
 * 16 KB part. It is slower than a table or a hardware unit. */
#ifndef SM_CRC32_H
#define SM_CRC32_H

#include <stddef.h>
#include <stdint.h>

/* CRC of a whole buffer. Check value: "123456789" gives 0xCBF43926. */
uint32_t sm_crc32(const uint8_t *data, size_t length);

/* Incremental form, so a long region can be checked in slices.
 * Start with sm_crc32_init(), feed slices, finish with sm_crc32_final(). */
uint32_t sm_crc32_init(void);
uint32_t sm_crc32_update(uint32_t state, const uint8_t *data, size_t length);
uint32_t sm_crc32_final(uint32_t state);

#endif
