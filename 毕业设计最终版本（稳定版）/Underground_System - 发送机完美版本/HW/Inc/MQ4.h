#ifndef __MQ4_H
#define __MQ4_H 

extern ADC_HandleTypeDef hadc1;    

void MQ4_ADC_Init(void);

uint16_t MQ4_GO();

#endif

