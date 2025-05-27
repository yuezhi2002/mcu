#include "stm32f1xx_hal.h"
#include "Oled.h" 
#include "iic_init.h" 
#include "DHT11.h"
#include "Delay.h"
#include "MQ4.h"
#include "uart_init.h"
#include <stdio.h>
//DHT11温度传感器使用单总线协议，此试验使用的是GPIOA5作为数据口，同时此处接一个上拉电阻
//lora接串口1：lora的Rx接单片机的A9，lora的Tx接A10，
//MQ4，的A0口接单片机的GPIOA1
//串口按顺序发，另一个按顺序收，这样不会出错
uint16_t Air_vlu;
uint16_t Temp_vlu;
uint16_t Humi_vle;
DHT11_Data Yata;
void main()
{
	
	HAL_Init();
	Rcc_HSE();
	TIM2_Init();//定时器2初始化，为了定时器延时函数服务

	MX_USART1_UART_Init();//串口1初始化，用于lora通过串口传输数据

	MQ4_ADC_Init();//烟雾传感器初始化
	
	delay_ms(2000);
	uint16_t TempData;
	uint16_t HumiData;
	uint16_t MQ4ppm;

	while (1)
	{
		MQ4ppm = MQ4_GO();
		DHT11_ReadData(&Yata);
		delay_ms(2000);
		
		Temp_vlu = Yata.Temp_H;           
		Humi_vle = Yata.Humi_H;
		UART_SendABC(Temp_vlu,Humi_vle,MQ4ppm);
	}
}