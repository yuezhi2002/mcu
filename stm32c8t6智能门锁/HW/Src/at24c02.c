#include "stm32f1xx_hal.h"
#include "iic_init.h" 
#include "at24c02.h" 

void at24c02_init()
{
	iic2_init();
}

/* 函    数：at24c02_write 存储数据函数
 * 参    数：SP:写入数据的起始位置
 *           Data：要发送的数据        
 *
 * 返 回 值：无
 * 注意事项：24c02是2k的内存，2kb == 2048bit，2048bit/8bit == 256个字节，所以能存储256字节的数据；根据数据手册，一次性最多传输一页(8字节)的数据，多了就要分开传输
 *           这个函数是一次直接写一页，（八个字节），
 *           ①24c02是2k的内存，而一个字节是8bit，所以24c02能存储 2k/8bit == 256个字节的数据
 * 					 ②24c02单次操作最多能存储一页数据，多了需要多次操作函数，24c02能写32页。
 *           ③每个页的起始地址0x00，0x08，0x10，0x18，0x20，0x28，0x30，0x38，0x40，0x48 ( 1到10页)
 *                             0x50，0x58，0x60，0x68，0x70，0x78，0x80，0x88，0x90，0x98（11到20页）
 *                             0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8（21到30页）
 *                             0xF0, 0xF8                                                （31，32页）
 * 
 * 
 *          
 * 
 * 
 * 1.   
 *      
 * 
 * 2.   0xA0的地址是用来存储密码，注册密码或者修改密码都在此地址中进行
 *      
 * 3.   0x50的地址表示存储密码输入错误的次数，如果大于三次，小于三次
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */

void at24c02_writepage(SP,data)
{
	uint32_t pDataPointer = data;
	HAL_I2C_Mem_Write(&I2C_Handleiic2,0xA0,SP,I2C_MEMADD_SIZE_8BIT,&pDataPointer,8,5000);
	HAL_Delay(10);
	
}

/* 函    数：at24c02_read 提取数据函数
 * 参    数：SP:读取数据的起始位置（在24c02芯片内存中的起始地址）
 *
 * 返 回 值：从24c02特定地址读取的数据（uint32_t）
 * 注意事项：24c02是2k的内存，2k/8bit == 256字节，所以能存储256字节的数据；根据数据手册，一次性最多传输一页(8字节)的数据，多了就要分开传输
 */
uint32_t at24c02_readpage(SP)
{
	uint16_t rDataPointer;
	HAL_I2C_Mem_Read(&I2C_Handleiic2,0xA0,SP,I2C_MEMADD_SIZE_8BIT,&rDataPointer,8,5000);
	return rDataPointer;
}