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
	GPIO_InitDHT11.Mode = GPIO_MODE_OUTPUT_OD;//配置成开漏输出
	GPIO_InitDHT11.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(DHT11_GPIO_PORT,&GPIO_InitDHT11);

}

void DHT11_Free()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitDHT11Free.Pin = GPIO_PIN_5;
	GPIO_InitDHT11Free.Mode = GPIO_MODE_INPUT;//配置成输入
	GPIO_InitDHT11Free.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitDHT11Free.Pull = GPIO_NOPULL;
	
	HAL_GPIO_Init(DHT11_GPIO_PORT,&GPIO_InitDHT11Free);

}

//---------------------------------------------------------------------------------------------------------------------
uint8_t DHT11_CheckState()//主机拉低，主机拉高，等待DHT11响应，DHT11拉高准备发送数据（检查无误）
{
	
	time = 0;
	DHT11_Init();//配置成输出模式，主机控制
	DHT11_LOW;
	delay_ms(30);//主机先拉低至少18ms表示开始,发现20ms都不行，所以这里给的30ms

	DHT11_Free();//开漏输出无法主动配置配高电平，要释放总线，切换成输入模式即可
//	delay_us(30);//拉高30us，再读取DHT11的响应信号。

	while(DHT11_READ);//当还是高电平的时候，阻塞程序(发现使用定时器延时，这里会阻塞)
	while(!DHT11_READ)
	{
		delay_us(10);
		time++;                           /*测量响应信号的持续时间*/
	}
	if(time < 6 || time > 10)
	{
		OLED_ShowString(4, 1,"error1");
		return 0;
	}
	time = 0;
//OLED_ShowNum(3, 1, 404, 3);
	while(DHT11_READ)          // 等待DHT11释放总线，持续时间再30到50之间
	{
		delay_us(10);
		time++;               	// 总线拉高时计数
	}
	if (time<6 || time>10) // 检测响应信号之后的高电平
	{
		OLED_ShowString(4, 1,"error2");
		return 0;
	}
	time = 0;
	return 1;
}
//---------------------------------------------------------------------------------------------------------------------
uint8_t DHT11_ReadByte()//读取一个字节（8bit）数据的函数，（检查无误）
{
	uint8_t Byt;
	for(uint8_t i = 0;i < 8;i++)
	{

		while(DHT11_READ);//等待拉低
		while(!DHT11_READ);//等待拉高，然后通过拉高的时间来判断是1还是0，(DHT11的独特规定)
		delay_us(40);
		Byt = Byt << 1;                            
		if(DHT11_READ)                 //如果延迟了50us后还是读出来高电平，表示是此bit是1。
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
		

//		OLED_ShowNum(1,1,data->Humi_H,4);	 	// 湿度 整数部分
//		OLED_ShowNum(1,8,data->Humi_L,4); 	  // 湿度 小数部分
//		OLED_ShowNum(2,1,data->Temp_H,4);	 	// 温度 整数部分
//		OLED_ShowNum(2,8,data->Temp_L,4); 	  // 温度 小数部分
	}
	else
	{
		OLED_ShowString(4, 1,"error3");
	}
		DHT11_Free();
	}	
}















