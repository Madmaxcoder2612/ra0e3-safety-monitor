# Decision log

Newest at the bottom. Records what was decided and by whom.

## 2026-11-21

- Scope: demo firmware with supervised blinking actuator LED. Not certified.
- Hardware facts confirmed from project files: 2 LEDs (P010, P011), 16 KB flash, 2 KB RAM.
- Button is S1 (observed by me on the board).
- Safe state = actuator off (my decision), plus fault code on LED2. Red/green indication rejected as primary safe state because it depends on possibly failed software.
- Fault tolerant time 1 s (my assumption, reasoning: people react slower than machines).
- Independent cutoff idea (fuse/breaker, mine) maps to the IWDT watchdog.
- Environment/obstacle interlock is out of scope (no sensors), recorded as assumption A-02.
- Clock monitor not found in the manual by me: assume missing, use a software check, confirm by a final search.
