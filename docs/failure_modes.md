# Failure modes of the first mechanisms

Status: draft, written for the host-tested code in `safety/`. A failure-mode entry says how a mechanism can itself fail, so it is not trusted blindly. Nothing here has been run on the board yet.

Hazard being addressed: [H-01](hazard_analysis.md), the actuator keeps running although the software has failed. Requirement: [SR-01](requirements.md).

## M-01: Flash image check (`sm_crc32`, `sm_check_image`)

Idea: at start-up, compute CRC-32 over the program image and compare it with a stored value. A mismatch means a fault and the actuator never starts.

| How it can fail | Effect | Covered by | Left open |
|---|---|---|---|
| The CRC routine has a bug | A wrong image passes or a good image fails | Standard check value `"123456789"` = `0xCBF43926`, sliced equals whole, a single flipped bit is caught (tests) | Only tested on the host, not on the Cortex-M23 |
| The expected value is stored in the same flash it protects | Corruption of the stored value looks like a bad image (safe direction) | The failure leads to the safe state | Where the value lives and how it is written is not decided |
| The check is skipped, for example by a jump over it | A bad image runs | Not covered | Needs program-flow monitoring (not implemented) |
| The check only runs at start-up | Corruption that appears later is missed | Not covered | Needs periodic or sliced checking. The fault tolerant time of SR-01 decides how often |
| CRC-32 cannot detect every error | Some multi-bit patterns pass | Not covered | Accepted residual risk, quantifying it is future work |

## M-02: Protected variable (`sm_protected`)

Idea: store a value and its bit inverse. A read compares the two.

| How it can fail | Effect | Covered by | Left open |
|---|---|---|---|
| One copy gets a bit flip | The copies disagree | All 64 single-bit flips (32 per copy) are detected in the test | None at this level |
| A write updates both copies consistently, for example a runaway pointer that happens to write the pair | Not detected | The test documents this as a known limit | Needs memory protection or a separate check, not available in this layer |
| Both copies are corrupted by the same event | Not detected if they stay consistent | Not covered | Place the copies apart in RAM, decide with the linker layout |

## M-03: Safety core state (`sm_core`)

Idea: one place decides if the actuator may run. A fault switches the actuator off first, is latched, and cannot be cleared except by reset.

| How it can fail | Effect | Covered by | Left open |
|---|---|---|---|
| The state word flips to RUN by a bit error | The actuator thinks it may run | The state is a protected variable. A flipped bit reads as SAFE (test) | A consistent write of RUN is not detected |
| The fault word is corrupted | The fault could read as "none" | Corruption reads as a fault, so running is refused (test) | None at this level |
| `sm_report_fault` is never called because the software hung | The actuator keeps running | Not covered by this module | This is the case for the independent watchdog, which is still to be confirmed and built |
| The port function that drives the pin fails, or the pin is stuck | The actuator stays on | Not covered | Needs read-back of the output or an external cutoff, see "Independent cutoff" in the hazard analysis |
| A second fault arrives after the first | The second code is lost | The first code is kept on purpose (test) | A fault history is not kept |

## What this does not show

- No statement about detection time, diagnostic coverage or PFH. No numbers have been measured or derived.
- The tests run on a PC compiler, so they say nothing about timing, memory use or behaviour on the board.
- The code is not yet part of the firmware. `Blink_Project/` is still the stock example.
