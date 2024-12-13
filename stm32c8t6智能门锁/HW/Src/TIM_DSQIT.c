#include "stm32f1xx_hal.h"
#include "TIM_DSQIT.h"
#include "OLED_init.h" 
#include "iic_init.h" 
//目标，设置一个能倒计时5秒的定时器中断函数，开关不在此函数里，使用TIM2；

/*实验背景：已经使用了8MHZ的外部高速时钟作为单片机时钟，且SYSTEMCLOCK设置为72MHZ（最大）
            且APB1外设已经被我设置成了36MHZ（最大），所以定时器2—4的时钟频率为36MHZ，定时器1为高级定时器与APB2时钟有关。
						*/




////	HAL_TIM_Base_Start_IT(&TIM2TypeDef);//打开定时器，设置为5s就触发中断
		
//		HAL_TIM_Base_Stop_IT(&TIM2TypeDef);//关闭定时器		
		
TIM_HandleTypeDef TIM2TypeDef;

void TIM2_DSQ_IT_INIT()
{
	__HAL_RCC_TIM2_CLK_ENABLE();
	
	TIM2TypeDef.Instance = TIM2;
	TIM2TypeDef.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIM2TypeDef.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;//是否开启自动重装载(计数一轮完成后继续计数),这里不需要，因为我们只需要计数一次就好
	TIM2TypeDef.Init.Prescaler =7200;
	TIM2TypeDef.Init.Period = 30000;
	//TIM2TypeDef.Init.RepetitionCounter  = 重复计数值，仅高级定时器有，不需要用
	TIM2TypeDef.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //滤波器，为1，不滤。
	TIM2TypeDef.Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;//4个通道的选择，（仅输出比较博士和输入捕获模式设置，普通中断不需要）
	
	HAL_TIM_Base_Init(&TIM2TypeDef);

	HAL_NVIC_SetPriority(TIM2_IRQn,1,0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	
	__HAL_TIM_CLEAR_FLAG(&TIM2TypeDef,TIM_FLAG_UPDATE);
	
//		HAL_TIM_Base_Start_IT(&TIM2TypeDef);不在此处打开
	
	
}




void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim -> Instance == TIM2)
	{

	}

}
