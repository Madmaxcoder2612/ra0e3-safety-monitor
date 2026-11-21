# Hazard analysis (first draft)

Written by me, refined in discussion. Status: draft.

## H-01: Actuator keeps running when the software has failed

- **Cause:** software crashes, hangs, or data is corrupted.
- **Hazard:** the actuator keeps running with nobody controlling it.
- **Harm:** in industrial automation this can injure a person or damage equipment.
- **Key point:** the dangerous case is the *undetected* failure. A crash that is detected and answered by switching the actuator off is acceptable.

## Safe state

Actuator output off. A fault code is blinked on the second LED. The status indication is informational only and the safety does not depend on it, because it is driven by the same software that may have failed.

## Independent cutoff

My idea: something like a fuse or circuit breaker that trips independently of the software. On this board the closest thing is the independent watchdog (IWDT), which has its own clock. A real machine would add an external hardware cutoff. This demo does not have one.

## Assumptions

- **A-01:** fault tolerant time is 1 s (detection plus reaction). Assumption, not derived from a real machine.
- **A-02:** whether the machine surroundings are clear (no obstacles, field free) is the user's responsibility. The demo has no sensors for this.

## Open

- Does the watchdog reset leave the LED pins off? (check manual)
- Can the IWDT be locked on so software cannot disable it? (check manual)
