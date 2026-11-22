#include "sm_protected.h"

void sm_protected_write(sm_protected_u32 *p, uint32_t value)
{
    p->value = value;
    p->inverse = ~value;
}

sm_protected_status sm_protected_read(const sm_protected_u32 *p, uint32_t *value)
{
    if ((p->value ^ p->inverse) != 0xFFFFFFFFu) {
        return SM_PROTECTED_CORRUPT;
    }
    *value = p->value;
    return SM_PROTECTED_OK;
}
