#include "sm_core.h"
#include "sm_crc32.h"
#include "sm_protected.h"

/* State is kept as protected variables so a bit flip cannot silently turn
 * the safe state into RUN. */
static sm_protected_u32 g_state;
static sm_protected_u32 g_fault;

void sm_init(void)
{
    sm_port_actuator_set(0);
    sm_protected_write(&g_state, (uint32_t)SM_STATE_SAFE);
    sm_protected_write(&g_fault, (uint32_t)SM_FAULT_NONE);
}

/* Read the fault code. If its own storage is corrupt, treat that as a data
 * fault: the answer is then "a fault exists". */
sm_fault sm_get_fault(void)
{
    uint32_t v;

    if (sm_protected_read(&g_fault, &v) != SM_PROTECTED_OK) {
        return SM_FAULT_DATA_CORRUPT;
    }
    return (sm_fault)v;
}

sm_state sm_get_state(void)
{
    uint32_t v;

    /* Unreadable or unknown state counts as SAFE. */
    if (sm_protected_read(&g_state, &v) != SM_PROTECTED_OK || v != (uint32_t)SM_STATE_RUN) {
        return SM_STATE_SAFE;
    }
    return SM_STATE_RUN;
}

void sm_report_fault(sm_fault fault)
{
    /* Switch off first. Recording the fault comes second on purpose. */
    sm_port_actuator_set(0);

    if (sm_get_fault() == SM_FAULT_NONE) {
        sm_protected_write(&g_fault, (uint32_t)fault);
    }
    sm_protected_write(&g_state, (uint32_t)SM_STATE_SAFE);
}

int sm_request_run(void)
{
    if (sm_get_fault() != SM_FAULT_NONE) {
        sm_port_actuator_set(0);
        return 0;
    }
    sm_protected_write(&g_state, (uint32_t)SM_STATE_RUN);
    sm_port_actuator_set(1);
    return 1;
}

int sm_check_image(const uint8_t *image, uint32_t length, uint32_t expected_crc)
{
    if (sm_crc32(image, length) != expected_crc) {
        sm_report_fault(SM_FAULT_FLASH_CRC);
        return 0;
    }
    return 1;
}
