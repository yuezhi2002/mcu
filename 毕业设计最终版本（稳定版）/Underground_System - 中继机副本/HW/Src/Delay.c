#include "stm32f1xx_hal.h"

TIM_HandleTypeDef htim2;

void TIM2_Init(void)
{
    __HAL_RCC_TIM2_CLK_ENABLE(); // 启用TIM2时钟

    htim2.Instance = TIM2;                      // TIM2定时器
    htim2.Init.Prescaler = 72 - 1;              // 预分频器（1MHz, 1μs计数一次）
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP; // 向上计数模式
    htim2.Init.Period = 0xFFFF;                 // 自动重装载值（最大值）
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    HAL_TIM_Base_Init(&htim2);                  // 初始化TIM2
    HAL_TIM_Base_Start(&htim2);                 // 启动TIM2
}

void delay_us(uint16_t us)
{
    uint16_t start = __HAL_TIM_GET_COUNTER(&htim2); // 获取当前计数值
    while ((__HAL_TIM_GET_COUNTER(&htim2) - start) < us); // 等待计数差值达到us
}

void delay_ms(uint16_t ms)
{
    for (uint16_t i = 0; i < ms; i++)
    {
        delay_us(1000); // 调用微秒延时函数
    }
}

