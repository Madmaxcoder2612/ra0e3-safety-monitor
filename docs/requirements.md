# Safety requirements

| ID | Requirement | Status |
|---|---|---|
| SR-01 | When any runtime fault is detected, the system shall switch the actuator output off and enter the safe state within 1 s of the fault occurring. | draft, from H-01 and A-01 |
| SR-02 | (to be written by me: what must be true before the actuator is allowed to run, and what happens if a start-up check fails) | open |

Later requirements (watchdog, program flow, stack canary, protected variables, fault reporting) follow after SR-02.
