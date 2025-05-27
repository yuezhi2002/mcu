#include "stm32f1xx_hal.h"
#include "Oled.h" 
#include "iic_init.h" 
#include "DHT11.h"
#include "Delay.h"
#include "MQ4.h"
#include "uart_init.h"
#include "stdio.h"
//iic��Ļ
//����1������lora��GPIOA9  TX��GPIOA10  RX  ��  loraģ��Rx�ӵ�Ƭ����A9��lora��Tx��A10��������9600
//����2������4G��GPIOA2 A3Ϊ��TX RX������4Gģ�顣������115200�������ݡ�
void main()
{
	LED_Init();
	HAL_Init();
	Rcc_HSE();
	TIM2_Init();//��ʱ��2��ʼ����Ϊ�˶�ʱ����ʱ��������
	OLED_Init();
	OLED_Clear();

	OLED_ShowString(1, 1, "Temp:");
	OLED_ShowString(2, 1, "Humi:");
	OLED_ShowString(3, 1, "PPM:");
	
	MX_USART1_UART_Init();//����1��ʼ��������loraͨ�����ڴ�������9600
	MX_USART2_UART_Init();//����2��ʼ����ͨ������4gģ�飬ͨ��4gģ�鷢����������115200
  HAL_UART_Receive_IT(&huart1, &rxBuffer[rxIndex], 1);
	

	while (1)
	{
    OLED_ShowNum(1, 8, temp, 4);
    OLED_ShowNum(2, 8, humi, 4);
    OLED_ShowNum(3, 8, ppm, 4);
        
		sendSensorData(temp,humi,ppm);
		
		if(temp < 35)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);//���̵�
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);//�ط�����
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);//�����
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);//���̵�
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);//��������
			OLED_ShowString(4, 1, "Warming!!!");
		}
		delay_ms(2000);

	}
}