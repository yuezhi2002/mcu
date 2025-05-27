#include "stm32f1xx_hal.h"  
#include "Oled.h" 
#include "iic_init.h" 
#include "oledfont.h" 
//________________________________________________oled hal库函数________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________


//iic1分配给OLED外设使用


//根据ssd1306数据手册和oled厂家给的原理图，b7 b6 b5 b4 b3 b2 b1 b0
//                                         0  1   1  1 1 0 SA0 R/W#
//其中sa0，厂家接地，0 了，而 R/W# 表示低电平为写
//看出01111000 （0x78）是写地址           01111001（0x79）是读地址
//
/**
  * @brief  I2C发送一个字节
  * @param  Addr 从机地址
  * @param  Byte 要发送的一个字节
  * @retval 无
  */
void OLED_I2C_SendByte(uint8_t Addr, uint8_t Byte)
{
  HAL_I2C_Master_Transmit(&I2C_Handleiic1, Addr, &Byte, 1, HAL_MAX_DELAY);
}

/**
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
//单片机往oled的写地址(0x78)按顺序发送了两个数据，第一个数据是0x00告诉iic是发送一个命令，第二个是自己往里写的参数（要发送的命令）
//                                             C0 | D/C# | 0 | | 0 | | 0 | | 0 | | 0 | | 0 | 
//                                             0      0    0     0     0     0     0     0       == 0x00
//其中C0 = 0，表示连续状态模式（一个命令，后续都是数据）、D/C# = 0表示选择C（commend命令） ，其他的6位都是0
void OLED_WriteCommand(uint8_t Command)
{
  uint8_t data[2] = {0x00, Command};
  HAL_I2C_Master_Transmit(&I2C_Handleiic1, 0x78, data, 2, HAL_MAX_DELAY);
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
//单片机往oled的写地址(0x78)按顺序发送了两个数据，第一个数据是 0x40 告诉iic是发送一个数据，第二个是自己往里写的参数（要发送的数据）
//                                             C0 | D/C# | 0 | | 0 | | 0 | | 0 | | 0 | | 0 | 
//                                             0      1    0     0     0     0     0     0       == 0x40
void OLED_WriteData(uint8_t Data)
{
  uint8_t data[2] = {0x40, Data};
  HAL_I2C_Master_Transmit(&I2C_Handleiic1, 0x78, data, 2, HAL_MAX_DELAY);
}

/* 其他函数保持不变 */

void OLED_SetCursor(uint8_t Y, uint8_t X)
{
  OLED_WriteCommand(0xB0 | Y);					//设置Y位置
  OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
  OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}

void OLED_Clear(void)
{  
  uint8_t i, j;
  for (j = 0; j < 8; j++)
  {
    OLED_SetCursor(j, 0);
    for(i = 0; i < 128; i++)
    {
//			OLED_SetCursor(j,i);//加了这个后清屏速度变慢
      OLED_WriteData(0x00);
    }
  }
}

void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
  uint8_t i;
  OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
  for (i = 0; i < 8; i++)
  {
    OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
  }
  OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
  for (i = 0; i < 8; i++)
  {
    OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
  }
}

void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
  uint8_t i;
  for (i = 0; String[i] != '\0'; i++)
  {
    OLED_ShowChar(Line, Column + i, String[i]);
  }
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
  uint32_t Result = 1;
  while (Y--)
  {
    Result *= X;
  }
  return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
  uint8_t i;
  for (i = 0; i < Length; i++)							
  {
    OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
  }
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
  uint8_t i;
  uint32_t Number1;
  if (Number >= 0)
  {
    OLED_ShowChar(Line, Column, '+');
    Number1 = Number;
  }
  else
  {
    OLED_ShowChar(Line, Column, '-');
    Number1 = -Number;
  }
  for (i = 0; i < Length; i++)							
  {
    OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
  }
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
  uint8_t i, SingleNumber;
  for (i = 0; i < Length; i++)							
  {
    SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
    if (SingleNumber < 10)
    {
      OLED_ShowChar(Line, Column + i, SingleNumber + '0');
    }
    else
    {
      OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
    }
  }
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
  uint8_t i;
  for (i = 0; i < Length; i++)							
  {
    OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
  }
}

/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init(void)
{
  uint32_t i, j;
	
  for (i = 0; i < 1000; i++)			//上电延时
  {
    for (j = 0; j < 1000; j++);
  }
	
  iic1_init();			//端口初始化
	
  OLED_WriteCommand(0xAE);	//关闭显示
	
  OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
  OLED_WriteCommand(0x80);
	
  OLED_WriteCommand(0xA8);	//设置多路复用率
  OLED_WriteCommand(0x3F);
	
  OLED_WriteCommand(0xD3);	//设置显示偏移
	
  OLED_WriteCommand(0x00);
	
  OLED_WriteCommand(0x40);	//设置显示开始行
	
  OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
  OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

  OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
  OLED_WriteCommand(0x12);
	
  OLED_WriteCommand(0x81);	//设置对比度控制
  OLED_WriteCommand(0xCF);

  OLED_WriteCommand(0xD9);	//设置预充电周期
  OLED_WriteCommand(0xF1);

  OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
  OLED_WriteCommand(0x30);

  OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

  OLED_WriteCommand(0xA6);	//设置正常/倒转显示

  OLED_WriteCommand(0x8D);	//设置充电泵
  OLED_WriteCommand(0x14);

  OLED_WriteCommand(0xAF);	//开启显示
		
  OLED_Clear();				//OLED清屏
}

void OLED_ClearOnce()
{
	


}
