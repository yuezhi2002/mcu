#include "stm32f1xx_hal.h"
#include "Oled.h" 
#include "iic_init.h" 
#include "DHT11.h"
#include "Delay.h"
#include "MQ4.h"
#include "uart_init.h"
#include "stdio.h"
//iic屏幕
//串口1：（接lora）GPIOA9  TX，GPIOA10  RX  。  lora模块Rx接单片机的A9，lora的Tx接A10，波特率9600
//串口2：（接4G）GPIOA2 A3为的TX RX，连上4G模块。波特率115200发送数据。
void main()
{
	LED_Init();
	HAL_Init();
	Rcc_HSE();
	TIM2_Init();//定时器2初始化，为了定时器延时函数服务
	OLED_Init();
	OLED_Clear();

	OLED_ShowString(1, 1, "Temp:");
	OLED_ShowString(2, 1, "Humi:");
	OLED_ShowString(3, 1, "PPM:");
	
	MX_USART1_UART_Init();//串口1初始化，用于lora通过串口传输数据9600
	MX_USART2_UART_Init();//串口2初始化，通过连接4g模块，通过4g模块发送数据上云115200
  HAL_UART_Receive_IT(&huart1, &rxBuffer[rxIndex], 1);
	

	while (1)
	{
    OLED_ShowNum(1, 8, temp, 4);
    OLED_ShowNum(2, 8, humi, 4);
    OLED_ShowNum(3, 8, ppm, 4);
        
		sendSensorData(temp,humi,ppm);
		
		if(temp < 35)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);//开绿灯
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);//关蜂鸣器
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);//开红灯
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);//关绿灯
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);//开蜂鸣器
			OLED_ShowString(4, 1, "Warming!!!");
		}
		delay_ms(2000);

	}
}