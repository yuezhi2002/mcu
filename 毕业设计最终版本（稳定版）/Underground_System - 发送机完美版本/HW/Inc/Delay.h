#ifndef __DELAY_H
#define __DELAY_H		

//采用8hmz外部高速晶振（hse）9倍频到72MHZ的SYSTEMCLOCK 
void TIM2_Init(void);
void delay_us(uint16_t us);
void delay_ms(uint16_t ms);
#endif  

