#include "stm32f1xx_hal.h"
#include "Oled.h" 
#include "iic_init.h" 
#include "DHT11.h"
#include "Delay.h"
#include "MQ4.h"
//ѡ��PA2������ΪA0��ģ�����룩
ADC_HandleTypeDef         hadc1;     // ����ADC1���
// MQ4 ģ���ѹ�����ADC��ʼ��
void MQ4_ADC_Init(void)
{
    ADC_ChannelConfTypeDef sConfig;
    
    /* ���� ADC1 ȫ�ֲ��� */
    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;          // ��ͨ���ɼ�
    hadc1.Init.ContinuousConvMode = ENABLE;              // ����ת��ģʽ
    hadc1.Init.DiscontinuousConvMode = DISABLE;           
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;    // �������
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
     				OLED_ShowNum(3, 1, 222, 8);
    }
    
    /* ���� ADC ����ͨ����ʹ�� PA1 ��Ӧ�� ADC ͨ�������� ADC_CHANNEL_1�� */
    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = ADC_REGULAR_RANK_1; // ��ֱ��д 1
    sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;; // �ɸ�����Ҫѡ�����ʱ��
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
							OLED_ShowNum(3, 1, 333, 8);
    }
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    if(hadc->Instance == ADC1)
    {
        /* ʹ�� ADC1 ʱ�� */
        __HAL_RCC_ADC1_CLK_ENABLE();
        
        /* ʹ����ʹ�õ� GPIOA ʱ�� */
        __HAL_RCC_GPIOA_CLK_ENABLE();
        
        /* ���� PA1 Ϊģ������ */
        GPIO_InitStruct.Pin = GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}
int adcValue = 0;
float voltage;
uint16_t MQ4_GO()
{
	HAL_ADC_Start(&hadc1);
	
	uint16_t ppm = 0;
	
 /* ������ʽ�ȴ� ADC ת����ɣ���ʱʱ����Ϊ 100ms */
 if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
		{
		/* ��ȡ ADC ת����� */
		adcValue = HAL_ADC_GetValue(&hadc1);
		
		voltage = (float)adcValue * 3.3 / 4096;          // ���ݲɼ���ֵ�����ѹֵ������ο���ѹΪ 3.3V��12λ ADC��voltage = (adcValue / 4095) * 3.3 
    HAL_ADC_Stop(&hadc1); // ֹͣADC
//		ppm = RS/R0*(voltage/3.3f-1)
			
		ppm = (voltage - 0.5) / 0.1 *200;
		                                                                                 
		return ppm;
   }
}
     
