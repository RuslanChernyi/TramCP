/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "modbus_ascii.h"
#include "drv_canfdspi_api.h"
#include "drv_canfdspi_defines.h"
#include "drv_canfdspi_register.h"
#include "drv_spi.h"
#include "IO_Board.h"
#include "Drive.h"
#include "systick.h"
#include "spi.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define IOBOARD_N0	0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern CAN_HandleTypeDef hcan1;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */
extern CAN_TxHeaderTypeDef TxHeader;
extern CAN_RxHeaderTypeDef RxHeader;
extern uint8_t RxData_fifo[8];
extern CAN_CONFIG can_config;
extern uint32_t MCU_CONTROL_COUNTER;
extern uint32_t next_case;
extern IOboard_request_t IOboard_req;
extern uint32_t position_in_program;
extern uint32_t cmd_for_IOboard;
extern uint32_t IOboard_adc_in_process;
extern IOBoard_t IOBoard1;
extern IOBoard_t IOBoard2;
extern IOBoard_t IOBoard3;
extern uint8_t usart1_receive_buffer[17];
extern uint32_t usart1_error;
extern uint32_t modbus_message_received;
extern uint32_t packet_counter;
extern uint8_t first_packet[8];
extern uint8_t second_packet[8];
extern uint8_t third_packet[8];
extern uint8_t fourth_packet[8];
extern IOBoard_t io1;
extern IOBoard_t io2;
extern IOBoard_t io3;
extern IOBoard_t io4;
extern uint8_t spi1_rx_buf[20];
extern uint8_t stop_spi_tx;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles CAN1 RX0 interrupts.
  */
void CAN1_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN CAN1_RX0_IRQn 0 */

  /* USER CODE END CAN1_RX0_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan1);
  /* USER CODE BEGIN CAN1_RX0_IRQn 1 */

  /* USER CODE END CAN1_RX0_IRQn 1 */
}

/**
  * @brief This function handles CAN1 RX1 interrupt.
  */
void CAN1_RX1_IRQHandler(void)
{
  /* USER CODE BEGIN CAN1_RX1_IRQn 0 */

  /* USER CODE END CAN1_RX1_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan1);
  /* USER CODE BEGIN CAN1_RX1_IRQn 1 */

  /* USER CODE END CAN1_RX1_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
	static uint32_t l = 0;
	if(USART1->DR == ':')
	{
		usart1_receive_buffer[0] = ':';
		l = 1;
	}
	else
	{
		usart1_receive_buffer[l] = USART1->DR;
		l++;
	}
	if(l > 16)
	{
		l = 1;
		modbus_message_received = 1;
	}
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt, DAC1 and DAC2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */
	typedef enum
	{
		NO,
		YES
	}go_next_enum;
	position_in_program = IN_TIM7_INTERRUPT;
	static uint32_t current_block = 1;
	uint32_t go_to_the_next_block = 0;
	/*** Meandr for CPU control ***/
	if(MCU_CONTROL_COUNTER == 10)
	{
		HAL_GPIO_TogglePin(MCU_CONTROL_MEANDR_GPIO_Port, MCU_CONTROL_MEANDR_Pin);
		MCU_CONTROL_COUNTER = 0;
		HAL_GPIO_TogglePin(XZ1_HL4_GPIO_Port, XZ1_HL4_Pin);
	}
	else
	{
		MCU_CONTROL_COUNTER++;
	}

	/*** Request data from drive board ***/
//	switch(next_case)
//	{
//		case 1:
//			askPacket(&hcan1);
//			next_case = 2;
//			break;
//		case 2:
//			if(HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
//			{
//				Error_Handler();
//			}
//			next_case = 3;
//			break;
//		case 3:
//			placeIntoTable();
//			next_case = 4;
//			break;
//		case 4:
//			IOboard_request(&hcan2, 0, cmd_for_IOboard, &IOboard_req);
//			next_case = 5;
//			break;
//		case 5:
//			receiveFromIO(&hcan2, RxData_fifo, 119);
//			next_case = 6;
//			break;
//		case 6:
//			placeIntoTableIO(RxData_fifo, &IOboard_req);
//			if((cmd_for_IOboard == 3) && ((IOboard_adc_in_process > 0) && (IOboard_adc_in_process < 7)))
//			{
//				cmd_for_IOboard = 3;
//				next_case = 4;
//			}
//			else
//			{
//				cmd_for_IOboard++;
//				next_case = 7;
//			}
//			break;
//		case 7:
//			CIO(&io1);
//			next_case = 8;
//		default:
//			next_case = 1;
//			break;
//	}
	switch(current_block)
	{
		case 1:
			go_to_the_next_block = CIO(&io1);
			if(go_to_the_next_block == YES)
			{
				current_block = 2;
			}
			break;
		case 2:
			go_to_the_next_block = CDR();
			if(go_to_the_next_block == YES)
			{
				current_block = 1;
			}
			break;
	}
  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}

/* USER CODE BEGIN 1 */

//	if((usart1_receive_buffer[0]) == ':')	//(usart1_receive_buffer[0])	//USART1->DR
//	{
//	  //usart1_receive_buffer[0] = ':';
//	  HAL_UART_Receive_IT(&huart1, &usart1_receive_buffer[1], 16);
//	  modbus_message_received = 1;
//
//	}
//	static uint32_t usart1_counter = 0;
//	static uint32_t k = 1;
//	static uint32_t start_reading = 0;
//	if((usart1_receive_buffer[0] = USART1->DR) == ':')
//	{
//		HAL_UART_Receive_IT(&huart1, &usart1_receive_buffer[1], 16, 1);
//		modbus_message_received = 1;
//	}
//	else
//	{
//		modbus_message_received = 0;
//	}

//	if((USART1->SR & (1U<<5)))
//	{
//		if(USART1->DR == ':')
//		{
//			usart1_receive_buffer[0] = ':';
//			k = 1;
//		}
//		else if(USART1->DR == CR_CODE)
//		{
//			usart1_receive_buffer[15] = CR_CODE;
//		}
//		else if(USART1->DR == LF_CODE)
//		{
//			usart1_receive_buffer[16] = LF_CODE;
//			k = 1;
//			modbus_message_received = 1;
//		}
//		else
//		{
//			usart1_receive_buffer[k] = USART1->DR;
//			k++;
//		}
//	}
//  position_in_program = IN_USART1_INTERRUPT;
//  uint32_t modbus_error = 0;
//  reqest_info_t req = {0};
//  modbus_error = receive_modbus_request(&huart1, &req);
////  if(modbus_error == 0)
////  {
//	  send_modbus_response(&huart1, &req);
////  }
/* USER CODE END 1 */
