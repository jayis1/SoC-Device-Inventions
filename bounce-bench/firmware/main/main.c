#include "ch32v00x.h"
#include "pins.h"
#include <stdint.h>
#include <stdio.h>

static volatile uint8_t samples[CAPTURE_SAMPLES];
static volatile uint16_t sample_count;
static volatile uint8_t armed, capturing;
static volatile uint32_t arm_ticks;

/* 10 kHz timer ISR: bound capture and stop after 20 ms. */
void TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == RESET) return;
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    if (armed && !capturing) {
        if (GPIO_ReadInputDataBit(PIN_TEST_PORT, PIN_TEST) == Bit_RESET) {
            capturing = 1; sample_count = 0;
        } else if (++arm_ticks >= 1000u) armed = 0;
    }
    if (capturing && sample_count < CAPTURE_SAMPLES) {
        samples[sample_count++] = (uint8_t)GPIO_ReadInputDataBit(PIN_TEST_PORT, PIN_TEST);
        if (sample_count == CAPTURE_SAMPLES) { capturing = 0; armed = 0; }
    }
}

static void setup(void) {
    GPIO_InitTypeDef g = {0}; TIM_TimeBaseInitTypeDef t = {0}; NVIC_InitTypeDef n = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC | RCC_APB2Periph_USART1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    g.GPIO_Pin = PIN_TEST; g.GPIO_Mode = GPIO_Mode_IPU; g.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PIN_TEST_PORT, &g);
    g.GPIO_Pin = PIN_UART_TX; g.GPIO_Mode = GPIO_Mode_AF_PP; GPIO_Init(PIN_UART_TX_PORT, &g);
    g.GPIO_Pin = PIN_UART_RX; g.GPIO_Mode = GPIO_Mode_IN_FLOATING; GPIO_Init(PIN_UART_RX_PORT, &g);
    USART_InitTypeDef u = {0}; u.USART_BaudRate = 115200; u.USART_WordLength = USART_WordLength_8b;
    u.USART_StopBits = USART_StopBits_1; u.USART_Parity = USART_Parity_No;
    u.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; u.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &u); USART_Cmd(USART1, ENABLE);
    /* Assumes vendor system clock configured to 48 MHz. */
    t.TIM_Period = 4799; t.TIM_Prescaler = 0; t.TIM_ClockDivision = TIM_CKD_DIV1; t.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &t); TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    n.NVIC_IRQChannel = TIM2_IRQn; n.NVIC_IRQChannelPreemptionPriority = 1; n.NVIC_IRQChannelSubPriority = 0; n.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&n); TIM_Cmd(TIM2, ENABLE);
}
static void putchar_uart(char c) { while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {} USART_SendData(USART1, (uint16_t)c); }
static void puts_uart(const char *s) { while (*s) putchar_uart(*s++); }
int main(void) {
    SystemCoreClockUpdate(); setup(); puts_uart("Bounce Bench ready; send T\r\n");
    for (;;) {
        if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET) {
            char c = (char)USART_ReceiveData(USART1);
            if (c == 'T' || c == 't') {
                __disable_irq(); sample_count = 0; arm_ticks = 0; capturing = 0; armed = 1; __enable_irq();
                puts_uart("armed\r\n");
                while (armed || capturing) {}
                puts_uart("t_ms,level\r\n");
                uint16_t last = 0; int prev = 1;
                for (uint16_t i=0; i<sample_count; ++i) if (samples[i] != prev) {
                    char line[32]; snprintf(line, sizeof line, "%u,%u\r\n", (unsigned)i/10u, samples[i]); puts_uart(line);
                    last = i; prev = samples[i];
                }
                char summary[40]; snprintf(summary, sizeof summary, "summary,%u,%u\r\n", sample_count, last/10u); puts_uart(summary);
            }
        }
    }
}
