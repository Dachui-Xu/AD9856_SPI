#ifndef __AD9850_H__
#define __AD9850_H__

#include "stm32f1xx_hal.h"  

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long int u32;



#define AD9850_CONTROL_PORT  GPIOD
#define AD9850_FQUD   GPIO_PIN_4  //PD4->spi_en,cs
#define AD9850_WCLK   GPIO_PIN_5  //PD5->sclk
#define AD9850_DATA   GPIO_PIN_6  //PD6->sda
#define AD9850_RST    GPIO_PIN_7  //PD7->rest

#define AD9850_WCLK_H	HAL_GPIO_WritePin(AD9850_CONTROL_PORT, AD9850_WCLK, GPIO_PIN_SET)
#define AD9850_WCLK_L	HAL_GPIO_WritePin(AD9850_CONTROL_PORT, AD9850_WCLK, GPIO_PIN_RESET)
#define AD9850_FQUD_H	HAL_GPIO_WritePin(AD9850_CONTROL_PORT, AD9850_FQUD, GPIO_PIN_SET)
#define AD9850_FQUD_L	HAL_GPIO_WritePin(AD9850_CONTROL_PORT, AD9850_FQUD, GPIO_PIN_RESET)
#define AD9850_DATA_H	HAL_GPIO_WritePin(AD9850_CONTROL_PORT, AD9850_DATA, GPIO_PIN_SET)
#define AD9850_DATA_L	HAL_GPIO_WritePin(AD9850_CONTROL_PORT, AD9850_DATA, GPIO_PIN_RESET)
#define AD9850_RESET_H	HAL_GPIO_WritePin(AD9850_CONTROL_PORT, AD9850_RST, GPIO_PIN_SET)
#define AD9850_RESET_L	HAL_GPIO_WritePin(AD9850_CONTROL_PORT, AD9850_RST, GPIO_PIN_RESET)


void AD9850_Write(unsigned char w0,unsigned long int data);
void send_data2AD9850(uint32_t* data);

#endif


