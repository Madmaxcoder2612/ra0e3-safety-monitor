/* generated configuration header file - do not edit */
#ifndef BSP_PIN_CFG_H_
#define BSP_PIN_CFG_H_
#include "r_ioport.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

#define LED1 (BSP_IO_PORT_00_PIN_10) /* GREEN_ARDUINO_AREF_A5 */
#define LED2 (BSP_IO_PORT_00_PIN_11) /* GREEN_ARDUINO_D13_A4 */
#define PMOD_GPIO8 (BSP_IO_PORT_00_PIN_12) /* ARDUINO_D12_A3 */
#define PMOD_GPIO9_ARDUINO_A2 (BSP_IO_PORT_00_PIN_13) /* ARDUINO_D8 */
#define ARDUINO_D11 (BSP_IO_PORT_01_PIN_00) /* PMOD_MISO_RXD_ARDUINO_A1_VCOM_RXD */
#define PMOD_RESET (BSP_IO_PORT_01_PIN_01) /* PMOD_MOSI_TXD_ARDUINO_D10_A0_VCOM_TXD */
#define ARDUINO_D9 (BSP_IO_PORT_01_PIN_02) /* PMOD_SCK_GPIO4 */
#define PMOD_GPIO10_ARDUINO_D4 (BSP_IO_PORT_01_PIN_09)
#define ARDUINO_D5 (BSP_IO_PORT_01_PIN_10)
#define PMOD_GPIO7_INT_ARDUINO_D6 (BSP_IO_PORT_01_PIN_12)
#define SW (BSP_IO_PORT_02_PIN_00) /* ARDUINO_D2 */
#define PMOD_INT_GPIO1_ARDUINO_D3 (BSP_IO_PORT_02_PIN_01)
#define RES (BSP_IO_PORT_02_PIN_06) /* ARDUINO_RESET */
#define PMOD_SCL_ARDUINO_SCL (BSP_IO_PORT_02_PIN_12) /* ARDUINO_RX */
#define PMOD_SDA_ARDUINO_SDA (BSP_IO_PORT_02_PIN_13) /* ARDUINO_RX */

#define PIN_SDAA0 (BSP_IO_PORT_02_PIN_13)
#define CFG_SDAA0 ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_PERIPHERAL_IICA5)

#define PIN_SCLA0 (BSP_IO_PORT_02_PIN_12)
#define CFG_SCLA0 ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_PERIPHERAL_IICA5)

extern const ioport_cfg_t g_bsp_pin_cfg; /* RA0E3 FPB */

void BSP_PinConfigSecurityInit();

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER
#endif /* BSP_PIN_CFG_H_ */
