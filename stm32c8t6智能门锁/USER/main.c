#include "stm32f1xx_hal.h"
#include "Rcc_HSE.h"
#include "stdio.h"
#include "iic_init.h" 
#include "oled_init.h" 
#include "ttp229_keyboard.h"
#include "at24c02.h" 
#include "state.h"
#include "duoji_SG90.h"
#include "uart_init.h"
#include "zw_101.h"
#include "TIM_DSQIT.h"
//#include "stm32f1xx_it.h" 
/*电容式触摸矩阵屏
搭载ttp229bsf芯片（模块已经设计好，使用串行通信，且16键可用）
使用PB12作为其scl时钟线接口，PB13作为sdo数据线接口（没使用uart3功能，此矩阵屏是简单的scl配合sda的单线协议）
*/

/*at24c02存储器
使用iic通信。此实验使用iic2，pb10连接存储器的scl，pb11连接存储器的sda
*/

/*舵机
本实验通过tim定时器的oc输出比较功能，通过pwm输出控制舵机的转动角度
f103c8系列是中等容量产品，所以只有一个PWM定时器（还有 3个串口，三个普通定时器，两个iic，两个SPI）
本实验使用高级定时器TIM1的通道1进行舵机控制（单片机的PinA8引脚）
*/

/*指纹模块
串口通信，使用uart1，pa9连接指纹模块的rx，pa10连接指纹模块的tx。指纹模块默认波特率为57600bps
绿线接vcc。黄线为uart_tx,接单片机串口的rx。黑线为uart_rx,接单片机串口的tx。红线接GND。其余两根线不接。

电脑修改指纹模块基础设置接线：指纹模块的tx接串口的rx，模块的rx接串口的tx。将串口3.3v和vcc短接，然后GND接单片机负极。指纹模块vcc接单片机正极，GND接单片机负极

此指纹模块串口通信校验码：除了包头和芯片地址，其余相加就是校验码
*/
//电容键盘的等待按键松开需要思考，目前用延时阻塞300ms保证300ms内只触发一次

//需要在zw模块的验证指纹函数里面加一个定时器的计时功能，因为如果手指大概八秒还不放上去，就会卡死，所以要计时，提前跳出

void main()
{
	HAL_Init();
	Rcc_HSE();
	at24c02_init();      //存储器iic初始化
	OLED_Init();         //oled显示屏初始化
	keyboard_gpio_init();//串行通信键盘初始化
//	TIM2_DSQ_IT_INIT();//定时器2配置初始化
//	SG90_Control();//舵机控制初始化
//	Servo_SetAngle(0);
	zw_init(57600);//uart1_init(57600);
	
	
	while(1)
	{
	State_Door();
	}

}
