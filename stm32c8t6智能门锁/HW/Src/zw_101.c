#include "stm32f1xx_hal.h"
#include "zw_101.h"
#include "uart_init.h"
#include "state.h"
#include "TIM_DSQIT.h"
#include "iic_init.h" 
#include "oled_init.h" 
/*指纹模块通过单片机串口1（PA9 tx；PA10 rx）与单片机通信
	1.自动注册模板：
	
	指令码：EF 01 FF FF FF FF 01 00 08 31 (2byte id号) （1byte 录入次数）（2byte  参数）（2byte SUM校验和）

	录入次数固定为2次 （02），参数固定为（00 1D）：LED 获取图像成功后灭，注册过程中，不要求模组返回关键步骤，
																										允许覆盖 ID 号，
																										注册时，多次指纹采集过程中，要求手指离开才能进入下一次指纹图像采集

	所以id1指令码(17字节)： EF 01 FF FF FF FF 01 00 08 31 00 01 02 00 1D 00 5A
													EF 01 FF FF FF FF 01 00 08 31 00 02 02 00 1D 00 5B
													EF 01 FF FF FF FF 01 00 08 31 00 03 02 00 1D 00 5C
													EF 01 FF FF FF FF 01 00 08 31 00 04 02 00 1D 00 5D
													EF 01 FF FF FF FF 01 00 08 31 00 05 02 00 1D 00 5E

*/



extern uint8_t registfinger_buff1[17] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x31,0x00,0x01,0x02,0x00,0x1D,0x00,0x5A};
uint8_t registfingerReturn_buff1[14];

	
	
extern uint8_t checkfinger_buff[17] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x32,0x02,0xFF,0xFF,0x00,0x01,0x02,0x3C};
uint8_t checkfingerRerurn_buff1[51] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};



extern uint8_t light_shank[16] =      {0xef,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x07,0x3c,0x02,0x04,0x04,0x02,0x00,0x50};

void zw_init(uint32_t baudrate)
{
	uart1_init(baudrate);
}


/* 函    数：指纹注册函数
 * 参    数：无
 * 返 回 值：registfingerReturn_buff1[9]   0x00成功，0x01失败
 * 注意事项：无
 */
uint8_t regist_finger()
{
	
	uint8_t returnstate = 0;
	HAL_UART_Transmit(&uart1,registfinger_buff1,17,HAL_MAX_DELAY);//串口结构体指针，发送数据包指针，数据包大小，超时时间
	
	HAL_UART_Receive(&uart1,registfingerReturn_buff1,14,HAL_MAX_DELAY);
	
	return registfingerReturn_buff1[9];
}




/* 函    数：验证指纹是否通过(单片机往指纹模块发送验证指纹命令)、
 * 参    数：无
 * 返 回 值：如果是2，表示成功，13表示错误
 *
 * 注意事项：发送的包里有17个字节
 *            应答包1：如果发送验证命令，但是指纹模块在等待指纹时，应答包为：
 *                                          EF 01 FF FF FF FF 07 00 08 00 00 00 FF 00 00 01 0E 确认码00，参数00，ID号00FF
 * 																					表示指令合法性验证成功，并在设定的时间内等待指纹录入验证。
 * 
 * 
 */



uint8_t check_finger()
{
	uint16_t value = 0;
	uint8_t checkfingerRerurn_buff1[51] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	

	HAL_UART_Transmit(&uart1,checkfinger_buff,17,4000);//串口结构体指针，发送数据包指针，数据包大小，超时时间

	
	if(checkfingerRerurn_buff1[17] != 0xEF)//此处表示有指纹放上去了
	{

		HAL_UART_Receive(&uart1,checkfingerRerurn_buff1,51,5000);//接收状态包
		
		if(checkfingerRerurn_buff1[32] == 0x01 && checkfingerRerurn_buff1[33] == 0x35)
		{
			value = 1;                                                                   //表示超时（指纹很久没识别出来） 
		}
		if(checkfingerRerurn_buff1[43] == 0x00 && checkfingerRerurn_buff1[44] == 0x05)
		{
			value = 2;                                                                  //表示指纹比对验证成功
		}
		if(checkfingerRerurn_buff1[43] == 0x09 && checkfingerRerurn_buff1[44] == 0x05)
		{
			value = 3;                                                                  //表示指纹比对验证失败
		}
		else
		{
		value = 4;
		}
	}

	

	return value;
	
}

/* 函    数：测试指纹模块是否正常工作函数
 * 参    数：
 * 返 回 值：
 * 注意事项：如果正常工作，会闪烁一次红
 */
void zw_101_test()
{
	HAL_UART_Transmit(&uart1,light_shank,16,HAL_MAX_DELAY);//串口结构体指针，发送数据包指针，数据包大小，超时时间
}


/* 函    数：指纹接收中断
 * 参    数：
 * 返 回 值：
 * 注意事项：
 */


//void ZW_INIT_IT()
//{
//	__HAL_RCC_GPIOA_CLK_ENABLE();
//	
//	GPIO_InitTypeDef GPIO_InitTypeDefA1;
//	GPIO_InitTypeDefA1.Mode = GPIO_MODE_IT_FALLING;
//	GPIO_InitTypeDefA1.Pin = GPIO_PIN_1;
//	GPIO_InitTypeDefA1.PULL = GPIO_PULLUP;
//	
//	HAL_GPIO_Init(GPIOA,&GPIO_InitTypeDefA1);
//	
//	HAL_NVIC_SetPriority(EXTI1_IRQn,4,0);
//	HAL_NVIC_EnableIRQ(EXTI1_IRQn);

//}

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	uint32_t i;
//	switch(GPIO_Pin)
//	{
//		case	GPIO_PIN_1:
//									if(check_finger() == 1 || check_finger() == 2)
//									{
//										state = 0;
//									}
//									if(check_finger() == 3)
//									{
//										state = 4;
//									}							
//									break;
//	}
//}