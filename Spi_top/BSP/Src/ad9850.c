/*
	ע�⣺�ⲻ��ad9850������������
	�����ĵ�д��spi_topʱ����Ҳ�����Ϊʲô��ad9850
*/

#include "ad9850.h"
#include "stm32f1xx_hal.h" 
#include <stdio.h>	
#include "usart.h"

void cycle(void); // һ��ʱ������:
void write8bit(long int w); // д8λ���ݣ�


// �򵥵Ŀ�ѭ����ʱ����������ϵͳʱ��Ƶ�� 72 MHz��
void Delay_us(uint32_t us) {
    // ����ϵͳʱ��Ƶ�ʼ���ѭ������
    // �������ÿ��ѭ����Լ��Ҫ 5 ��ʱ������
    uint32_t count = 72  * us;
    while (count--) {
        __NOP();  // �ղ�����ռ��һ��ʱ������
    }
}


void delay_ms(uint32_t ms) {
    // ��ȡ��ǰ��SysTickֵ
    uint32_t start = SysTick->VAL;
    // ������ʱ��SysTick��
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
		@Fuction: �������ݵ�IC	
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
	//���´�����sda�����Ҳ�����ʱ��ûд������ԭ����877�г���д
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
		|Start: ��ʼ�����ȴ�3���� |
		---------------------------
	*/

	//��λ+����spi_en
	AD9850_RESET_H;
	AD9850_FQUD_H;

	w=w0;
	//3�����ڵĵȴ��ź�
	for(i=0;i<3;i++)
	{
		AD9850_DATA_L;
cycle();

	}
	/*
		------------------------
		|	Part One ��д��ַw0   |
		------------------------
	*/
	//2λ������01д
	AD9850_DATA_L; // 0
cycle();

	AD9850_DATA_H; // 1
cycle();

	
	//4λ��ַ
	for(i=0;i<4;i++)
	{
		(w << i) & 0x8 ? AD9850_DATA_H : AD9850_DATA_L;
cycle();

	}
	/*
		-------------------------
		|	Part Two ������4��sclk |
		-------------------------
	*/
	//ǰ����sclk����spi_en
	w & 0x01 ? AD9850_DATA_H : AD9850_DATA_L; //���˸о���ʲô���ݶ���
cycle();

	AD9850_FQUD_L;
	w & 0x01 ? AD9850_DATA_H : AD9850_DATA_L; //���˸о���ʲô���ݶ���
cycle();

	AD9850_FQUD_H;
	//������spi_en�ߵ�λ
	w = w0;
	AD9850_DATA_L;
cycle();

	
	AD9850_DATA_L;
cycle();

	
	/*
		-------------------------------
		|	Part Three ��д32λ����data |
		-------------------------------
	*/
	//дw4
	w = y;
	for(i=0;i<8;i++){
		(w<<i)&0x80000000 ? AD9850_DATA_H : AD9850_DATA_L;
cycle();
	}
	//дw3
	w = (y<<8);
	for(i=0;i<8;i++){
		(w<<i)&0x80000000 ? AD9850_DATA_H : AD9850_DATA_L;
cycle();
	}
	//дw2
	w = (y<<16);
	for(i=0;i<8;i++){
		(w<<i)&0x80000000 ? AD9850_DATA_H : AD9850_DATA_L;
cycle();
	}
	//дw1
	w = (y<<24);
	for(i=0;i<8;i++){
		(w<<i)&0x80000000 ? AD9850_DATA_H : AD9850_DATA_L;
cycle();
	}
	/*
		-------------------
		|	End ������spi_en |
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
	@Function��ʱ��sclk��һ����
*/
void cycle(){
	AD9850_WCLK_H;
//		Delay_us(41);
	AD9850_WCLK_L;
//	Delay_us(41);
}
/*
 @Function:	д���λ��һ��8
�ֽ�����
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
