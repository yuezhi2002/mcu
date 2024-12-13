#ifndef __UART_INIT_H
#define __UART_INIT_H

extern UART_HandleTypeDef uart1;

void uart1_init(uint32_t baudrate);//PA9 USART1_tx ;PA10 USART1_rx

void uart2_init();//PA2 USART2_tx ;PA3 USART2_rx

void uart3_init();//PB10 USART3_tx ;PB11 USART3_rx
#endif