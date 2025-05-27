#include "stm32f1xx_hal.h"  
#include "iic_init.h" 
//_______________________________________________________________________________________________________________
//__________________________________________IIC1_Init__________________________________________________________________________________________________________________________________________________________________
I2C_HandleTypeDef I2C_Handleiic1;


void iic1_init()
{
	I2C_Handleiic1.Instance = I2C1;
	I2C_Handleiic1.Init.ClockSpeed = 100000;//ͨ������100k
//�ܶණ����ʵ������Ҫ���ã����磺��Ϊʹ�õ�ͨ��������100k��û��ʹ��400k����ģʽ�����Բ���Ҫʹ��DutyCycle����ģʽ��ռ�ձ�
//����OwnAddress1�������ַ1������Ϊ��ʹ�õ�stm������Ϊ���豸���������ַ��stm32��Ƭ����Ϊ���豸Ϊ�������豸Ѱַ���õģ�����Ҳ����Ҫ����
//DualAddressMode���Ƿ�ʹ��˫��ַ���ǵ�Ƭ����Ϊ�ӻ�ʱ���Ƿ���Ӧ����iic�豸�����󣬵�����������ʹ�õ�Ƭ����Ϊ���豸������Ҳ����Ҫ���ã����߽��ü���
//GeneralCallMode(�㲥����)��Ƭ�������дӻ�����ͬһ�����һ�����ã���������
//NoStretchMode��ʱ���ӳ���Ҳ�ǵ�Ƭ����Ϊ�ӻ��ſ�����Ҫ���õġ�
	I2C_Handleiic1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;   //iicѰַģʽ������ʹ�õ��ⲿ�豸�ĵ�ַ����7λ������at24c02�������ַ��0xA0��1010 0000����λ�����ݣ����һλ�Ƕ�дλ������8-1 = 7bit
	
	HAL_I2C_Init(&I2C_Handleiic1);
	
}
//_______________________________________________________________________________________________________________
//__________________________________________IIC2_Init__________________________________________________________________________________________________________________________________________________________________
I2C_HandleTypeDef I2C_Handleiic2;

void iic2_init()
{
	I2C_Handleiic2.Instance = I2C2;
	I2C_Handleiic2.Init.ClockSpeed = 100000;//ͨ������100k
	I2C_Handleiic2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;   //iicѰַģʽ������ʹ�õ��ⲿ�豸�ĵ�ַ����7λ������at24c02�������ַ��0xA0��1010 0000����λ�����ݣ����һλ�Ƕ�дλ������8-1 = 7bit
	
	HAL_I2C_Init(&I2C_Handleiic2);
	
}
//_______________________________________________________________________________________________________________
//__________________________________����豸Ӳ���Զ����ú���_____________________________________________________________

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	if(hi2c->Instance == I2C1)
	{
		GPIO_InitTypeDef GPIO_InitTypeDefB67;//����GPIOBpin6 7��gpio����
		__HAL_RCC_GPIOB_CLK_ENABLE();//��GPIOBʱ��
		__HAL_RCC_I2C1_CLK_ENABLE();//��i2c1ʱ��
	
		GPIO_InitTypeDefB67.Mode = GPIO_MODE_AF_OD;
		GPIO_InitTypeDefB67.Pin = GPIO_PIN_6|GPIO_PIN_7;
		GPIO_InitTypeDefB67.Speed = GPIO_SPEED_FREQ_MEDIUM;

		HAL_GPIO_Init(GPIOB,&GPIO_InitTypeDefB67);
	
	}
	if(hi2c->Instance == I2C2)
	{
		GPIO_InitTypeDef GPIO_InitTypeDefB1011;//����GPIOBpin10 11��gpio����
		__HAL_RCC_GPIOB_CLK_ENABLE();//��GPIOBʱ��
		__HAL_RCC_I2C2_CLK_ENABLE();//��i2c1ʱ��
	
		GPIO_InitTypeDefB1011.Mode = GPIO_MODE_AF_OD;
		GPIO_InitTypeDefB1011.Pin = GPIO_PIN_10|GPIO_PIN_11;
		GPIO_InitTypeDefB1011.Speed = GPIO_SPEED_FREQ_MEDIUM;

		HAL_GPIO_Init(GPIOB,&GPIO_InitTypeDefB1011);
	}	
}
