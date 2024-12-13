#include "stm32f1xx_hal.h"
#include "Rcc_HSE.h"
#include "stdio.h"
#include "iic_init.h" 
#include "oled_init.h" 
#include "ttp229_keyboard.h"
#include "at24c02.h" 
#include "duoji_SG90.h" 
#include "state.h"
#include "zw_101.h"
#include "uart_init.h"
#include "TIM_DSQIT.h"
/*
状态控制：
1.第一次上电，初始密码是1234
2.登录：此时可以直接输入密码，也能随时将手放在指纹识别器上，开锁。密码只有三次机会，指纹次数无所谓
3.系统：需要先输入密码才能进入，可以修改登录密码和管理指纹.

3.系统->管理指纹：添加指纹，删除指纹
*/
/*--------------------------------------------------------------------------------------------------------------------*/

static uint8_t state;  // 0：上电唤醒的初始状态（此状态可以选择登录和系统选项）
                   // 1：表示进入了登录状态
                   // 2：表示进入了系统修改状态
                   // 3: 表示密码正确了，开锁
                   // 4: 表示进入修改密码的状态
									 // 5：表示能进入修改指纹状态
									 // 6: 表示添加新指纹
									 // 7: 表示删除指纹
									 //
									 //
static uint8_t OLEDClear_State = 0;          //用来防止重复清屏的量
static uint8_t num = 0;                 //表示密码输入时候输入的数字数量
uint8_t readkey;                     //用来读取按键的值
uint16_t value;                       //用来将LOGIN函数中的密码输入的四个数字变成一个十进制的4位数  ,要注意范围，所以uint16_t 
uint8_t Value[4] = {0};       // LOGIN()函数中用来临时存储输入的密码的数组
static uint8_t state0 = 0;//在状态0中ENTER_STATE()函数中，用来离开其while函数的标志量
static uint8_t state2 = 0;//在状态2中SYSTEM()函数中，用来离开其while函数的标志量
static uint8_t state5 = 0;//在状态5中SET_Pringer()函数中，用来离开其while函数的标志量
uint8_t state1_2;//比如ENTER_STATE()函数和SYSTEM()函数的其中一个标志量

static uint8_t num_N = 0;                 //表示新密码输入时候输入的数字数量
uint16_t value_N;               //用来将SYSTEM_SET_PASSWORD()函数中的密码输入的四个数字变成一个十进制的4位数  ,要注意范围，所以uint16_t 
uint8_t Value_N[4] = {0};       // SYSTEM_SET_PASSWORD()函数中用来临时存储输入的密码的数组




			uint8_t x = 0;
/* 函    数：登录初始界面.
 * 参    数：
 * 返 回 值：
 * 注意事项：当 state 静态状态量 state 等于0时 
 *					此函数主要是显示两个选项：登录；系统。并且能够选择，同时更改state状态量
 *             
 *
 */
void ENTER_STATE()
{
	value_N = 0;value = 0;
	state2 = 0;
	
	while(state == 0)
	{
		OLED_Clear();   
	
		OLEDClear_State = 3;
	
		while(state0 == 0)                 //只要状态量state2等于0，就不会离开这个while函数，修改state2的值即可离开此while循环
		{
			readkey = TTP229_ReadKey();
			HAL_Delay(300);                            //!!!!!!!!!!!!!!!!!!!!!!!!!等待按键松开还需要继续想
		
			if(state1_2 % 2 == 0)              //如果state1对2取余等于0，
			{
				if(OLEDClear_State != 0)      //状态不变则只执行一次清屏的操作，不然在while循环里面会一直清屏闪烁
				{
					OLED_Clear();                   
					OLEDClear_State = 0;
					OLED_ShowString(2, 6, "log in");
					state = 1;   
				}
                                 //state状态量改变，但是无法跳出while循环
			}
		
			else if(state1_2 % 2 == 1)                        //如果state1对2取余等于1，
			{
				if(OLEDClear_State != 1)
				{
					OLED_Clear();                    
					OLEDClear_State = 1;
					OLED_ShowString(2, 6, "system");
					state = 2;
				}

			}
		
			if(readkey == 67){state1_2++;}              //按下c按键,state1的状态改变,屏幕分别显示login和system选项
			else if(readkey == 65){state0 = 1;}       //按下A按键,state2的状态改变,代表离开这个while循环并且进入另一个状态
	}
	}

}


/* 函    数：登录函数
 * 参    数：
 * 返 回 值：
 * 注意事项：进入条件，state状态量为1；退出条件state状态量为3
 */

