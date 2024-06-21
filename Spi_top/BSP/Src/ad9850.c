/*
	注意：这不是ad9850的驱动！！！
	根据文档写的spi_top时序，我也不清楚为什么叫ad9850
*/

#include "ad9850.h"
#include "stm32f1xx_hal.h" 
#include <stdio.h>	
#include "usart.h"

void cycle(void); // 一个时钟周期:
void write8bit(long int w); // 写8位数据；


// 简单的空循环延时函数（基于系统时钟频率 72 MHz）
void Delay_us(uint32_t us) {
    // 根据系统时钟频率计算循环次数
    // 这里假设每个循环大约需要 5 个时钟周期
    uint32_t count = 72  * us;
    while (count--) {
        __NOP();  // 空操作，占用一个时钟周期
    }
}


void delay_ms(uint32_t ms) {
    // 获取当前的SysTick值
    uint32_t start = SysTick->VAL;
    // 计算延时的SysTick数
    uint32_t ticks = ms * (SystemCoreClock / 1000);

    while (ticks > 0) {
        uint32_t current = SysTick->VAL;
        uint32_t elapsed = (start < current) ? start + SysTick->LOAD - current : start - current;
        if (elapsed >= ticks) {
            break;
        }
        ticks -= elapsed;
        start = current;
    }
}

/*
		@Fuction: 发送数据到IC	
*/
void send_data2AD9850(uint32_t* data){
		u8 i = 0;
		AD9850_FQUD_L;
		AD9850_RESET_L;
		delay_ms(200);
		AD9850_FQUD_H;
		AD9850_RESET_H;
	for(i = 0; i<8;i++){
		AD9850_Write(i,data[i]);
	}
	//以下代码是sda读？我不懂，时序没写，按照原程序877行出改写
	uint8_t w = 0x10;
	for(i=0;i<8;i++){
		(w<<i)&0x80 ? AD9850_DATA_H : AD9850_DATA_L;
cycle();
	}
	AD9850_DATA_H;
cycle();
	AD9850_DATA_H;
cycle();
	AD9850_FQUD_L;
	AD9850_DATA_H;
cycle();

	AD9850_FQUD_H;
	for(i=0 ;i<100;i++){
cycle();

	}

}

void AD9850_Write(u8 w0,u32 data)
{
	u8 i;
	uint32_t y,w;
	y=data;
	u8 test = 0;
	/*
		---------------------------
		|Start: 初始化，等待3周期 |
		---------------------------
	*/

	//复位+拉高spi_en
	AD9850_RESET_H;
	AD9850_FQUD_H;

	w=w0;
	//3个周期的等待信号
	for(i=0;i<3;i++)
	{
		AD9850_DATA_L;
cycle();

	}
	/*
		------------------------
		|	Part One ：写地址w0   |
		------------------------
	*/
	//2位操作：01写
	AD9850_DATA_L; // 0
cycle();

	AD9850_DATA_H; // 1
cycle();

	
	//4位地址
	for(i=0;i<4;i++)
	{
		(w << i) & 0x8 ? AD9850_DATA_H : AD9850_DATA_L;
cycle();

	}
	/*
		-------------------------
		|	Part Two ：闲置4个sclk |
		-------------------------
	*/
	//前两个sclk拉低spi_en
	w & 0x01 ? AD9850_DATA_H : AD9850_DATA_L; //个人感觉给什么数据都行
cycle();

	AD9850_FQUD_L;
	w & 0x01 ? AD9850_DATA_H : AD9850_DATA_L; //个人感觉给什么数据都行
cycle();

	AD9850_FQUD_H;
	//后两个spi_en高电位
	w = w0;
	AD9850_DATA_L;
cycle();

	
	AD9850_DATA_L;
cycle();

	
	/*
		-------------------------------
		|	Part Three ：写32位数据data |
		-------------------------------
	*/
	//写w4
	w = y;
	for(i=0;i<8;i++){
		(w<<i)&0x80000000 ? AD9850_DATA_H : AD9850_DATA_L;
cycle();
	}
	//写w3
	w = (y<<8);
	for(i=0;i<8;i++){
		(w<<i)&0x80000000 ? AD9850_DATA_H : AD9850_DATA_L;
cycle();
	}
	//写w2
	w = (y<<16);
	for(i=0;i<8;i++){
		(w<<i)&0x80000000 ? AD9850_DATA_H : AD9850_DATA_L;
cycle();
	}
	//写w1
	w = (y<<24);
	for(i=0;i<8;i++){
		(w<<i)&0x80000000 ? AD9850_DATA_H : AD9850_DATA_L;
cycle();
	}
	/*
		-------------------
		|	End ：拉低spi_en |
		-------------------
	*/
	AD9850_DATA_L;
cycle();

	AD9850_FQUD_L;
	AD9850_DATA_L;
cycle();

	AD9850_FQUD_H;
}


/*
	@Function：时钟sclk走一周期
*/
void cycle(){
	AD9850_WCLK_H;
//		Delay_us(41);
	AD9850_WCLK_L;
//	Delay_us(41);
}
/*
 @Function:	写最高位，一共8
字节数据
*/
void write8bit(long int w){
	u8 test;
	u8 i;
	for(i=0;i<8;i++){
		(w<<i)&0x80000000 ? AD9850_DATA_H : AD9850_DATA_L;
		test = (w << i) & 0x80000000 ? 1 : 0;
		printf("%d",test);
	AD9850_WCLK_H;
	AD9850_WCLK_L;	}
}
