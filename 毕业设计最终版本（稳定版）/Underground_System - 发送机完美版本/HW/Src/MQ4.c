#include "stm32f1xx_hal.h"
#include "Oled.h" 
#include "iic_init.h" 
#include "DHT11.h"
#include "Delay.h"
#include "MQ4.h"
//选择PA2引脚作为A0（模拟输入）
ADC_HandleTypeDef         hadc1;     // 定义ADC1句柄
// MQ4 模拟电压输入的ADC初始化
void MQ4_ADC_Init(void)
{
    ADC_ChannelConfTypeDef sConfig;
    
    /* 配置 ADC1 全局参数 */
    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;          // 单通道采集
    hadc1.Init.ContinuousConvMode = ENABLE;              // 连续转换模式
    hadc1.Init.DiscontinuousConvMode = DISABLE;           
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;    // 软件触发
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
     				OLED_ShowNum(3, 1, 222, 8);
    }
    
    /* 配置 ADC 正则通道：使用 PA1 对应的 ADC 通道（例如 ADC_CHANNEL_1） */
    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = ADC_REGULAR_RANK_1; // 或直接写 1
    sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;; // 可根据需要选择采样时间
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
        /* 使能 ADC1 时钟 */
        __HAL_RCC_ADC1_CLK_ENABLE();
        
        /* 使能所使用的 GPIOA 时钟 */
        __HAL_RCC_GPIOA_CLK_ENABLE();
        
        /* 配置 PA1 为模拟输入 */
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
	
 /* 阻塞方式等待 ADC 转换完成，超时时间设为 100ms */
 if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
		{
		/* 获取 ADC 转换结果 */
		adcValue = HAL_ADC_GetValue(&hadc1);
		
		voltage = (float)adcValue * 3.3 / 4096;          // 根据采集的值计算电压值（假设参考电压为 3.3V、12位 ADC）voltage = (adcValue / 4095) * 3.3 
    HAL_ADC_Stop(&hadc1); // 停止ADC
//		ppm = RS/R0*(voltage/3.3f-1)
			
		ppm = (voltage - 0.5) / 0.1 *200;
		                                                                                 
		return ppm;
   }
}
     
