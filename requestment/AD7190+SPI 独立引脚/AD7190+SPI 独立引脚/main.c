#include<LQ12864.h>
#include<reg52.h>
#include<stdio.h>
#include <string.h>
#include<SPI_WR32bit.h>



// ���崮�ڲ���
#define RX_ENABLE 0x10 // �����ڽ���
#define TX_ENABLE 0x20 // �����ڷ���
#define BUFFER_SIZE 256
#define COMMAND_START 0x706C6179  //��ʼ������SPI��������
#define COMMAND_DATA 0x64617461		//չʾ��������

 // ����ȫ�ֱ���
unsigned char received_byte;
unsigned char RxBuffer[BUFFER_SIZE];   //��������
unsigned char aRxBuffer;			//�����жϻ���
unsigned char Uart1_Rx_Cnt = 0;		//���ջ������
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
	

								 // ��SPI������Ѫ
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

// ���ڳ�ʼ������
void UART_Init() {
    SCON = 0x50; // 8λ����, 1λֹͣλ, ��ʹ����żУ��
    TMOD &= 0x0F; // �������λ
    TMOD |= 0x20; // ���ö�ʱ��1Ϊģʽ2��8λ�Զ����أ�
    TH1 = 0xfd;   // ��������Ĺ�ʽ����õ�
 	TL1 = 0xfd; //���ñ�����9600��
    TR1 = 1; // ������ʱ��1
    ES = 1; // ʹ�ܴ����ж�
    EA = 1; // ʹ�����ж�
}
void UART_SendByte(uchar byte) {
    SBUF = byte; // ��������
    while (!TI); // �ȴ��������
    TI = 0; // ��������жϱ�־
}

void UART_SendString(const char *str) {
    while (*str) {
        UART_SendByte(*str++);
    }
}

/*
	@function:��֡���ݴ洢��data��										
*/
void process_frame(unsigned char *frame, ulong *datatem) {
    unsigned char reg = frame[0];
    ulong datatemp = (frame[1] << 24) | (frame[2] << 16) | (frame[3] << 8) | frame[4];
		datatem[reg] = datatemp;
		printf("Received data (hex) in reg_bank[%d]: 0x%08X\n", reg ,datatem[reg]);
}

/*
	@fuction:��ӡdata�е�����
*/
void printf_data(){
		int i = 0;
		for(;i<8;i++){
		printf("reg:%d, 0x%08X\n",i,datatem[i]);		
		}
		printf("---------------\n");
}
/*
	@function:��������
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



