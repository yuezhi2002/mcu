#include "stm32f1xx_hal.h"
#include "uart_init.h"
UART_HandleTypeDef uart1;


void uart1_init(uint32_t baudrate)//PA9 USART1_tx ;PA10 USART1_rx
{
	uart1.Instance = USART1;          //选择用的串口的基地址
	uart1.Init.BaudRate = baudrate;    //约定的波特率
	uart1.Init.WordLength = UART_WORDLENGTH_8B;//一数据帧8字节
	uart1.Init.Mode = UART_MODE_TX_RX;//发送和接收模式
	uart1.Init.Parity = UART_PARITY_NONE;//不需要校验位
	uart1.Init.StopBits = UART_STOPBITS_1;//一个停止位
	uart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;//硬件流控制关闭
	uart1.Init.OverSampling = UART_OVERSAMPLING_16;//此处保持，除非是f100系列需要改动
	
	HAL_UART_Init(&uart1);

}


void uart2_init()//PA2 USART2_tx ;PA3 USART2_rx
{


}



void uart3_init()//PB10 USART3_tx ;PB11 USART3_rx
{


}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_InitTypeDefA9;//GPIOA9A10结构体
	GPIO_InitTypeDef GPIO_InitTypeDefA10;

	if(huart->Instance == USART1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_USART1_CLK_ENABLE();
		
		GPIO_InitTypeDefA9.Mode = GPIO_MODE_AF_PP;
		GPIO_InitTypeDefA9.Pin = GPIO_PIN_9;
		GPIO_InitTypeDefA9.Speed = GPIO_SPEED_FREQ_MEDIUM;

		GPIO_InitTypeDefA10.Mode = GPIO_MODE_AF_INPUT;
		GPIO_InitTypeDefA10.Pin = GPIO_PIN_10;
		GPIO_InitTypeDefA10.Pull = GPIO_NOPULL;
		
		HAL_GPIO_Init(GPIOA,&GPIO_InitTypeDefA9);
		HAL_GPIO_Init(GPIOA,&GPIO_InitTypeDefA10);
		
	}
}