void LOGIN()
{
while(state == 1)//当总控状态量等于1时，一直循环执行此内容
{
	
			OLED_Clear();				
			OLED_ShowString(1,5,"PUT FINGER");


			check_finger();		//如果不把指纹放上去，函数出不来，直接卡死
			uint8_t fg = check_finger();
			switch(fg)
			{			
			case 2:
				OLED_Clear();				
				OLED_ShowString(2,5,"SUCCESS");
				HAL_Delay(2000);
				//开锁，
				state = 0;
				num = 5;
				break;

					
			case 3:
				OLED_Clear();				
				OLED_ShowString(2,5,"ERROR");
				HAL_Delay(1000);
				num = 0;
				state = 0;
				break;
			
			case 4:
				OLED_Clear();				
				OLED_ShowString(2,5,"TIMEOUT");
				HAL_Delay(1000);
				num = 5;
				state = 0;
			}
	                                        

	while (num < 4) 
	{
		
		if(OLEDClear_State != 1)
		{
			OLED_Clear();
			OLEDClear_State = 1;
			
			OLED_ShowString(1,1,"enter");
			OLED_ShowString(2,1,"password");
		}
		
    readkey = TTP229_ReadKey(); // 调用按键读取函数

		if (readkey != 0) //如果之前读取有键值 
		{
			Value[num] = readkey;   // 存储按键值并移至下一个位置		
			OLED_ShowNum(4,num+1,Value[num],1);
			
			HAL_Delay(300);         // 延时300ms防止重复触发		
			num++;                  //num++,准备下一个数字
						
			if(readkey == 66)//按B全选删除,并且回到num = 0,重新输入
			{
				num = 0;
				Value[0] = 0;Value[1] = 0;Value[2] = 0;Value[3] = 0;
				OLED_ShowNum(4,1,0,4);
			}		
		}

	}		
	
	
	
	while(num == 4)
	{
		if(OLEDClear_State != 2)
		{
			OLED_Clear();
			OLED_ShowString(2,4,"enter by A");
			OLEDClear_State = 2;                       //        防止重复清屏导致闪烁
			
			value = Value[0] * 1000 + Value[1] * 100 + Value[2] * 10 + Value[3];//将输入的四数字生成一个十进制的四位数，方便比对
		}
		
	
		if(TTP229_ReadKey() == 65 && value !=  at24c02_readpage(0xA0))//当密码输入错误时
		{
			if(OLEDClear_State != 3)
			{
				OLED_Clear();

				OLED_ShowNum(1,1,value,5);

				OLED_ShowString(2,5,"ERROR");
				OLED_ShowString(3,5,"try again");
				HAL_Delay(1000);
				OLEDClear_State = 3;//表示只执行一次，防止重复执行OLED_Clear导致屏幕闪烁
				num = 0;            //让num = 1，回到此函数第一个while循环重新输入密码
			}
		}
		if(TTP229_ReadKey() == 65 && value ==  at24c02_readpage(0xA0))//如果按下A号键并且输入的与存储的密码一致
		{
			
			if(OLEDClear_State != 4)
			{
			OLED_Clear();	
			OLED_ShowString(2,3,"Deblocking");
			OLED_ShowString(3,5,"Success!");
			OLEDClear_State = 4;//表示只执行一次，防止重复执行OLED_Clear导致屏幕闪烁
			HAL_Delay(1000);
			}
			num = 5;               //密码正确了，让num等于5跳出循环
			state = 3;             //密码正确了，修改状态量
			HAL_Delay(1000);
			state = 0;
		}	
	}			
}
}	


/* 函    数：系统设置函数
 * 参    数：
 * 返 回 值：
 * 注意事项：进入条件，总控状态量state为2； 离开条件：总控状态量state为0时，返回主菜单
 */

void SYSTEM()
{
	while(state == 2)
	{
		state1_2 = 2;//稍后让他对2取余，判断0或1，用来控制修改密码或者指纹的状态
		uint8_t OLEDClear_State1 = 3;
	
		while(state2 == 0)                 //只要状态量state3等于0，就不会离开这个while函数，修改state3的值即可离开此while循环
		{
			readkey = TTP229_ReadKey();
			HAL_Delay(300);                           
		
			if(state1_2 % 2 == 0)              //如果state1对2取余等于0，
			{
				if(OLEDClear_State != 0)      //状态不变则只执行一次清屏的操作，不然在while循环里面会一直清屏闪烁
				{
					OLED_Clear();  
					OLED_ShowString(1, 1, "SET PASSWORD");
					OLED_ShowString(2, 1, "SET FINGER");
					OLEDClear_State = 0;
				}
				OLED_ShowString(1, 14, "<-");
				state = 4;                                    //state状态量改变，但是无法跳出while循环
			}
			
			else if(state1_2 % 2 == 1)                        //如果state1对2取余等于1，
			{
				if(OLEDClear_State != 1)
				{
					OLED_Clear();  
					OLED_ShowString(1, 1, "SET PASSWORD");
					OLED_ShowString(2, 1, "SET FINGER");					
					OLEDClear_State = 1;
				}
				OLED_ShowString(2, 14, "<-");
				state = 5;
			}
		
			if(readkey == 67){state1_2++;}              //按下c按键,state1的状态改变,屏幕分别显示login和system选项
			else if(readkey == 65){state2 = 1;}       //按下A按键,state3的状态改变,代表离开这个while循环并且进入另一个状态
	}
	}   
}		


/* 函    数：系统函数中密码修改函数
 * 参    数：
 * 返 回 值：
 * 注意事项：state等于4时进入，等于0时表示设置完成返回主菜单
 */
