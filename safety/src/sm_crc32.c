#include "sm_crc32.h"

uint32_t sm_crc32_init(void)
{
    return 0xFFFFFFFFu;
}

uint32_t sm_crc32_update(uint32_t state, const uint8_t *data, size_t length)
{
    size_t i;
    int bit;

    for (i = 0; i < length; i++) {
        state ^= data[i];
        for (bit = 0; bit < 8; bit++) {
            if (state & 1u) {
                state = (state >> 1) ^ 0xEDB88320u;
            } else {
                state >>= 1;
            }
        }
    }
    return state;
}

uint32_t sm_crc32_final(uint32_t state)
{
    return state ^ 0xFFFFFFFFu;
}

uint32_t sm_crc32(const uint8_t *data, size_t length)
{
    return sm_crc32_final(sm_crc32_update(sm_crc32_init(), data, length));
}
