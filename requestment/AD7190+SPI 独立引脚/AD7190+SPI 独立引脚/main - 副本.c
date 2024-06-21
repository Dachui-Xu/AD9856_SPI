#include<LQ12864.h>
#include<reg52.h>
#include<stdio.h>
#include <string.h>
#include<SPI_WR32bit.h>


/*
sbit zuo=P1^5;
sbit you=P1^1;
sbit shang=P1^6;
sbit xia=P1^0;
sbit yes=P1^4;
*/


sbit zuo=P1^4;
sbit you=P1^3;
sbit shang=P1^6;
sbit xia=P1^5;
sbit yes=P1^2;
sbit SET=P1^1;
sbit RST=P1^0;

void main()
{
  ulong rdadc,state;
  float dianya,VREF,voltage;	  //full scale
  ulong data0=0x0000055E,data1=0x04600847,data2=0x19DF48C4,data3=0x00000000,data4=0x21D00000,data5=0x00000152,data6=0x6800006C,data7=0x00000000,datatem;
  int num=1,flag=0,i=0,w,judge=0,flag2=0,reg=0,mimaflag=0,mima=123412,mimaansun=442231,mimahaoqi=444444,mimatemp=0,mimabit=0,j,flag_count;		 //password
  int flag_writein=0, count=0, ADC_mode=0;
  char str_reg[10],str_data[10],str_data_0x[10],str_count[10];
  OLED_Init();
  Draw_BMP(0,0,128,7,BMP1);
  delay(2000);
  delay(2000);
  delay(2000);
  delay(2000);
  delay(2000);
  OLED_Init();
  //OLED_P8x16Str(1, 2,"V:");
  //sprintf(str_reg,"%f",reg);
  //OLED_P8x16Str(40, 2,str_reg);
  ad7190_reset_serial();

  while(1)
  {
  	while(mimaflag==0)											   //password :you xia zuo shang you xia
	{
	    OLED_Init();
	    OLED_P6x8Str(0, 1,"Please enter password");
		mimatemp=0;
		i=0;
		OLED_P8x16Str(20, 5,"_ _ _ _ _ _");
		for(i=0;i<6;i++)
		{
		 while(shang==1&&xia==1&&zuo==1&&you==1);
		 if(you==0)
		{
		  delay(1000);
		  while(you==0);
		   mimabit=1;
		}
		if(xia==0)
		{
	      delay(1000);
		  while(xia==0);
		   mimabit=2;
		}
		if(zuo==0)
		{
		  delay(1000);
		  while(zuo==0);
		   mimabit=3;
		}
		if(shang==0)
		{
		  delay(1000);
		  while(shang==0);
		   mimabit=4;
		}
		mimatemp=mimatemp*10+mimabit;
		sprintf(str_reg,"%d",i);
		OLED_P8x16Str(16*i+20, 5,"*");
		}
		if(mimatemp==mima)
		{
		  mimaflag=1;
		   OLED_Fill(0x00);
		   OLED_P8x16Str(30, 3,"Welcome!");
		   delay(2000);
           delay(2000);
		   delay(2000);
           delay(2000);
		   delay(2000);
           delay(2000);
		   OLED_Fill(0x00);
		}
		if(mimatemp==mimaansun)
		{
		   mimaflag=1;
		   OLED_Fill(0x00);
		   OLED_P8x16Str(30, 2,"Welcome!");
		   OLED_P8x16Str(26, 4,"Dr.AnSun!");	 
		   delay(2000);
           delay(2000);
		   delay(2000);
           delay(2000);
		   delay(2000);
           delay(2000);
		   OLED_Fill(0x00);
		}
		if(mimatemp==mimahaoqi)
		{
		   mimaflag=1;
		   OLED_Fill(0x00);
		   OLED_P8x16Str(30, 2,"Welcome!");
		   OLED_P8x16Str(26, 4,"Mr.HaoQi!");
		   delay(2000);
           delay(2000);
		   delay(2000);
           delay(2000);
		   delay(2000);
           delay(2000);
		   OLED_Fill(0x00);
		}
		if(mimatemp!=mima&&mimatemp!=mimaansun&&mimatemp!=mimahaoqi)
		{
		  Draw_BMP(0,0,128,7,BMP2);
		  delay(2000);
          delay(2000);
          delay(2000);
          delay(2000);
		}
	 }
    


 
	while(mimaflag==1)
	{
		//OLED_Fill(0x00);
		//OLED_P6x8Str(15, 2,"Please select ADC mode OR SPI mode:");
		OLED_P8x16Str(0, 2,"Up:ADC mode");
		OLED_P8x16Str(0, 4,"Down:SPI mode");
		if(RST==0&&SET==0) 										//lock screen
		{
	      OLED_Init();
		  OLED_Fill(0x00);
		  mimaflag=0;
		  
		}
		if(shang==0) 
		{
	      while(shang==0);
		  ADC_mode=0;
		  OLED_Fill(0x00);
		  OLED_P6x8Str(40, 1, "ADC_mode");
		  judge=1;
		  
		  OLED_P6x8Str(0, 3, "UP:5V,Ex CLK,x1");
		  OLED_P6x8Str(0, 5, "RIGHT:5V,Ex CLK,x8");
		  OLED_P6x8Str(0, 7, "DOWN:2.5V,In CLK,x1");
		  
		  while(!ADC_mode)
		  {
				if(shang==0)
				{
					 while(shang==0);
					 ad7190_wr8_serial(0x10);
		 			 ad7190_wr24_serial(0x800158);			//1倍增益
		 			 /*ad7190_wr8_serial(0x08);
		 			 ad7190_wr24_serial(0x9827FF);
		 			 ad7190_wr8_serial(0x08);
		 			 ad7190_wr24_serial(0xB827FF);
		 			 ad7190_wr8_serial(0x08);		//internal Crystal Oscillator
		 			 ad7190_wr24_serial(0x1827FF);
		 			 ad7190_wr8_serial(0x5C);*/
					 
		 			 ad7190_wr8_serial(0x08);
		 			 ad7190_wr24_serial(0x9027FF);	  //full scale calibration 
		  			 ad7190_wr8_serial(0x08);
					 ad7190_wr24_serial(0xB027FF);	 //zero input calibration
		 			 ad7190_wr8_serial(0x08);		//external
		  			 ad7190_wr24_serial(0x1027FF);
		 			 //ad7190_wr8_serial(0x08);		//internal Crystal Oscillator
		  			 //ad7190_wr24_serial(0x1827FF);
					 VREF=5;
					 OLED_P6x8Str(0, 3, "UP:5V,Ex CLK,x1");
		             OLED_P6x8Str(0, 5, "                    ");
		  			 OLED_P6x8Str(0, 7, "                    ");
					 delay(8000);
					 OLED_P6x8Str(0, 3, "                    ");
					 ADC_mode=1;
				}
				
				if(you==0)
				{
					 while(you==0);
					 ad7190_wr8_serial(0x10);
		  			 ad7190_wr24_serial(0x80015B);			//8倍增益
		 			 /*ad7190_wr8_serial(0x08);
		 			 ad7190_wr24_serial(0x9827FF);
		 			 ad7190_wr8_serial(0x08);
		 			 ad7190_wr24_serial(0xB827FF);
		 			 ad7190_wr8_serial(0x08);		//internal Crystal Oscillator
		 			 ad7190_wr24_serial(0x1827FF);
		 			 ad7190_wr8_serial(0x5C);*/
					 
		 			 ad7190_wr8_serial(0x08);
		 			 ad7190_wr24_serial(0x9027FF);	  //full scale calibration 
		  			 ad7190_wr8_serial(0x08);
					 ad7190_wr24_serial(0xB027FF);	 //zero input calibration
		 			 ad7190_wr8_serial(0x08);		//external
		  			 ad7190_wr24_serial(0x1027FF);
		 			 //ad7190_wr8_serial(0x08);		//internal Crystal Oscillator
		  			 //ad7190_wr24_serial(0x1827FF);
					 VREF=5;
					 OLED_P6x8Str(0, 3, "                    ");
		             OLED_P6x8Str(0, 5, "RIGHT:5V,Ex CLK,x8");
		  			 OLED_P6x8Str(0, 7, "                    ");
					 delay(8000);
					 OLED_P6x8Str(0, 5, "                    ");
					 ADC_mode=1;
				}
				
				if(xia==0)
				{
					 while(xia==0);
					 ad7190_wr8_serial(0x10);
		 			 ad7190_wr24_serial(0x800158);			//1倍增益
		 			 /*ad7190_wr8_serial(0x08);
		 			 ad7190_wr24_serial(0x9827FF);
		 			 ad7190_wr8_serial(0x08);
		 			 ad7190_wr24_serial(0xB827FF);
		 			 ad7190_wr8_serial(0x08);		//internal Crystal Oscillator
		 			 ad7190_wr24_serial(0x1827FF);
		 			 ad7190_wr8_serial(0x5C);*/
					 
		 			 ad7190_wr8_serial(0x08);
		 			 ad7190_wr24_serial(0x9027FF);	  //full scale calibration 
		  			 ad7190_wr8_serial(0x08);
					 ad7190_wr24_serial(0xB027FF);	 //zero input calibration
		 			 //ad7190_wr8_serial(0x08);		//external
		  			 //ad7190_wr24_serial(0x1027FF);
		 			 ad7190_wr8_serial(0x08);		//internal Crystal Oscillator
		  			 ad7190_wr24_serial(0x1827FF);
					 VREF=2.5;
					 OLED_P6x8Str(0, 3, "                    ");
		             OLED_P6x8Str(0, 5, "                    ");
		  			 OLED_P6x8Str(0, 7, "DOWN:2.5V,In CLK,x1");
					 delay(8000);
					 OLED_P6x8Str(0, 7, "                    ");
					 ADC_mode=1;
				}	
		  } 

		  /*ad7190_wr8_serial(0x10);
		  //ad7190_wr24_serial(0x80015B);			//8倍增益
		  ad7190_wr24_serial(0x800158);			//1倍增益
		  /*ad7190_wr8_serial(0x08);
		  ad7190_wr24_serial(0x9827FF);
		  ad7190_wr8_serial(0x08);
		  ad7190_wr24_serial(0xB827FF);
		  ad7190_wr8_serial(0x08);		//internal Crystal Oscillator
		  ad7190_wr24_serial(0x1827FF);
		  ad7190_wr8_serial(0x5C);*/

		  /*ad7190_wr8_serial(0x08);
		  ad7190_wr24_serial(0x9027FF);	  //full scale calibration 
		  ad7190_wr8_serial(0x08);
		  ad7190_wr24_serial(0xB027FF);	 //zero input calibration
		  //ad7190_wr8_serial(0x08);		//external
		  //ad7190_wr24_serial(0x1027FF);
		  ad7190_wr8_serial(0x08);		//internal Crystal Oscillator
		  ad7190_wr24_serial(0x1827FF);
		  //OLED_P6x8Str(40, 1, "ADC_mode");	
		  */

		  ad7190_wr8_serial(0x5C);  
		}

		
		while(judge==1)
		{			   //enter the iteration of ADC								
			for(flag2=0, voltage=0, i=0;i<3;i++)
			{
				while(ad7190_bit_out==1)
				{
					if(RST==0) break;
				}
				rdadc=ad7190_rd24_serial();						//read the ADC 10 times
				state=rdadc&0x000000FF;
				rdadc=rdadc>>8&0x00FFFFFF;
				//dianya=rdadc*VREF/16777216/8;					//8倍增益
				dianya=rdadc*VREF/16777216;
				voltage=voltage+dianya;							//1倍增益
			}

			  if(RST==1)
			   {
				sprintf(str_reg,"%f",voltage/3);
				OLED_P8x16Str(30, 4,"         (V)");
				OLED_P8x16Str(30, 4,str_reg);
				count=count+1;
			   }
			
			if(count==10)
			{
				ad9850_fq_up=0;
				ad9850_rest=0;
				delay(200);
				ad9850_fq_up=1;
				ad9850_rest=1;
			
		 	    ad9850_wr_serial(0,data0);
				ad9850_wr_serial(1,data1);	
				ad9850_wr_serial(2,data2);
				ad9850_wr_serial(3,data3);
				ad9850_wr_serial(4,data4);
				ad9850_wr_serial(5,data5);
				ad9850_wr_serial(6,data6);
				ad9850_wr_serial(7,data7);

			

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
					}
				OLED_P8x16Str(25, 6, "write in");
				delay(1000);
				OLED_P8x16Str(25, 6, "          ");
				count=0;	
			}
						

			if(RST==0)
				{ 
				    while(RST==0);
					judge=0;
					OLED_Fill(0x00);
					count=0;	
				}

			 
			/*while(flag2==0)
			{
				if(SET==0) flag2=1;
				if(RST==0)
				{
					flag2=1;
					judge=0;
				}
			} */
			
		}
		
		if(xia==0) 
		{
		  while(xia==0);
		  OLED_Fill(0x00);
		  judge=2;
		  sprintf(str_reg,"%d",reg);
  		  OLED_P6x8Str(10, 2,"reg:");
  		  OLED_P6x8Str(50, 2,"[31:0]:");
  		  OLED_P6x8Str(15, 4,str_reg);
  		  OLED_P6x8Str(50, 4,"0X");
  		  OLED_P6x8Str(15, 5,"*");
		  OLED_P6x8Str(60,0,"  ?");
		  OLED_P6x8Str(40, 1, "SPI_mode");
		}

		while(judge==2)

		{						    //enter the iteration of SPI	
			switch (reg)
  				  {
  					case 0:
						datatem=data0;
						break;
  					case 1:
						datatem=data1;
						break;
 				  	case 2:	   	  
						datatem=data2;
						break;
					case 3:
						datatem=data3;
						break;
 				  	case 4:	   	  
						datatem=data4;
						break;
					case 5:
						datatem=data5;
						break;
 				  	case 6:	   	  
						datatem=data6;
						break;
					case 7:
						datatem=data7;
						break;
  			     }
                 

 	   if(zuo==0||you==0||shang==0||xia==0)
   		{
			

  			if(you==0)
  			{
   				while(you==0);
  					num++;
				if(num<1)
  	 				num=9;
  				if(num>9)
  	 				num=1;
	 			switch (num)
  				{
   					case 1:
						OLED_P6x8Str(15, 5,"*");
						OLED_P6x8Str(68, 5," ");
						OLED_P6x8Str(74, 5," ");
						OLED_P6x8Str(80, 5," ");
						OLED_P6x8Str(86, 5," ");
						OLED_P6x8Str(95, 5," ");
						OLED_P6x8Str(101, 5," ");
						OLED_P6x8Str(107, 5," ");
						OLED_P6x8Str(113, 5," ");

						break;
   					case 2:
					    OLED_P6x8Str(15, 5," ");
						OLED_P6x8Str(68, 5,"*");
						OLED_P6x8Str(74, 5," ");
						OLED_P6x8Str(80, 5," ");
						OLED_P6x8Str(86, 5," ");
						OLED_P6x8Str(95, 5," ");
						OLED_P6x8Str(101, 5," ");
						OLED_P6x8Str(107, 5," ");
						OLED_P6x8Str(113, 5," ");
						break;
   					case 3:
					    OLED_P6x8Str(15, 5," ");
						OLED_P6x8Str(68, 5," ");
						OLED_P6x8Str(74, 5,"*");
						OLED_P6x8Str(80, 5," ");
						OLED_P6x8Str(86, 5," ");
						OLED_P6x8Str(95, 5," ");
						OLED_P6x8Str(101, 5," ");
						OLED_P6x8Str(107, 5," ");
						OLED_P6x8Str(113, 5," ");
						break;
					case 4:
						OLED_P6x8Str(15, 5," ");
						OLED_P6x8Str(68, 5," ");
						OLED_P6x8Str(74, 5," ");
						OLED_P6x8Str(80, 5,"*");
						OLED_P6x8Str(86, 5," ");
						OLED_P6x8Str(95, 5," ");
						OLED_P6x8Str(101, 5," ");
						OLED_P6x8Str(107, 5," ");
						OLED_P6x8Str(113, 5," ");
						break;
   					case 5:
					    OLED_P6x8Str(15, 5," ");
						OLED_P6x8Str(68, 5," ");
						OLED_P6x8Str(74, 5," ");
						OLED_P6x8Str(80, 5," ");
						OLED_P6x8Str(86, 5,"*");
						OLED_P6x8Str(95, 5," ");
						OLED_P6x8Str(101, 5," ");
						OLED_P6x8Str(107, 5," ");
						OLED_P6x8Str(113, 5," ");
						break;
   					case 6:
					    OLED_P6x8Str(15, 5," ");
						OLED_P6x8Str(68, 5," ");
						OLED_P6x8Str(74, 5," ");
						OLED_P6x8Str(80, 5," ");
						OLED_P6x8Str(86, 5," ");
						OLED_P6x8Str(95, 5,"*");
						OLED_P6x8Str(101, 5," ");
						OLED_P6x8Str(107, 5," ");
						OLED_P6x8Str(113, 5," ");
						break;
					case 7:
						OLED_P6x8Str(15, 5," ");
						OLED_P6x8Str(68, 5," ");
						OLED_P6x8Str(74, 5," ");
						OLED_P6x8Str(80, 5," ");
						OLED_P6x8Str(86, 5," ");
						OLED_P6x8Str(95, 5," ");
						OLED_P6x8Str(101, 5,"*");
						OLED_P6x8Str(107, 5," ");
						OLED_P6x8Str(113, 5," ");
						break;
   					case 8:
					    OLED_P6x8Str(15, 5," ");
						OLED_P6x8Str(68, 5," ");
						OLED_P6x8Str(74, 5," ");
						OLED_P6x8Str(80, 5," ");
						OLED_P6x8Str(86, 5," ");
						OLED_P6x8Str(95, 5," ");
						OLED_P6x8Str(101, 5," ");
						OLED_P6x8Str(107, 5,"*");
						OLED_P6x8Str(113, 5," ");
						break;
   					case 9:
					    OLED_P6x8Str(15, 5," ");
						OLED_P6x8Str(68, 5," ");
						OLED_P6x8Str(74, 5," ");
						OLED_P6x8Str(80, 5," ");
						OLED_P6x8Str(86, 5," ");
						OLED_P6x8Str(95, 5," ");
						OLED_P6x8Str(101, 5," ");
						OLED_P6x8Str(107, 5," ");
						OLED_P6x8Str(113, 5,"*");
						break;
  				}
  			}


			if(zuo==0)
  			{
   				while(zuo==0);
  					num--;
				if(num<1)
  	 				num=9;
  				if(num>9)
  	 				num=1;
	 			switch (num)
  				{
   					case 1:
						OLED_P6x8Str(15, 5,"*");
						OLED_P6x8Str(68, 5," ");
						OLED_P6x8Str(74, 5," ");
						OLED_P6x8Str(80, 5," ");
						OLED_P6x8Str(86, 5," ");
						OLED_P6x8Str(95, 5," ");
						OLED_P6x8Str(101, 5," ");
						OLED_P6x8Str(107, 5," ");
						OLED_P6x8Str(113, 5," ");

						break;
   					case 2:
					    OLED_P6x8Str(15, 5," ");
						OLED_P6x8Str(68, 5,"*");
						OLED_P6x8Str(74, 5," ");
						OLED_P6x8Str(80, 5," ");
						OLED_P6x8Str(86, 5," ");
						OLED_P6x8Str(95, 5," ");
						OLED_P6x8Str(101, 5," ");
						OLED_P6x8Str(107, 5," ");
						OLED_P6x8Str(113, 5," ");
						break;
   					case 3:
					    OLED_P6x8Str(15, 5," ");
						OLED_P6x8Str(68, 5," ");
						OLED_P6x8Str(74, 5,"*");
						OLED_P6x8Str(80, 5," ");
						OLED_P6x8Str(86, 5," ");
						OLED_P6x8Str(95, 5," ");
						OLED_P6x8Str(101, 5," ");
						OLED_P6x8Str(107, 5," ");
						OLED_P6x8Str(113, 5," ");
						break;
					case 4:
						OLED_P6x8Str(15, 5," ");
						OLED_P6x8Str(68, 5," ");
						OLED_P6x8Str(74, 5," ");
						OLED_P6x8Str(80, 5,"*");
						OLED_P6x8Str(86, 5," ");
						OLED_P6x8Str(95, 5," ");
						OLED_P6x8Str(101, 5," ");
						OLED_P6x8Str(107, 5," ");
						OLED_P6x8Str(113, 5," ");
						break;
   					case 5:
					    OLED_P6x8Str(15, 5," ");
						OLED_P6x8Str(68, 5," ");
						OLED_P6x8Str(74, 5," ");
						OLED_P6x8Str(80, 5," ");
						OLED_P6x8Str(86, 5,"*");
						OLED_P6x8Str(95, 5," ");
						OLED_P6x8Str(101, 5," ");
						OLED_P6x8Str(107, 5," ");
						OLED_P6x8Str(113, 5," ");
						break;
   					case 6:
					    OLED_P6x8Str(15, 5," ");
						OLED_P6x8Str(68, 5," ");
						OLED_P6x8Str(74, 5," ");
						OLED_P6x8Str(80, 5," ");
						OLED_P6x8Str(86, 5," ");
						OLED_P6x8Str(95, 5,"*");
						OLED_P6x8Str(101, 5," ");
						OLED_P6x8Str(107, 5," ");
						OLED_P6x8Str(113, 5," ");
						break;
					case 7:
						OLED_P6x8Str(15, 5," ");
						OLED_P6x8Str(68, 5," ");
						OLED_P6x8Str(74, 5," ");
						OLED_P6x8Str(80, 5," ");
						OLED_P6x8Str(86, 5," ");
						OLED_P6x8Str(95, 5," ");
						OLED_P6x8Str(101, 5,"*");
						OLED_P6x8Str(107, 5," ");
						OLED_P6x8Str(113, 5," ");
						break;
   					case 8:
					    OLED_P6x8Str(15, 5," ");
						OLED_P6x8Str(68, 5," ");
						OLED_P6x8Str(74, 5," ");
						OLED_P6x8Str(80, 5," ");
						OLED_P6x8Str(86, 5," ");
						OLED_P6x8Str(95, 5," ");
						OLED_P6x8Str(101, 5," ");
						OLED_P6x8Str(107, 5,"*");
						OLED_P6x8Str(113, 5," ");
						break;
   					case 9:
					    OLED_P6x8Str(15, 5," ");
						OLED_P6x8Str(68, 5," ");
						OLED_P6x8Str(74, 5," ");
						OLED_P6x8Str(80, 5," ");
						OLED_P6x8Str(86, 5," ");
						OLED_P6x8Str(95, 5," ");
						OLED_P6x8Str(101, 5," ");
						OLED_P6x8Str(107, 5," ");
						OLED_P6x8Str(113, 5,"*");
						break;
  				}
  			}

			  

			if(shang==0)
  			{
   				while(shang==0);
	 			switch (num)
  				{
  					case 1:
						reg=reg+1;
						if(reg<0)
  	 				    reg=7;
  				        if(reg>7)
  	 			    	reg=0;
						switch (reg)
  				  {
  					case 0:
						datatem=data0;
						break;
  					case 1:
						datatem=data1;
						break;
 				  	case 2:	   	  
						datatem=data2;
						break;
					case 3:
						datatem=data3;
						break;
 				  	case 4:	   	  
						datatem=data4;
						break;
					case 5:
						datatem=data5;
						break;
 				  	case 6:	   	  
						datatem=data6;
						break;
					case 7:
						datatem=data7;
						break;
  			     }
						sprintf(str_reg,"%d",reg);
						OLED_P6x8Str(15, 4,str_reg);
						break;
  					case 2:
						datatem=datatem+268435456;
						break;
 				  	case 3:	   	  
						datatem=datatem+16777216;
						break;
					case 4:
						datatem=datatem+1048576;
						break;
 				  	case 5:	   	  
						datatem=datatem+65536;
						break;
					case 6:
						datatem=datatem+4096;
						break;
 				  	case 7:	   	  
						datatem=datatem+256;
						break;
					case 8:
						datatem=datatem+16;
						break;
 				  	case 9:	   	  
						datatem=datatem+1;
						break;
  				}
  			}


			  if(xia==0)
  			{
   				while(xia==0);
	 			switch (num)
  				{
  					case 1:
						reg=reg-1;
						if(reg<0)
  	 				    reg=7;
  				        if(reg>7)
  	 			    	reg=0;
						switch (reg)
  				  {
  					case 0:
						datatem=data0;
						break;
  					case 1:
						datatem=data1;
						break;
 				  	case 2:	   	  
						datatem=data2;
						break;
					case 3:
						datatem=data3;
						break;
 				  	case 4:	   	  
						datatem=data4;
						break;
					case 5:
						datatem=data5;
						break;
 				  	case 6:	   	  
						datatem=data6;
						break;
					case 7:
						datatem=data7;
						break;
  			     }
						sprintf(str_reg,"%d",reg);
						OLED_P6x8Str(15, 4,str_reg);
						break;
  					case 2:
						datatem=datatem-268435456;
						break;
 				  	case 3:	 	  
						datatem=datatem-16777216;
						break;
					case 4:
						datatem=datatem-1048576;
						break;
 				  	case 5:	  	  
						datatem=datatem-65536;
						break;
					case 6:
						datatem=datatem-4096;
						break;
 				  	case 7:	 	  
						datatem=datatem-256;
						break;
					case 8:
						datatem=datatem-16;
						break;
 				  	case 9:	 	  
						datatem=datatem-1;
						break;
  				}
  			}


						switch (reg)
  				  {
  					case 0:
						data0=datatem;
						break;
  					case 1:
						data1=datatem;
						break;
 				  	case 2:	   	  
						data2=datatem;
						break;
					case 3:
						data3=datatem;
						break;
 				  	case 4:	   	  
						data4=datatem;
						break;
					case 5:
						data5=datatem;
						break;
 				  	case 6:	   	  
						data6=datatem;
						break;
					case 7:
						data7=datatem;
						break;
  			     }

			if(flag==0)
   			{	
				OLED_P6x8Str(60,0,"  ?");
				flag=1;
   			}
					 
			
 	      }	
		  	

			 if(datatem<0)
  	 		 datatem=4294967295;
  		     if(datatem>4294967295)
  	 		 datatem=0;

			 
			 sprintf(str_data_0x,"%04lx",(datatem<<16)>>16);
			 OLED_P6x8Str(95, 4,str_data_0x);
			 sprintf(str_data_0x,"%04lx",datatem>>16);
			 OLED_P6x8Str(68, 4,str_data_0x);

			 sprintf(str_data,"%10lu",datatem);
			 OLED_P6x8Str(0, 7,"decimal");
	         OLED_P6x8Str(60, 7,str_data);

			 if(yes==0)
	 	   {
	  		while(yes==0);	
		     OLED_P6x8Str(60, 0,"OK!");		 
	   		 flag=0; 

			ad9850_fq_up=0;
			ad9850_rest=0;
			delay(200);
			ad9850_fq_up=1;
			ad9850_rest=1;
			
		    ad9850_wr_serial(0,data0);
			ad9850_wr_serial(1,data1);	
			ad9850_wr_serial(2,data2);
			ad9850_wr_serial(3,data3);
			ad9850_wr_serial(4,data4);
			ad9850_wr_serial(5,data5);
			ad9850_wr_serial(6,data6);
			ad9850_wr_serial(7,data7);

			

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
		
		   if(SET==0)
				{
				while(SET==0);
				flag_writein=1;
				OLED_P6x8Str(13, 6, "continuous write");
				OLED_P6x8Str(60,0,"    ");		
				}

			while(flag_writein==1)
				{				
				ad9850_fq_up=0;
				ad9850_rest=0;
				delay(200);
				ad9850_fq_up=1;
				ad9850_rest=1;
			
		 	    ad9850_wr_serial(0,data0);
				ad9850_wr_serial(1,data1);	
				ad9850_wr_serial(2,data2);
				ad9850_wr_serial(3,data3);
				ad9850_wr_serial(4,data4);
				ad9850_wr_serial(5,data5);
				ad9850_wr_serial(6,data6);
				ad9850_wr_serial(7,data7);

			

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
				
				OLED_P6x8Str(60,0,"WR");
				flag_count=1;

				for(i=0;i<10&&flag_count;i++)
				{
					for(j=0;j<5&&flag_count;j++)
					{
						delay(4000);
						if(yes==0)
						{
							flag_writein=0;
							flag_count=0;
							
						}
					}
					sprintf(str_count,"%d",i);
					OLED_P6x8Str(60,0,"   ");
					OLED_P6x8Str(60,0,str_count);
				}	

				OLED_P6x8Str(60,0,"   ");
				if(yes==0)
					{
						while(yes==0)
						OLED_P6x8Str(10, 6, "                 ");
						OLED_P6x8Str(60,0,"  ?");
					}
				
				}



			if(RST==0)
				{
				    while(RST==0);
					judge=0;
					OLED_Fill(0x00);
				}

		}
	}
}
}



