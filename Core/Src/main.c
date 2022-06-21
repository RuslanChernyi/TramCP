/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "can.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "drv_canfdspi_defines.h"
#include "drv_canfdspi_register.h"
#include "IO_Board.h"
#include "modbus_ascii.h"
#include "canfd_stm.h"
#include "canfd_stm_config.h"
#include "DMA.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define STID (0x7FF<<15)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t RxData_fifo[8];
uint8_t Unfiltered_CAN_msgs[8];
uint8_t first_packet[8];
uint8_t second_packet[8];
uint8_t third_packet[8];
uint8_t fourth_packet[8];

modbus_ascii_request_msg_t modbus_request_msg;
modbus_ascii_response_msg_t modbus_response_msg;
reqest_info_t request_info;


new_MODBUSTable_uni_t New_MODBUS_Table;
uint8_t XA2_dir;

uint32_t next_case = 1;

IOboard_request_t IOboard_req;

uint32_t error_notice;
uint32_t position_in_program;

uint32_t IOADCs[] = {CH2, CH3, CH4, CH5, CH7, CH14, CH15};

IOBoard_t io1;
IOBoard_t io2;
IOBoard_t io3;
IOBoard_t io4;

uint8_t usart1_receive_buffer[17];
uint32_t modbus_message_received;

uint32_t allow_placement;

spiCAN spican1;
spiCAN spican2;
spiCAN spican3;
spiCAN spican4;

UsedFIFOs canfd3_fifos = {0};
UsedFIFOs canfd4_fifos = {0};


mcp_status canfd1_status = {0};
mcp_status canfd2_status = {0};
mcp_status canfd3_status = {0};
mcp_status canfd4_status = {0};

CAN_RX_MSGOBJ received_msg = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  USART1->CR1 |= (1U<<5);	// Enable Receive interrupt
  SPI1->CR1 |= (1U<<6);	// Enable SPI1
  SPI2->CR1 |= (1U<<6);	// Enable SPI2
  IOboard1Init();
  IOboard2Init();
  IOboard3Init();

  //  spiCAN1_Init();
  //  spiCAN2_Init();
  spiCAN3_Init();
  spiCAN4_Init();

  //  DMA_SPI1RXInit();
  DMA_SPI2RXInit();

  //  canfd_configure(&spican1);
  //  canfd_configure(&spican2);
  canfd_configure(&canfd3_fifos, &spican3);
  canfd_configure(&canfd4_fifos, &spican4);

  //  canfd_getStatus(&canfd1_status, &spican1);
  //  canfd_getStatus(&canfd2_status, &spican2);
  canfd_getStatus(&canfd3_status, &spican3);
  canfd_getStatus(&canfd4_status, &spican4);

  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
	  position_in_program = IN_MAIN;
	  if(modbus_message_received)
	  {
		__HAL_UART_DISABLE_IT(&huart1, UART_IT_RXNE);
		uint32_t modbus_error = 0;
		reqest_info_t modbus_request = {0};
		modbus_error = check_modbus_request(usart1_receive_buffer, &modbus_request);
		if(modbus_error == 0)
		{
			send_modbus_response(&huart1, &modbus_request);
		}
		modbus_message_received = 0;

		__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	  }
	  else
	  {
		  static uint8_t my_message[8] = {0};

		  for(uint32_t i = 0; i < sizeof(my_message); i++)
		  {
			  my_message[i] = i;
		  }
		  canfd_getStatus(&canfd3_status, &spican3);
		  canfd_transmit(my_message, CAN_FIFO_CH1, &spican3);
		  canfd_getStatus(&canfd4_status, &spican4);
//		  canfd_transmit(my_message, CAN_FIFO_CH2, &spican4);
		  received_msg = canfd_receive(CAN_FIFO_CH2, &spican3);
		  __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	  }

    /* USER CODE END WHILE */

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
//{
//	uint8_t id = (hcan->Instance->sFIFOMailBox[0].RIR & STID)>>21;
//	switch(id)
//	{
//		case 20:
//			HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO0, &RxHeader, RxData_fifo);
//			for(int i = 0; i < 8; i++)
//			{
//				first_packet[i] = RxData_fifo[i];
//			}
//			break;
//		case 21:
//			HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO0, &RxHeader, RxData_fifo);
//			for(int i = 0; i < 8; i++)
//			{
//				second_packet[i] = RxData_fifo[i];
//			}
//			break;
//		case 22:
//			HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO0, &RxHeader, RxData_fifo);
//			for(int i = 0; i < 8; i++)
//			{
//				third_packet[i] = RxData_fifo[i];
//			}
//			break;
//		case 23:
//			HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO0, &RxHeader, RxData_fifo);
//			for(int i = 0; i < 8; i++)
//			{
//				fourth_packet[i] = RxData_fifo[i];
//			}
//			break;
//		default:
//			break;
//	}
//	allow_placement = 1;
//}
void HAL_USART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	return;
}


void SPI_Transmit (uint8_t *data, int size, SPI_TypeDef * SPIx)
{

	/************** STEPS TO FOLLOW *****************
	1. Wait for the TXE bit to set in the Status Register
	2. Write the data to the Data Register
	3. After the data has been transmitted, wait for the BSY bit to reset in Status Register
	4. Clear the Overrun flag by reading DR and SR
	************************************************/

	int i=0;
	while (i<size)
	{
	   while (!((SPIx->SR)&(1<<1))) {};  // wait for TXE bit to set -> This will indicate that the buffer is empty
	   SPIx->DR = data[i];  // load the data into the Data Register
	   i++;
	}


/*During discontinuous communications, there is a 2 APB clock period delay between the
write operation to the SPI_DR register and BSY bit setting. As a consequence it is
mandatory to wait first until TXE is set and then until BSY is cleared after writing the last
data.
*/
	while (!((SPIx->SR)&(1<<1))) {};  // wait for TXE bit to set -> This will indicate that the buffer is empty
	while (((SPIx->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication

	//  Clear the Overrun flag by reading DR and SR
	uint8_t temp = SPIx->DR;
	temp = SPIx->SR;

}

void SPI_Receive (uint8_t *data, int size, SPI_TypeDef * SPIx)
{
	/************** STEPS TO FOLLOW *****************
	1. Wait for the BSY bit to reset in Status Register
	2. Send some Dummy data before reading the DATA
	3. Wait for the RXNE bit to Set in the status Register
	4. Read data from Data Register
	************************************************/

	while (size)
	{
		while (((SPIx->SR) & (1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
		SPIx->DR = 0;  // send dummy data
		while (!((SPIx->SR) & (1<<0))){};  // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
	  *data++ = (SPIx->DR);
		size--;
	}
	//  Clear the Overrun flag by reading DR and SR
	uint32_t temp = SPIx->SR;
}
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
