#include "stm32f1xx_hal.h"
#include "ttp229_keyboard.h"

//本实验是使用搭载ttp229bsf芯片的电容式触摸矩阵屏（模块已经设计好，使用串行通信，且16键可用）
//使用PB12作为scl时钟线接口，PB13作为sdo数据线接口
//这里的数据线是键盘向单片机发送的数据的线路。时钟线是用来表示单片机接收到了一个数据的线路


	/* 函    数：keyboard_init();
   * 参    数：无
   * 返 回 值：无
   * 注意事项：目前仅初始化GPIO
   */
void keyboard_gpio_init()
{
	GPIO_InitTypeDef GPIO_InitTypeDefKeyboardscl;
	GPIO_InitTypeDefKeyboardscl.Pin = GPIO_PIN_12;
	GPIO_InitTypeDefKeyboardscl.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitTypeDefKeyboardscl.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	GPIO_InitTypeDef  GPIO_InitTypeDefKeyboardsdo;
	GPIO_InitTypeDefKeyboardsdo.Pin = GPIO_PIN_13;
	GPIO_InitTypeDefKeyboardsdo.Mode = GPIO_MODE_INPUT;
	GPIO_InitTypeDefKeyboardsdo.Pull = GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOB,&GPIO_InitTypeDefKeyboardscl);
	HAL_GPIO_Init(GPIOB,&GPIO_InitTypeDefKeyboardsdo);
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
}

uint8_t TTP229_ReadKey()
{
	uint8_t i = 0;
	uint8_t ttp229KeyNum = 0;
	uint8_t Num = 0;
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,RESET);//根据时序图，在使用之前需要将scl和sdo线拉低
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,RESET);
	
	for(i = 0;i<16;i++)
		{
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13))
			{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,SET);//如果是第一次拉高拉低表示是第一个按键,第二次则表示是第二个按键
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,RESET);	
			Num++;
			}
		}
		switch (Num)
		{
			default:break;
			case 1:ttp229KeyNum = 1;break;
			case 2:ttp229KeyNum = 2;break;
			case 3:ttp229KeyNum = 3;break;
			case 4:ttp229KeyNum = 'A';break;                  //A 65
			case 5:ttp229KeyNum = 4;break;
			case 6:ttp229KeyNum = 5;break;
			case 7:ttp229KeyNum = 6;break;
			case 8:ttp229KeyNum = 'B';break;                 //B66
			case 9:ttp229KeyNum = 7;break;
			case 10:ttp229KeyNum = 8;break;
			case 11:ttp229KeyNum = 9;break;
			case 12:ttp229KeyNum = 'C';break;                 //c67
			case 13:ttp229KeyNum = '*';break;                //*42
			case 14:ttp229KeyNum = 10;break;//注意是10 不是0，不然返回值为0；
			case 15:ttp229KeyNum = '#';break;                 //#35
//			case 16:ttp229KeyNum = 'D';break;                 //D68  这个按键如果使用，按键空闲时也会自动停留再这里，所以禁用


		}
		return ttp229KeyNum;
}
//	if(Num == 1)
//	{
//		ttp229KeyNum = 1;
//	}	
//		if(Num == 2)
//	{
//		ttp229KeyNum = 2;
//	}
//		if(Num == 3)
//	{
//		ttp229KeyNum = 3;
//	}
//		if(Num == 5)
//	{
//		ttp229KeyNum = 4;
//	}
//		if(Num == 6)
//	{
//		ttp229KeyNum = 5;
//	}
//		if(Num == 7)
//	{
//		ttp229KeyNum = 6;
//	}
//		if(Num == 9)
//	{
//		ttp229KeyNum = 7;
//	}
//		if(Num == 10)
//	{
//		ttp229KeyNum = 8;
//	}
//		if(Num == 11)
//	{
//		ttp229KeyNum = 9;
//	}
//		if(Num == 14)
//	{
//		ttp229KeyNum = 0;
//	}
//	
//	
//		if(Num == 4)
//	{
//		ttp229KeyNum = 'A';//A 65
//	}
//		if(Num == 8)
//	{
//		ttp229KeyNum = 'B';//B66
//	}
//		if(Num == 12)
//	{
//		ttp229KeyNum = 'C';//c67
//	}
//		if(Num == 13)
//	{
//		ttp229KeyNum = '*';//*42
//	}
//		if(Num == 15)
//	{
//		ttp229KeyNum = '#';//#35
//	}
////		if(Num == 16)
////	{
////		ttp229KeyNum = 'D';//D68
////	}
//		
//	return ttp229KeyNum;
//}


