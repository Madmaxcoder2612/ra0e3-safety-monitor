/* Host unit tests for the safety core. No framework, plain asserts.
 * The .c file is included so the tests can corrupt the private state, which is
 * exactly what a bit flip would do. */
#include <stdio.h>
#include <string.h>

#include "../src/sm_core.c"
#include "../src/sm_crc32.c"
#include "../src/sm_protected.c"

static int g_actuator = -1;   /* last commanded level, -1 = never commanded */
static int g_off_calls = 0;
static int g_failures = 0;
static int g_tests = 0;

void sm_port_actuator_set(int on)
{
    g_actuator = on;
    if (!on) {
        g_off_calls++;
    }
}

#define CHECK(cond)                                                          \
    do {                                                                     \
        g_tests++;                                                           \
        if (!(cond)) {                                                       \
            g_failures++;                                                    \
            printf("  FAIL line %d: %s\n", __LINE__, #cond);                 \
        }                                                                    \
    } while (0)

static void reset_port(void)
{
    g_actuator = -1;
    g_off_calls = 0;
}

/* ---- CRC-32 ---- */
static void test_crc(void)
{
    const uint8_t check[] = "123456789";
    const uint8_t a[] = "12345";
    const uint8_t b[] = "6789";
    uint32_t s;

    printf("crc32\n");
    CHECK(sm_crc32(check, 9) == 0xCBF43926u);          /* standard check value */
    CHECK(sm_crc32((const uint8_t *)"", 0) == 0u);     /* empty input */

    s = sm_crc32_init();                                /* sliced == whole */
    s = sm_crc32_update(s, a, 5);
    s = sm_crc32_update(s, b, 4);
    CHECK(sm_crc32_final(s) == 0xCBF43926u);
}

/* ---- protected variable ---- */
static void test_protected(void)
{
    sm_protected_u32 p;
    uint32_t v = 0;
    int bit;

    printf("protected variable\n");
    sm_protected_write(&p, 0x12345678u);
    CHECK(sm_protected_read(&p, &v) == SM_PROTECTED_OK && v == 0x12345678u);

    /* Every single-bit flip in either copy must be detected. */
    for (bit = 0; bit < 32; bit++) {
        sm_protected_write(&p, 0x12345678u);
        p.value ^= (1u << bit);
        v = 0xDEADBEEFu;
        CHECK(sm_protected_read(&p, &v) == SM_PROTECTED_CORRUPT);
        CHECK(v == 0xDEADBEEFu);                        /* no unverified value out */

        sm_protected_write(&p, 0x12345678u);
        p.inverse ^= (1u << bit);
        CHECK(sm_protected_read(&p, &v) == SM_PROTECTED_CORRUPT);
    }

    /* Known limit: both copies changed consistently is NOT detected. */
    sm_protected_write(&p, 5u);
    p.value = 9u;
    p.inverse = ~9u;
    CHECK(sm_protected_read(&p, &v) == SM_PROTECTED_OK && v == 9u);
}

/* ---- safety core ---- */
static void test_core(void)
{
    static const uint8_t img[] = "firmware image bytes";
    uint32_t good = sm_crc32(img, sizeof img);

    printf("safety core\n");

    /* start: actuator off, no fault */
    reset_port();
    sm_init();
    CHECK(g_actuator == 0);
    CHECK(sm_get_state() == SM_STATE_SAFE);
    CHECK(sm_get_fault() == SM_FAULT_NONE);

    /* a good image allows running */
    CHECK(sm_check_image(img, sizeof img, good) == 1);
    CHECK(sm_request_run() == 1);
    CHECK(g_actuator == 1 && sm_get_state() == SM_STATE_RUN);

    /* a runtime fault switches off, latches, and blocks running */
    sm_report_fault(SM_FAULT_DATA_CORRUPT);
    CHECK(g_actuator == 0);
    CHECK(sm_get_state() == SM_STATE_SAFE);
    CHECK(sm_get_fault() == SM_FAULT_DATA_CORRUPT);
    CHECK(sm_request_run() == 0);
    CHECK(g_actuator == 0);

    /* first fault is kept */
    sm_report_fault(SM_FAULT_FLASH_CRC);
    CHECK(sm_get_fault() == SM_FAULT_DATA_CORRUPT);

    /* a bad image is a fault and the actuator never runs */
    reset_port();
    sm_init();
    CHECK(sm_check_image(img, sizeof img, good ^ 1u) == 0);
    CHECK(sm_get_fault() == SM_FAULT_FLASH_CRC);
    CHECK(sm_request_run() == 0);
    CHECK(g_actuator == 0);

    /* one flipped bit in the image is caught */
    {
        uint8_t copy[sizeof img];
        memcpy(copy, img, sizeof img);
        copy[7] ^= 0x04;
        reset_port();
        sm_init();
        CHECK(sm_check_image(copy, sizeof copy, good) == 0);
    }

    /* the actuator is switched off BEFORE the fault is recorded */
    reset_port();
    sm_init();
    (void)sm_request_run();
    reset_port();
    sm_report_fault(SM_FAULT_DATA_CORRUPT);
    CHECK(g_off_calls >= 1);
}

/* ---- corruption of the core's own state ---- */
static void test_core_self_corruption(void)
{
    printf("core state corruption\n");

    /* flipped bit in the state: reads as SAFE, never as RUN */
    reset_port();
    sm_init();
    (void)sm_request_run();
    g_state.value ^= 1u;
    CHECK(sm_get_state() == SM_STATE_SAFE);

    /* a stray value that is consistent but unknown also reads as SAFE */
    sm_protected_write(&g_state, 77u);
    CHECK(sm_get_state() == SM_STATE_SAFE);

    /* corrupt fault word: reported as a fault, so running is refused */
    sm_init();
    g_fault.inverse ^= 0x10u;
    CHECK(sm_get_fault() != SM_FAULT_NONE);
    CHECK(sm_request_run() == 0);
    CHECK(g_actuator == 0);
}

int main(void)
{
    test_crc();
    test_protected();
    test_core();
    test_core_self_corruption();

    printf("\n%d checks, %d failed\n", g_tests, g_failures);
    return g_failures ? 1 : 0;
}
