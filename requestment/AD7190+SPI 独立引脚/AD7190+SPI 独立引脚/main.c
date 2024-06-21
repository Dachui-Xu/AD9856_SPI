#include<LQ12864.h>
#include<reg52.h>
#include<stdio.h>
#include <string.h>
#include<SPI_WR32bit.h>



// 定义串口参数
#define RX_ENABLE 0x10 // 允许串口接收
#define TX_ENABLE 0x20 // 允许串口发送
#define BUFFER_SIZE 256
#define COMMAND_START 0x706C6179  //开始将启动SPI控制命令
#define COMMAND_DATA 0x64617461		//展示数据命令

 // 定义全局变量
unsigned char received_byte;
unsigned char RxBuffer[BUFFER_SIZE];   //接收数据
unsigned char aRxBuffer;			//接收中断缓冲
unsigned char Uart1_Rx_Cnt = 0;		//接收缓冲计数
ulong datatem[8] = {
				0xFFFFFFFF,
				0x04600847,
				0x19DF48C4,
				0x00000000,
				0x21D00000,
				0x00000152,
				0x6800006C,
				0x00000000
};

 void weiteIC(ulong *datatem){
   		ulong w;
  		int i = 0;
		ad9850_fq_up=0;
		ad9850_rest=0;
		delay(200);
		ad9850_fq_up=1;
		ad9850_rest=1;
	
	for(i = 0; i<8;i++){
		ad9850_wr_serial(i,datatem[i]);
	}
	

								 // 读SPI，放瘀血
		w=0x10;   
		for(i=0;i<8;i++)
			{
			ad9850_bit_data=(w<<i)&0x80;
			ad9850_w_clk=1;
			ad9850_w_clk=0;
			}	 

		ad9850_bit_data=1;
		ad9850_w_clk=1;
		ad9850_w_clk=0;

  	    ad9850_bit_data=1;
		ad9850_w_clk=1;
		ad9850_w_clk=0;

		ad9850_fq_up=0;
		ad9850_bit_data=1;
		ad9850_w_clk=1;
		ad9850_w_clk=0;
		ad9850_fq_up=1;	

		for(i=0;i<100;i++)
			{
			ad9850_w_clk=1;
			ad9850_w_clk=0;
			}					  //
 }

// 串口初始化函数
void UART_Init() {
    SCON = 0x50; // 8位数据, 1位停止位, 不使用奇偶校验
    TMOD &= 0x0F; // 清除低四位
    TMOD |= 0x20; // 设置定时器1为模式2（8位自动重载）
    TH1 = 0xfd;   // 根据下面的公式计算得到
 	TL1 = 0xfd; //设置比特率9600率
    TR1 = 1; // 启动定时器1
    ES = 1; // 使能串口中断
    EA = 1; // 使能总中断
}
void UART_SendByte(uchar byte) {
    SBUF = byte; // 发送数据
    while (!TI); // 等待发送完成
    TI = 0; // 清除发送中断标志
}

void UART_SendString(const char *str) {
    while (*str) {
        UART_SendByte(*str++);
    }
}

/*
	@function:把帧数据存储到data中										
*/
void process_frame(unsigned char *frame, ulong *datatem) {
    unsigned char reg = frame[0];
    ulong datatemp = (frame[1] << 24) | (frame[2] << 16) | (frame[3] << 8) | frame[4];
		datatem[reg] = datatemp;
		printf("Received data (hex) in reg_bank[%d]: 0x%08X\n", reg ,datatem[reg]);
}

/*
	@fuction:打印data中的数据
*/
void printf_data(){
		int i = 0;
		for(;i<8;i++){
		printf("reg:%d, 0x%08X\n",i,datatem[i]);		
		}
		printf("---------------\n");
}
/*
	@function:处理命令
*/
void process_command(unsigned char *frame,ulong *datatem) {
     ulong datatemp = (frame[1] << 24) | (frame[2] << 16) | (frame[3] << 8) | frame[4];
    switch (datatemp) {
        case COMMAND_START:
            weiteIC(datatem);
            break;
        case COMMAND_DATA:
            printf_data();
            break;
        default:
            printf("Error control signal: 0x%08X\n", datatem);
            break;
    }
}






void main()
{

	while(1){
		weiteIC(datatem);	
	}
 
		
}



