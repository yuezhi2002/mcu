#include "stm32f1xx_hal.h"
#include "duoji_SG90.h"

TIM_HandleTypeDef TIM_HandleSG90;         //时基单元基本结构体
TIM_OC_InitTypeDef TIM_OC_InitOC1;        //输出比较结构体

void SG90_Control()
{
/*---------------------------------------------------------------------------------------------------------------
		   TIM_HandleTypeDef TIM_HandleSG90;         
             时基单元基本结构体成员*/
	
	TIM_HandleSG90.Instance = TIM1;                                       //指向需要使用的定时器的寄存器基地址的指针
	TIM_HandleSG90.Channel = HAL_TIM_ACTIVE_CHANNEL_1;                    //当前激活的通道
	TIM_HandleSG90.Init.CounterMode = TIM_COUNTERMODE_UP;                 //计数模式
	TIM_HandleSG90.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;           //时钟分频因子         72mhz
	TIM_HandleSG90.Init.Prescaler  = 72-1;                              //预分频值
	TIM_HandleSG90.Init.Period = 20000-1;                               //自动重装载值
	TIM_HandleSG90.Init.RepetitionCounter = 0;                            //重复计数值
	TIM_HandleSG90.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;//自动重载值
//DMA_HandleTypeDef  DMA结构体； Lock 配置锁；State 状态；
	
	HAL_TIM_PWM_Init(&TIM_HandleSG90);
	
/*---------------------------------------------------------------------------------------------------------------
	     TIM_OC_InitTypeDef TIM_OC_InitOC1; 
             输出比较结构体成员*/ 

	
	TIM_OC_InitOC1.OCMode = TIM_OCMODE_PWM1;                      //输出比较的pwm模式
	TIM_OC_InitOC1.Pulse = 0;                                     //指定要加载到捕获比较寄存器的脉冲值
	TIM_OC_InitOC1.OCPolarity = TIM_OCPOLARITY_HIGH;              //指定输出极性
	
//其他未用到成员：OCNPolarity 互补输出极性； OCFastMode 快速模式状态；OCIdleState空闲状态下输出pin状态； OCNIdleState空闲状态下输出pin状态；
/*---------------------------------------------------------------------------------------------------------------*/
	
//	HAL_TIM_PWM_Init(&TIM_HandleSG90);
	HAL_TIM_PWM_ConfigChannel(&TIM_HandleSG90,&TIM_OC_InitOC1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&TIM_HandleSG90,TIM_CHANNEL_1);
}


void Servo_SetAngle(float Angle)
{
	__HAL_TIM_SET_COMPARE(&TIM_HandleSG90,TIM_CHANNEL_1,Angle / 180 * 2000 + 500);//更改占空比
}


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim -> Instance == TIM1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();               
		__HAL_RCC_TIM1_CLK_ENABLE();
		
		GPIO_InitTypeDef GPIO_InitPA8;

		GPIO_InitPA8.Mode = GPIO_MODE_AF_PP;
		GPIO_InitPA8.Pin = GPIO_PIN_8;
		GPIO_InitPA8.Speed = GPIO_SPEED_FREQ_MEDIUM;

		HAL_GPIO_Init(GPIOA,&GPIO_InitPA8);
		
	}

}