#include "stm32f1xx_hal.h"

TIM_HandleTypeDef htim2;

void TIM2_Init(void)
{
    __HAL_RCC_TIM2_CLK_ENABLE(); // ����TIM2ʱ��

    htim2.Instance = TIM2;                      // TIM2��ʱ��
    htim2.Init.Prescaler = 72 - 1;              // Ԥ��Ƶ����1MHz, 1��s����һ�Σ�
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP; // ���ϼ���ģʽ
    htim2.Init.Period = 0xFFFF;                 // �Զ���װ��ֵ�����ֵ��
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    HAL_TIM_Base_Init(&htim2);                  // ��ʼ��TIM2
    HAL_TIM_Base_Start(&htim2);                 // ����TIM2
}

void delay_us(uint16_t us)
{
    uint16_t start = __HAL_TIM_GET_COUNTER(&htim2); // ��ȡ��ǰ����ֵ
    while ((__HAL_TIM_GET_COUNTER(&htim2) - start) < us); // �ȴ�������ֵ�ﵽus
}

void delay_ms(uint16_t ms)
{
    for (uint16_t i = 0; i < ms; i++)
    {
        delay_us(1000); // ����΢����ʱ����
    }
}

