#include "stm32f1xx_hal.h"
#include "uart_init.h"   // �ٶ����д�ͷ�ļ�������ؽӿ�
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "stm32f1xx_it.h"  


UART_HandleTypeDef huart1;

void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 9600;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
//        Error_Handler();
    }
}
//------------------------------------------------------------------------------------------------------------

void UART1_SendData(uint8_t *pData, uint16_t size)
{
    HAL_UART_Transmit(&huart1, pData, size, HAL_MAX_DELAY);
}

void UART_SendABC(uint16_t a, uint16_t b, uint16_t c)
{
    char txData[32]; // ��������С������Ҫ����������һЩ�ռ�ȷ���㹻�����ֵ����ʽ����
    
    // ��ʽ���ַ�����ʹ�ö��ŷָ�����������ĩβ��� "\r\n"
    snprintf(txData, sizeof(txData), "%u,%u,%u\r\n", a, b, c);
    
    // ʵ�ʷ���ʱ�������ַ������Ⱥ��͡�strlen(txData)��������ֹ��'\0'
    UART1_SendData((uint8_t *)txData, strlen(txData));
}

void sendSensorData(int32_t temp, int32_t humi, int32_t ppm)
{
    char jsonString[256]; // ����һ���㹻����ַ���������
    const uint8_t id = 1742; // �̶��� ID
    snprintf(jsonString, sizeof(jsonString),
             "{\"id\":\"%d\",\"version\":\"1.0\",\"params\":{\"Temp\":{\"value\":%d},\"Humi\":{\"value\":%d},\"PPM\":{\"value\":%d}}}",
             id, temp, humi, ppm);

    HAL_UART_Transmit(&huart1, (uint8_t*)jsonString, strlen(jsonString), HAL_MAX_DELAY);
}

//------------------------------------------------------------------------------------------------------------
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if(huart->Instance == USART1)
    {
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}

