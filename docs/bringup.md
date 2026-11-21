# Board bring-up notes

Stock FSP "Blinky" project for the FPB-RA0E3.

## Observed on the board

- The documentation of the example says it blinks all LEDs. On this board I see **two** LEDs.
- Button: **S1** (user switch).
- Watchdog and clock monitor are not visible on the board or the quick start guide. They are on-chip blocks and must be looked up in the RA0E3 hardware user's manual.

## Confirmed from the project files

| Item | Value | Source |
|---|---|---|
| LED1 | P010 | `Blink_Project/ra/board/ra0e3_fpb/board_leds.c` |
| LED2 | P011 | same file |
| Flash | 16384 bytes | `ra_cfg/fsp_cfg/bsp/bsp_mcu_device_pn_cfg.h` |
| RAM | 2048 bytes | same file |
| Usable flash | 15296 bytes (starts at 0x440) | `Debug/Blink_Project.map` (linker script) |

The stock example blinks every entry of `g_bsp_leds`, and this board defines two, which matches what I see.

## Open, to confirm in the hardware manual

- Which pin S1 uses (P012 or P013 are configured as inputs).
- IWDT: FSP marks it available (own 16.384 kHz clock). Confirm in the manual.
- CRC calculator: FSP marks it available. Confirm in the manual.
- Clock frequency monitor (CAC): not found by me in the manual or FSP feature file. Treat as missing until a search of the manual for "CAC", "Clock Frequency Accuracy" and "oscillation stop" is done.

Media: see `media/` (photo and video of the running board).
