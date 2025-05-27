#include "stm32f1xx_hal.h"
#include "Oled.h" 
#include "iic_init.h" 
#include "DHT11.h"
#include "Delay.h"
#include "uart_init.h"

GPIO_InitTypeDef GPIO_InitDHT11;
GPIO_InitTypeDef GPIO_InitDHT11Free;

uint16_t time;
void DHT11_Init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitDHT11.Pin = GPIO_PIN_5;
	GPIO_InitDHT11.Mode = GPIO_MODE_OUTPUT_OD;//���óɿ�©���
	GPIO_InitDHT11.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(DHT11_GPIO_PORT,&GPIO_InitDHT11);

}

void DHT11_Free()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitDHT11Free.Pin = GPIO_PIN_5;
	GPIO_InitDHT11Free.Mode = GPIO_MODE_INPUT;//���ó�����
	GPIO_InitDHT11Free.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitDHT11Free.Pull = GPIO_NOPULL;
	
	HAL_GPIO_Init(DHT11_GPIO_PORT,&GPIO_InitDHT11Free);

}

//---------------------------------------------------------------------------------------------------------------------
uint8_t DHT11_CheckState()//�������ͣ��������ߣ��ȴ�DHT11��Ӧ��DHT11����׼���������ݣ��������
{
	
	time = 0;
	DHT11_Init();//���ó����ģʽ����������
	DHT11_LOW;
	delay_ms(30);//��������������18ms��ʾ��ʼ,����20ms�����У������������30ms

	DHT11_Free();//��©����޷�����������ߵ�ƽ��Ҫ�ͷ����ߣ��л�������ģʽ����
//	delay_us(30);//����30us���ٶ�ȡDHT11����Ӧ�źš�

	while(DHT11_READ);//�����Ǹߵ�ƽ��ʱ����������(����ʹ�ö�ʱ����ʱ�����������)
	while(!DHT11_READ)
	{
		delay_us(10);
		time++;                           /*������Ӧ�źŵĳ���ʱ��*/
	}
	if(time < 6 || time > 10)
	{
		OLED_ShowString(4, 1,"error1");
		return 0;
	}
	time = 0;
//OLED_ShowNum(3, 1, 404, 3);
	while(DHT11_READ)          // �ȴ�DHT11�ͷ����ߣ�����ʱ����30��50֮��
	{
		delay_us(10);
		time++;               	// ��������ʱ����
	}
	if (time<6 || time>10) // �����Ӧ�ź�֮��ĸߵ�ƽ
	{
		OLED_ShowString(4, 1,"error2");
		return 0;
	}
	time = 0;
	return 1;
}
//---------------------------------------------------------------------------------------------------------------------
uint8_t DHT11_ReadByte()//��ȡһ���ֽڣ�8bit�����ݵĺ��������������
{
	uint8_t Byt;
	for(uint8_t i = 0;i < 8;i++)
	{

		while(DHT11_READ);//�ȴ�����
		while(!DHT11_READ);//�ȴ����ߣ�Ȼ��ͨ�����ߵ�ʱ�����ж���1����0��(DHT11�Ķ��ع涨)
		delay_us(40);
		Byt = Byt << 1;                            
		if(DHT11_READ)                 //����ӳ���50us���Ƕ������ߵ�ƽ����ʾ�Ǵ�bit��1��
		{
			Byt = Byt | 0x01;
		}
		else
		{
			Byt = Byt | 0x00;
		}  
	}
	return Byt;
}
//---------------------------------------------------------------------------------------------------------------------
uint16_t DHT11_ReadData(DHT11_Data *data)
{
//	DHT11_Data Data;
	uint8_t i;
	uint16_t buf[5];
	uint16_t totalvalue;
	if(DHT11_CheckState() == 1)
	{

		for(i = 0;i < 5;i++)
		{
			buf[i] = DHT11_ReadByte();
		}
	if(buf[4] == buf[0]+buf[1]+buf[2]+buf[3])
	{
		data->Humi_H = buf[0];
		data->Humi_L = buf[1];
		data->Temp_H = buf[2];
		data->Temp_L = buf[3];
		

//		OLED_ShowNum(1,1,data->Humi_H,4);	 	// ʪ�� ��������
//		OLED_ShowNum(1,8,data->Humi_L,4); 	  // ʪ�� С������
//		OLED_ShowNum(2,1,data->Temp_H,4);	 	// �¶� ��������
//		OLED_ShowNum(2,8,data->Temp_L,4); 	  // �¶� С������
	}
	else
	{
		OLED_ShowString(4, 1,"error3");
	}
		DHT11_Free();
	}	
}















