#ifndef __UART_INIT_H
#define __UART_INIT_H

#include "stm32f1xx_hal.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "stm32f1xx_it.h"
#include "stdint.h"

#define RX_BUFFER_SIZE 128

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern uint8_t rxBuffer[RX_BUFFER_SIZE];
extern volatile uint16_t rxIndex;
extern uint16_t temp;
extern uint16_t humi;
extern uint16_t ppm;

void MX_USART1_UART_Init(void);
void HAL_UART_MspInit(UART_HandleTypeDef *huart);
void ProcessReceivedMessage(char *msg);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void USART1_IRQHandler(void);


void MX_USART2_UART_Init(void);
void sendSensorData(int32_t temp, int32_t humi, int32_t ppm);
#endif // __UART_INIT_H
