/* Safety core: the state that decides whether the actuator may run.
 *
 * Rules (they come from SR-01 and the safe state in docs/hazard_analysis.md):
 *  - After sm_init() the actuator is OFF and stays off until sm_request_run()
 *    is accepted.
 *  - sm_report_fault() always switches the actuator off first, then records
 *    the fault code.
 *  - A fault is latched. Only a reset clears it, there is no clear function.
 *  - The first fault code is kept, later ones do not overwrite it.
 *
 * Hardware is reached through two functions the port must provide. On the
 * board they will drive the LED pins, in the host tests they are fakes. */
#ifndef SM_CORE_H
#define SM_CORE_H

#include <stdint.h>

typedef enum {
    SM_FAULT_NONE = 0,
    SM_FAULT_FLASH_CRC = 1,    /* start-up image check failed */
    SM_FAULT_DATA_CORRUPT = 2  /* a protected variable disagreed with itself */
} sm_fault;

typedef enum {
    SM_STATE_SAFE = 0,  /* actuator off, no run allowed */
    SM_STATE_RUN = 1    /* actuator may be on */
} sm_state;

/* Port functions (provided outside this module). */
void sm_port_actuator_set(int on);

void sm_init(void);

/* Ask to run. Refused (returns 0) while a fault is latched. */
int sm_request_run(void);

/* Switch the actuator off, latch the fault, enter the safe state. */
void sm_report_fault(sm_fault fault);

sm_state sm_get_state(void);
sm_fault sm_get_fault(void);

/* Convenience: compare an image against the expected CRC-32 and report
 * SM_FAULT_FLASH_CRC on mismatch. Returns 1 when it matches. */
int sm_check_image(const uint8_t *image, uint32_t length, uint32_t expected_crc);

#endif
