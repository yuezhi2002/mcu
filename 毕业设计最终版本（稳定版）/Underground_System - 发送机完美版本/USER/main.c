#include "stm32f1xx_hal.h"
#include "Oled.h" 
#include "iic_init.h" 
#include "DHT11.h"
#include "Delay.h"
#include "MQ4.h"
#include "uart_init.h"
#include <stdio.h>
//DHT11�¶ȴ�����ʹ�õ�����Э�飬������ʹ�õ���GPIOA5��Ϊ���ݿڣ�ͬʱ�˴���һ����������
//lora�Ӵ���1��lora��Rx�ӵ�Ƭ����A9��lora��Tx��A10��
//MQ4����A0�ڽӵ�Ƭ����GPIOA1
//���ڰ�˳�򷢣���һ����˳���գ������������
uint16_t Air_vlu;
uint16_t Temp_vlu;
uint16_t Humi_vle;
DHT11_Data Yata;
void main()
{
	
	HAL_Init();
	Rcc_HSE();
	TIM2_Init();//��ʱ��2��ʼ����Ϊ�˶�ʱ����ʱ��������

	MX_USART1_UART_Init();//����1��ʼ��������loraͨ�����ڴ�������

	MQ4_ADC_Init();//����������ʼ��
	
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