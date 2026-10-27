#ifndef PINS_H
#define PINS_H
/* CH32V003 GPIO names; verify package pad mapping against WCH datasheet. */
#define PIN_UART_TX_PORT GPIOD
#define PIN_UART_TX GPIO_Pin_1
#define PIN_UART_RX_PORT GPIOD
#define PIN_UART_RX GPIO_Pin_6
#define PIN_TEST_PORT GPIOC
#define PIN_TEST GPIO_Pin_4
#define CAPTURE_HZ 10000u
#define CAPTURE_SAMPLES 200u
#endif
