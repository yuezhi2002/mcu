#ifndef __DHT11_H
#define __DHT11_H


#define DHT11_GPIO_PORT          GPIOA
#define DHT11_GPIO_PIN           GPIO_PIN_5

#define DHT11_READ               HAL_GPIO_ReadPin(DHT11_GPIO_PORT,DHT11_GPIO_PIN)
#define DHT11_HIGH               HAL_GPIO_WritePin(DHT11_GPIO_PORT,DHT11_GPIO_PIN,GPIO_PIN_SET)
#define DHT11_LOW                HAL_GPIO_WritePin(DHT11_GPIO_PORT,DHT11_GPIO_PIN,GPIO_PIN_RESET)


typedef struct
{
	uint8_t Temp_H;
	uint8_t Temp_L;
	uint8_t Humi_H;
	uint8_t Humi_L;
}DHT11_Data;

void DHT11_Init();
uint8_t DHT11_CheckState();
uint8_t DHT11_ReadByte();
uint16_t DHT11_ReadData(DHT11_Data *data);
void DHT11_Free();



#endif