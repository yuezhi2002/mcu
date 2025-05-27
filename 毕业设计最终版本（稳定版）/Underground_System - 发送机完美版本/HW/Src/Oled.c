#include "stm32f1xx_hal.h"  
#include "Oled.h" 
#include "iic_init.h" 
#include "oledfont.h" 
//________________________________________________oled hal�⺯��________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________


//iic1�����OLED����ʹ��


//����ssd1306�����ֲ��oled���Ҹ���ԭ��ͼ��b7 b6 b5 b4 b3 b2 b1 b0
//                                         0  1   1  1 1 0 SA0 R/W#
//����sa0�����ҽӵأ�0 �ˣ��� R/W# ��ʾ�͵�ƽΪд
//����01111000 ��0x78����д��ַ           01111001��0x79���Ƕ���ַ
//
/**
  * @brief  I2C����һ���ֽ�
  * @param  Addr �ӻ���ַ
  * @param  Byte Ҫ���͵�һ���ֽ�
  * @retval ��
  */
void OLED_I2C_SendByte(uint8_t Addr, uint8_t Byte)
{
  HAL_I2C_Master_Transmit(&I2C_Handleiic1, Addr, &Byte, 1, HAL_MAX_DELAY);
}

/**
  * @brief  OLEDд����
  * @param  Command Ҫд�������
  * @retval ��
  */
//��Ƭ����oled��д��ַ(0x78)��˳�������������ݣ���һ��������0x00����iic�Ƿ���һ������ڶ������Լ�����д�Ĳ�����Ҫ���͵����
//                                             C0 | D/C# | 0 | | 0 | | 0 | | 0 | | 0 | | 0 | 
//                                             0      0    0     0     0     0     0     0       == 0x00
//����C0 = 0����ʾ����״̬ģʽ��һ����������������ݣ���D/C# = 0��ʾѡ��C��commend��� ��������6λ����0
void OLED_WriteCommand(uint8_t Command)
{
  uint8_t data[2] = {0x00, Command};
  HAL_I2C_Master_Transmit(&I2C_Handleiic1, 0x78, data, 2, HAL_MAX_DELAY);
}

/**
  * @brief  OLEDд����
  * @param  Data Ҫд�������
  * @retval ��
  */
//��Ƭ����oled��д��ַ(0x78)��˳�������������ݣ���һ�������� 0x40 ����iic�Ƿ���һ�����ݣ��ڶ������Լ�����д�Ĳ�����Ҫ���͵����ݣ�
//                                             C0 | D/C# | 0 | | 0 | | 0 | | 0 | | 0 | | 0 | 
//                                             0      1    0     0     0     0     0     0       == 0x40
void OLED_WriteData(uint8_t Data)
{
  uint8_t data[2] = {0x40, Data};
  HAL_I2C_Master_Transmit(&I2C_Handleiic1, 0x78, data, 2, HAL_MAX_DELAY);
}

/* �����������ֲ��� */

void OLED_SetCursor(uint8_t Y, uint8_t X)
{
  OLED_WriteCommand(0xB0 | Y);					//����Yλ��
  OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//����Xλ�ø�4λ
  OLED_WriteCommand(0x00 | (X & 0x0F));			//����Xλ�õ�4λ
}

void OLED_Clear(void)
{  
  uint8_t i, j;
  for (j = 0; j < 8; j++)
  {
    OLED_SetCursor(j, 0);
    for(i = 0; i < 128; i++)
    {
//			OLED_SetCursor(j,i);//��������������ٶȱ���
      OLED_WriteData(0x00);
    }
  }
}

void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
  uint8_t i;
  OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//���ù��λ�����ϰ벿��
  for (i = 0; i < 8; i++)
  {
    OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//��ʾ�ϰ벿������
  }
  OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//���ù��λ�����°벿��
  for (i = 0; i < 8; i++)
  {
    OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//��ʾ�°벿������
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
  * @brief  OLED�η�����
  * @retval ����ֵ����X��Y�η�
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
  * @brief  OLED��ʾ���֣�ʮ���ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~4294967295
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
  * @retval ��
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
  * @brief  OLED��ʾ���֣�ʮ���ƣ�����������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��-2147483648~2147483647
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
  * @retval ��
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
  * @brief  OLED��ʾ���֣�ʮ�����ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~0xFFFFFFFF
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~8
  * @retval ��
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
  * @brief  OLED��ʾ���֣������ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~1111 1111 1111 1111
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~16
  * @retval ��
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
  * @brief  OLED��ʼ��
  * @param  ��
  * @retval ��
  */
void OLED_Init(void)
{
  uint32_t i, j;
	
  for (i = 0; i < 1000; i++)			//�ϵ���ʱ
  {
    for (j = 0; j < 1000; j++);
  }
	
  iic1_init();			//�˿ڳ�ʼ��
	
  OLED_WriteCommand(0xAE);	//�ر���ʾ
	
  OLED_WriteCommand(0xD5);	//������ʾʱ�ӷ�Ƶ��/����Ƶ��
  OLED_WriteCommand(0x80);
	
  OLED_WriteCommand(0xA8);	//���ö�·������
  OLED_WriteCommand(0x3F);
	
  OLED_WriteCommand(0xD3);	//������ʾƫ��
	
  OLED_WriteCommand(0x00);
	
  OLED_WriteCommand(0x40);	//������ʾ��ʼ��
	
  OLED_WriteCommand(0xA1);	//�������ҷ���0xA1���� 0xA0���ҷ���
	
  OLED_WriteCommand(0xC8);	//�������·���0xC8���� 0xC0���·���

  OLED_WriteCommand(0xDA);	//����COM����Ӳ������
  OLED_WriteCommand(0x12);
	
  OLED_WriteCommand(0x81);	//���öԱȶȿ���
  OLED_WriteCommand(0xCF);

  OLED_WriteCommand(0xD9);	//����Ԥ�������
  OLED_WriteCommand(0xF1);

  OLED_WriteCommand(0xDB);	//����VCOMHȡ��ѡ�񼶱�
  OLED_WriteCommand(0x30);

  OLED_WriteCommand(0xA4);	//����������ʾ��/�ر�

  OLED_WriteCommand(0xA6);	//��������/��ת��ʾ

  OLED_WriteCommand(0x8D);	//���ó���
  OLED_WriteCommand(0x14);

  OLED_WriteCommand(0xAF);	//������ʾ
		
  OLED_Clear();				//OLED����
}

void OLED_ClearOnce()
{
	


}
