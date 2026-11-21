# RA0E3 Safety Monitor

A functional-safety learning project on the Renesas **FPB-RA0E3** board
(RA0E3 MCU, Arm Cortex-M23, 32 MHz, 16 KB flash, 2 KB RAM).

> **Not certified.** This is a learning and portfolio project that follows
> IEC 61508 / IEC 60730 Class B *patterns*. It is not certified, not assessed
> by any third party, and must not be used in a real safety-related product.

![FPB-RA0E3 running the Blink baseline](media/blink_board.jpg)

Demo video of the Blink baseline: [media/blink_demo.mp4](media/blink_demo.mp4)

## Idea

A blinking "actuator" LED is supervised by safety mechanisms. When a fault is
detected, the actuator is switched off (safe state) and a fault code is shown on
the second LED.

## Status

Early stage. Nothing below is implemented yet.

| Step | State |
|---|---|
| Board bring-up (stock Blink project, 2 LEDs, button S1) | done, see [docs/bringup.md](docs/bringup.md) |
| Hazard analysis (first draft, in my own words) | started, [docs/hazard_analysis.md](docs/hazard_analysis.md) |
| Safety requirements | SR-01 drafted, [docs/requirements.md](docs/requirements.md) |
| Peripheral confirmation from hardware manual (IWDT, CRC, clock monitor) | open |
| Safety mechanisms, fault injection, CI | not started |

## Repository layout

- `Blink_Project/`: e2 studio / FSP project (generated Renesas files in `ra/`, `ra_cfg/`, `ra_gen/` are not hand-edited)
- `docs/`: hazard analysis, requirements, design decisions
- `media/`: photos and video of the board (own recordings)

## Honest limitations

- Not certified (see above).
- No external hardware cutoff (breaker or fuse equivalent). The watchdog is the only independent trip mechanism.
- The 1 s fault tolerant time is an assumption, not derived from a real machine.

## License

MIT for original work, see [LICENSE](LICENSE). Renesas FSP files keep their own licenses.
