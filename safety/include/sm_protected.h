/* Protected variable: the value is stored twice, once as is and once
 * bit-inverted. A single flipped bit, or a stray write to one copy, makes the
 * two disagree and is detected on the next read.
 *
 * Limits: it detects corruption of one copy. It does not detect a write that
 * updates both copies consistently, so it does not replace access control. */
#ifndef SM_PROTECTED_H
#define SM_PROTECTED_H

#include <stdint.h>

typedef struct {
    uint32_t value;
    uint32_t inverse;
} sm_protected_u32;

typedef enum {
    SM_PROTECTED_OK = 0,
    SM_PROTECTED_CORRUPT = 1
} sm_protected_status;

void sm_protected_write(sm_protected_u32 *p, uint32_t value);

/* Returns SM_PROTECTED_CORRUPT and leaves *value untouched when the copies
 * disagree. Never returns a value it could not verify. */
sm_protected_status sm_protected_read(const sm_protected_u32 *p, uint32_t *value);

#endif
