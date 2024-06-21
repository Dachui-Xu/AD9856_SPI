/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "ad9850.h"
#include <stdint.h>
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RXBUFFERSIZE  256     //最大接收字节数
#define COMMAND_START 0x706C6179  //开始将启动SPI控制命令
#define COMMAND_DATA 0x64617461		//展示数据命令
/* USER CODE END PD */
/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t RxBuffer[RXBUFFERSIZE];   //接收数据
uint8_t aRxBuffer;			//接收中断缓冲
uint8_t Uart1_Rx_Cnt = 0;		//接收缓冲计数
uint8_t TX_str1[] = "Welcome!\r\n";
uint32_t data[8] = {
										0xFFFFFFFF,
										0x04600847,
										0x19DF48C4,
										0x00000000,
										0x21D00000,
										0x00000152,
										0x6800006C,
										0x00000000
										};										
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
										
/*
	@function:把帧数据存储到data中										
*/
void process_frame(uint8_t *frame, uint8_t len, uint32_t *data) {
    uint8_t reg = frame[0];
    uint32_t datatem = (frame[1] << 24) | (frame[2] << 16) | (frame[3] << 8) | frame[4];
		data[reg] = datatem;
		printf("Received data (hex) in reg_bank[%d]: 0x%08X\n", reg ,data[reg]);
}

/*
	@fuction:打印data中的数据
*/
void printf_data(){
		for(int i= 0;i<8;i++){
		printf("reg:%d, 0x%08X\n",i,data[i]);
		
		}
		printf("---------------\n");
}
/*
	@function:处理命令
*/
void process_command(uint8_t *frame,uint32_t* data) {
     uint32_t datatem = (frame[1] << 24) | (frame[2] << 16) | (frame[3] << 8) | frame[4];
    switch (datatem) {
        case COMMAND_START:
            send_data2AD9850(data);
            break;
        case COMMAND_DATA:
            printf_data();
            break;
        default:
            printf("Error control signal: 0x%08X\n", datatem);
            break;
    }
}

										
 // 串口接收中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
	if(Uart1_Rx_Cnt >= 255)  //溢出判断
	{
		Uart1_Rx_Cnt = 0;
		memset(RxBuffer,0x00,sizeof(RxBuffer));
		HAL_UART_Transmit(&huart1, (uint8_t *)"overflow\n", 10,0xFFFF); 	
        
	}
	else
	{
		RxBuffer[Uart1_Rx_Cnt++] = aRxBuffer;   //接收数据转存
	
		if((RxBuffer[Uart1_Rx_Cnt-1] == 0x0A)&&(RxBuffer[Uart1_Rx_Cnt-2] == 0x0D)) //判断结束位
		{	
			if(RxBuffer[0] == 0x0D){
				 process_command(RxBuffer,data);
			}
			else if(RxBuffer[0]>=0x00 && RxBuffer[0]<=0x07 && Uart1_Rx_Cnt==7){
				process_frame(RxBuffer,Uart1_Rx_Cnt-2,data);
			}else{
			printf("Please input correct parameter!\n");
			}
				Uart1_Rx_Cnt = 0;
				memset(RxBuffer,0x00,sizeof(RxBuffer)); //清空数组
			}
	}
	
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Transmit(&huart1,TX_str1,sizeof(TX_str1),10000);
	printf("\n");
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);
	
/* USER CODE END 2 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		send_data2AD9850(data);

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
 

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
