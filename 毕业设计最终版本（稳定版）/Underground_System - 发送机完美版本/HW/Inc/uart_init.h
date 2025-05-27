#ifndef __UART_INIT_H
#define __UART_INIT_H

#include "stm32f1xx_hal.h"  // HAL ��ͷ�ļ�
#include <stdarg.h>         // �ɱ��������֧��
#include <stdio.h>          // ��׼���������
#include <string.h>         // �ַ���������

extern UART_HandleTypeDef huart1;

void MX_USART1_UART_Init(void);
void UART1_SendData(uint8_t *pData, uint16_t size);
void UART_SendABC(uint16_t a, uint16_t b, uint16_t c);

void sendSensorData(int32_t temp, int32_t humi, int32_t ppm);

#endif