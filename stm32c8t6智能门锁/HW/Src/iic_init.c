#include "stm32f1xx_hal.h"  
#include "iic_init.h" 
//_______________________________________________________________________________________________________________
//__________________________________________IIC1_Init__________________________________________________________________________________________________________________________________________________________________
I2C_HandleTypeDef I2C_Handleiic1;


void iic1_init()
{
	I2C_Handleiic1.Instance = I2C1;
	I2C_Handleiic1.Init.ClockSpeed = 100000;//通信速率100k
//很多东西其实都不需要设置，例如：因为使用的通信速率是100k，没有使用400k快速模式，所以不需要使用DutyCycle快速模式下占空比
//还有OwnAddress1（自身地址1），因为我使用的stm都是作为主设备，而自身地址是stm32单片机作为从设备为了其他设备寻址设置的，所以也不需要设置
//DualAddressMode（是否使用双地址）是单片机作为从机时，是否响应两个iic设备的请求，但是我们往往使用单片机作为主设备，所以也不需要设置，或者禁用即可
//GeneralCallMode(广播呼叫)单片机向所有从机发送同一个命令，一般无用，无需设置
//NoStretchMode（时钟延长）也是单片机作为从机才可能需要设置的。
	I2C_Handleiic1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;   //iic寻址模式，像我使用的外部设备的地址都是7位，例如at24c02的外设地址是0xA0（1010 0000）八位的数据，最后一位是读写位，所以8-1 = 7bit
	
	HAL_I2C_Init(&I2C_Handleiic1);
	
}
//_______________________________________________________________________________________________________________
//__________________________________________IIC2_Init__________________________________________________________________________________________________________________________________________________________________
I2C_HandleTypeDef I2C_Handleiic2;

void iic2_init()
{
	I2C_Handleiic2.Instance = I2C2;
	I2C_Handleiic2.Init.ClockSpeed = 100000;//通信速率100k
	I2C_Handleiic2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;   //iic寻址模式，像我使用的外部设备的地址都是7位，例如at24c02的外设地址是0xA0（1010 0000）八位的数据，最后一位是读写位，所以8-1 = 7bit
	
	HAL_I2C_Init(&I2C_Handleiic2);
	
}
//_______________________________________________________________________________________________________________
//__________________________________相关设备硬件自动配置函数_____________________________________________________________

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	if(hi2c->Instance == I2C1)
	{
		GPIO_InitTypeDef GPIO_InitTypeDefB67;//设置GPIOBpin6 7的gpio配置
		__HAL_RCC_GPIOB_CLK_ENABLE();//打开GPIOB时钟
		__HAL_RCC_I2C1_CLK_ENABLE();//打开i2c1时钟
	
		GPIO_InitTypeDefB67.Mode = GPIO_MODE_AF_OD;
		GPIO_InitTypeDefB67.Pin = GPIO_PIN_6|GPIO_PIN_7;
		GPIO_InitTypeDefB67.Speed = GPIO_SPEED_FREQ_MEDIUM;

		HAL_GPIO_Init(GPIOB,&GPIO_InitTypeDefB67);
	
	}
	if(hi2c->Instance == I2C2)
	{
		GPIO_InitTypeDef GPIO_InitTypeDefB1011;//设置GPIOBpin10 11的gpio配置
		__HAL_RCC_GPIOB_CLK_ENABLE();//打开GPIOB时钟
		__HAL_RCC_I2C2_CLK_ENABLE();//打开i2c1时钟
	
		GPIO_InitTypeDefB1011.Mode = GPIO_MODE_AF_OD;
		GPIO_InitTypeDefB1011.Pin = GPIO_PIN_10|GPIO_PIN_11;
		GPIO_InitTypeDefB1011.Speed = GPIO_SPEED_FREQ_MEDIUM;

		HAL_GPIO_Init(GPIOB,&GPIO_InitTypeDefB1011);
	}	
}