void SYSTEM_SET_PASSWORD()
{
	while(state == 4)
	{
		OLED_Clear();			

		while (num_N < 4) 
		{
			if(OLEDClear_State != 7)
			{
				OLED_Clear();
				OLEDClear_State = 7;
				OLED_ShowString(1,1,"new password");
			}
			readkey = TTP229_ReadKey(); // 调用按键读取函数
			if (readkey != 0) //如果之前读取有键值 
			{
				Value_N[num_N] = readkey;   // 存储按键值并移至下一个位置		
				OLED_ShowNum(4,num_N+1,Value_N[num_N],1);
			
				HAL_Delay(300);         // 延时300ms防止重复触发		
				num_N++;                  //num++,准备下一个数字
			}
		}	
	
		while(num_N == 4)
		{
			if(OLEDClear_State != 5)
			{
				OLED_Clear();
				OLED_ShowString(2,4,"ensure by A");
				OLEDClear_State = 5;                       //        防止重复清屏导致闪烁
			
				value_N = Value_N[0] * 1000 + Value_N[1] * 100 + Value_N[2] * 10 + Value_N[3];//将输入的四数字生成一个十进制的四位数，方便比对
			}
			if(TTP229_ReadKey() == 65)//如果按下A确认
			{
				if(OLEDClear_State != 6)
				{
					OLED_Clear();
					OLED_ShowString(2,4,"SUCCESS");
					OLEDClear_State = 6;                       //        防止重复清屏导致闪烁
					at24c02_writepage(0xA0,value_N);	
					HAL_Delay(1000); 
				}	
				
				state = 0;
				num_N = 5;
			}
		}
	}	
}

/* 函    数：修改指纹控制函数（添加，删除）
 * 参    数：
 * 返 回 值：
 * 注意事项：当总状态量state等于5时进入此函数，当状态量等于0时，返回主菜单
 */

void SYSTEM_SET_FINGER()
{
	while(state == 5)
	{
		state1_2 = 2;//稍后让他对2取余，判断0或1，用来控制修改密码或者指纹的状态
		OLEDClear_State = 3;
		
		while(state5 == 0)                 //只要状态量state3等于0，就不会离开这个while函数，修改state3的值即可离开此while循环
		{
			readkey = TTP229_ReadKey();
			HAL_Delay(300);                           
	
			if(state1_2 % 2 == 0)              //如果state1对2取余等于0，
			{
				if(OLEDClear_State != 0)      //状态不变则只执行一次清屏的操作，不然在while循环里面会一直清屏闪烁
				{
					OLED_Clear();  
					OLED_ShowString(1, 1, "Add Finger");
					OLED_ShowString(2, 1, "Del Finger");
					OLEDClear_State = 0;
				}
				OLED_ShowString(1, 14, "<-");
				state = 6;                                    //state状态量改变，但是无法跳出while循环
			}
			
			else if(state1_2 % 2 == 1)                        //如果state1对2取余等于1，
			{
				if(OLEDClear_State != 1)
				{
					OLED_Clear();  
					OLED_ShowString(1, 1, "ADD Finger");
					OLED_ShowString(2, 1, "DEL Finger");			
					OLEDClear_State = 1;
				}
				OLED_ShowString(2, 14, "<-");
				state = 7;
			}
		
			if(readkey == 67){state1_2++;}              //按下c按键,state1的状态改变,屏幕分别显示ADD和DEL选项
			else if(readkey == 65){state5 = 1;}       //按下A按键,state3的状态改变,代表离开这个while循环并且进入另一个状态
	}
	}
}

/* 函    数：添加指纹函数
 * 参    数：
 * 返 回 值：
 * 注意事项：
 */

//void SYSTEM_ADD_FINGER()
//{
//	uint8_t adFngrNum = 0;
//	uint8_t AdFngrNum = 0;
//	while(state == 6)
//	{
//		OLED_Clear();
//		OLED_ShowString(1,1,"fngr.No1");
//		OLED_ShowString(2,1,"fngr.No2");	
//		OLED_ShowString(3,1,"fngr.No3");			
//		
//		while(state6 != 1)
//		{
//			if(TTP229_ReadKey() == 67)
//			{
//				OLED_ShowString(adFngrNum,14,"<-");
//				adFngrNum++;
//				AdFngrNum++;
//				if(adFngrNum > 3){adFngrNum = 0;}
//				if(AdFngrNum > 3){AdFngrNum = 0;}
//			}
//			if(TTP229_ReadKey() == 65){state6 = 1;}
//		switch (AdFngrNum)
//		{
//			case 0:
//			case 1:
//			case 2: 
//		}
//		
//		}	
//	
//	}
//}












/*-------------------------------------------------------------------------------------------------------------------------*/	
/* 函    数：状态总控函数
 * 参    数：无
 * 返 回 值：无
 * 注意事项：1.
*            
 *           
 *
 *
 *
 *
 */
void State_Door()
{
	ENTER_STATE();
	LOGIN();//
	SYSTEM();
	SYSTEM_SET_PASSWORD();
	SYSTEM_SET_FINGER();
}	
	
	
