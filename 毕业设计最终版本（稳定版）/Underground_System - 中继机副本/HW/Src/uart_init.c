#include "stm32f1xx_hal.h"
#include "uart_init.h"   // 假定你有此头文件声明相关接口
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "stm32f1xx_it.h"
#include "stdint.h"

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
#define RX_BUFFER_SIZE 128
uint8_t rxBuffer[RX_BUFFER_SIZE];
volatile uint16_t rxIndex = 0;
uint16_t temp = 0, humi = 0, ppm = 0;

//===========================================================================================================================

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
    if(HAL_UART_Init(&huart1) != HAL_OK)
    {
        //while(1);
    }
}


void MX_USART2_UART_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if(HAL_UART_Init(&huart2) != HAL_OK)
    {
        //while(1);
    }
}
//===========================================================================================================================



void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStructLora;
	  GPIO_InitTypeDef GPIO_InitStruct4G;
    if(huart->Instance == USART1)
    {
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitStructLora.Pin = GPIO_PIN_9;
        GPIO_InitStructLora.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStructLora.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStructLora);
        GPIO_InitStructLora.Pin = GPIO_PIN_10;
        GPIO_InitStructLora.Mode = GPIO_MODE_INPUT;
        GPIO_InitStructLora.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStructLora);
        HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
		
		
		if(huart->Instance == USART2)
    {
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitStruct4G.Pin = GPIO_PIN_2;
        GPIO_InitStruct4G.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct4G.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct4G);
        GPIO_InitStruct4G.Pin = GPIO_PIN_3;
        GPIO_InitStruct4G.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct4G.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct4G);
    }
}
//===================================================中继机串口2发送函数（连接4g发送数据上云端）=======================================================================================



void sendSensorData(int32_t temp, int32_t humi, int32_t ppm)
{
    char jsonString[256]; // 定义一个足够大的字符串缓冲区
    const uint8_t id = 1742; // 固定的 ID
    snprintf(jsonString, sizeof(jsonString),
             "{\"id\":\"%d\",\"version\":\"1.0\",\"params\":{\"Temp\":{\"value\":%d},\"Humi\":{\"value\":%d},\"PPM\":{\"value\":%d}}}",id, temp, humi, ppm);
		 strcat(jsonString, "\r\n"); // 在格式化的 JSON 字符串末尾添加 \r\n 换行符
    HAL_UART_Transmit(&huart2, (uint8_t*)jsonString, strlen(jsonString), HAL_MAX_DELAY);
}



//--------------------------------中继机串口接收函数函数(包括数据处理和中断，连接lora接收发送机的数据并且储存起来)------------------------------------------------------------
void ProcessReceivedMessage(char *msg)
{
    size_t len = strlen(msg);
    while(len && (msg[len-1]=='\r' || msg[len-1]=='\n'))
    {
        msg[--len] = '\0';
    }
    sscanf(msg, "%hu,%hu,%hu", &temp, &humi, &ppm);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        if(rxBuffer[rxIndex]=='\n')
        {
            rxBuffer[rxIndex] = '\0';
            ProcessReceivedMessage((char*)rxBuffer);
            rxIndex = 0;
        }
        else
        {
            if(rxIndex < RX_BUFFER_SIZE-1)
                rxIndex++;
            else
                rxIndex = 0;
        }
        HAL_UART_Receive_IT(huart, &rxBuffer[rxIndex], 1);
    }
}


void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------