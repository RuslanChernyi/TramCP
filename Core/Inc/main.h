/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void SPI_Transmit (uint8_t *data, int size, SPI_TypeDef * SPIx);
void SPI_Receive (uint8_t *data, int size, SPI_TypeDef * SPIx);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CAN3_CS_Pin GPIO_PIN_2
#define CAN3_CS_GPIO_Port GPIOE
#define CAN3_CLKO_Pin GPIO_PIN_3
#define CAN3_CLKO_GPIO_Port GPIOE
#define CAN3_INT_Pin GPIO_PIN_4
#define CAN3_INT_GPIO_Port GPIOE
#define CAN3_INT0_Pin GPIO_PIN_5
#define CAN3_INT0_GPIO_Port GPIOE
#define CAN3_INT1_Pin GPIO_PIN_6
#define CAN3_INT1_GPIO_Port GPIOE
#define CAN4_CS_Pin GPIO_PIN_15
#define CAN4_CS_GPIO_Port GPIOC
#define CAN4_CLKO_Pin GPIO_PIN_0
#define CAN4_CLKO_GPIO_Port GPIOF
#define CAN4_INT_Pin GPIO_PIN_1
#define CAN4_INT_GPIO_Port GPIOF
#define CAN4_INT0_Pin GPIO_PIN_2
#define CAN4_INT0_GPIO_Port GPIOF
#define CAN4_INT1_Pin GPIO_PIN_3
#define CAN4_INT1_GPIO_Port GPIOF
#define Y_5V_Pin GPIO_PIN_0
#define Y_5V_GPIO_Port GPIOA
#define YVCC_Pin GPIO_PIN_1
#define YVCC_GPIO_Port GPIOA
#define T1_Pin GPIO_PIN_2
#define T1_GPIO_Port GPIOA
#define SS1_Pin GPIO_PIN_5
#define SS1_GPIO_Port GPIOC
#define SS2_Pin GPIO_PIN_0
#define SS2_GPIO_Port GPIOB
#define K1_Pin GPIO_PIN_14
#define K1_GPIO_Port GPIOF
#define K2_Pin GPIO_PIN_15
#define K2_GPIO_Port GPIOF
#define K3_Pin GPIO_PIN_0
#define K3_GPIO_Port GPIOG
#define K4_Pin GPIO_PIN_1
#define K4_GPIO_Port GPIOG
#define K5_Pin GPIO_PIN_7
#define K5_GPIO_Port GPIOE
#define K6_Pin GPIO_PIN_8
#define K6_GPIO_Port GPIOE
#define XZ1_HL4_Pin GPIO_PIN_13
#define XZ1_HL4_GPIO_Port GPIOE
#define XZ2_HL5_Pin GPIO_PIN_14
#define XZ2_HL5_GPIO_Port GPIOE
#define MCU_CONTROL_MEANDR_Pin GPIO_PIN_8
#define MCU_CONTROL_MEANDR_GPIO_Port GPIOD
#define CAN6_INT1_Pin GPIO_PIN_9
#define CAN6_INT1_GPIO_Port GPIOD
#define CAN6_INT0_Pin GPIO_PIN_10
#define CAN6_INT0_GPIO_Port GPIOD
#define CAN6_INT_Pin GPIO_PIN_11
#define CAN6_INT_GPIO_Port GPIOD
#define CAN6_CLKO_Pin GPIO_PIN_12
#define CAN6_CLKO_GPIO_Port GPIOD
#define CAN6_CS_Pin GPIO_PIN_13
#define CAN6_CS_GPIO_Port GPIOD
#define CAN5_INT1_Pin GPIO_PIN_4
#define CAN5_INT1_GPIO_Port GPIOG
#define CAN5_INT0_Pin GPIO_PIN_5
#define CAN5_INT0_GPIO_Port GPIOG
#define CAN5_INT_Pin GPIO_PIN_6
#define CAN5_INT_GPIO_Port GPIOG
#define CAN5_CLKO_Pin GPIO_PIN_7
#define CAN5_CLKO_GPIO_Port GPIOG
#define CAN5_CS_Pin GPIO_PIN_8
#define CAN5_CS_GPIO_Port GPIOG
#define U1DE_Pin GPIO_PIN_8
#define U1DE_GPIO_Port GPIOA
#define U2DE_Pin GPIO_PIN_4
#define U2DE_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

typedef enum program_positions
{
	IN_MAIN,
	IN_USART1_INTERRUPT,
	IN_USART2_INTERRUPT,
	IN_CAN1_INTERRUPT,
	IN_TIM7_INTERRUPT
}program_positions_enum;
///******************* DIAG_struct START ******************/
//typedef struct dicr0
//{
//	uint8_t XET3:1;
//	uint8_t res:7;
//}dicr0;
//typedef struct dicr1
//{
//	uint8_t XSNI:1;
//	uint8_t XKL1:1;
//	uint8_t XKL3:1;
//	uint8_t XAT:1;
//	uint8_t XET1:1;
//	uint8_t XMT:1;
//	uint8_t res:2;
//}dicr1;
//typedef struct dicr2
//{
//	uint8_t XZ3:1;
//	uint8_t XZP:1;
//	uint8_t res1:1;
//	uint8_t XT:1;
//	uint8_t res2:1;
//	uint8_t XM:1;
//	uint8_t XKYB:1;
//	uint8_t XSI:1;
//}dicr2;
//typedef struct dicr3
//{
//	uint8_t res1:3;
//	uint8_t XP:1;
//	uint8_t XZ:1;
//	uint8_t XH:1;
//	uint8_t XZ1:1;
//	uint8_t XZ2:1;
//}dicr3;
//typedef struct dicr4
//{
//	uint8_t res:8;
//}dicr4;
//typedef struct dicr5
//{
//	uint8_t res:8;
//}dicr5;
//typedef struct dicr6
//{
//	uint8_t res:8;
//}dicr6;
//typedef struct dicr7
//{
//	uint8_t YKL3:1;
//	uint8_t YYB:1;
//	uint8_t YK04:1;
//	uint8_t MT1:1;
//	uint8_t MT2:1;
//	uint8_t YRT:1;
//	uint8_t YDET:1;
//	uint8_t YGR:1;
//}dicr7;
//typedef struct dicr8
//{
//	uint8_t res:8;
//}dicr8;
//typedef struct dicr9
//{
//	uint8_t res:8;
//}dicr9;
//typedef struct dicr10
//{
//	uint8_t XDVT5:1;
//	uint8_t XDVT6:1;
//	uint8_t XDVT7:1;
//	uint8_t XDVT:1;
//	uint8_t XDI:1;
//	uint8_t XDU:1;
//	uint8_t YVENT:1;
//	uint8_t res:1;
//}dicr10;
//typedef struct dicr11
//{
//	uint8_t res:1;
//	uint8_t XKT:1;
//	uint8_t XVENT:1;
//	uint8_t TZ:1;
//	uint8_t XDVT1:1;
//	uint8_t XDVT2:1;
//	uint8_t XDVT3:1;
//	uint8_t XDVT4:1;
//}dicr11;
//
//typedef struct DIAG
//{
//	dicr0 byte0;
//	dicr1 byte1;
//	dicr2 byte2;
//	dicr3 byte3;
//	dicr4 byte4;
//	dicr5 byte5;
//	dicr6 byte6;
//	dicr7 byte7;
//	dicr8 byte8;
//	dicr9 byte9;
//	dicr10 byte10;
//	dicr11 byte11;
//	uint16_t I;
//	uint16_t U;
//	uint16_t XTT;
//	uint16_t XTH;
//	uint16_t res;
//	uint16_t CI;
//}diag_t;
//
//typedef union diag
//{
//	diag_t diag_struct;
//	uint8_t diag_array[24];
//}diag;
///******************* DIAG_struct END ******************/
//
///******************* IOboardStruct START ******************/
///*** 1 ***/
//typedef struct IOboardStruct
//{
//	uint8_t prefixCmd;
//	uint8_t boardAdress;
//	uint8_t cmdNum;
//	uint8_t answer;
//	uint8_t ending;
//}IOboardPayload_t;
//
//typedef union IOboardPayload
//{
//	IOboardPayload_t IOmessageStruct;
//	uint8_t IOmessageArray[5];
//}IOboardPayload;
///*** 2 ***/
//typedef struct IOboardStructADC
//{
//	uint8_t prefixCmd;
//	uint8_t boardAdress;
//	uint8_t cmdNum;
//	uint8_t answer[3];
//	uint8_t ending;
//}IOboardStructADC_t;
//
//typedef struct IOboardStructDOUT
//{
//	uint8_t prefixCmd;
//	uint8_t boardAdress;
//	uint8_t cmdNum;
//	uint8_t answer[3];
//	uint8_t ending;
//}IOboardStructDOUT_t;
//
//typedef struct IOboardStructXAx
//{
//	uint8_t prefixCmd;
//	uint8_t boardAdress;
//	uint8_t cmdNum;
//	uint8_t answer[3];
//	uint8_t ending;
//}IOboardStructXAx_t;
//
//typedef union IOboardPayload2
//{
//	IOboardStructADC_t IO_ADCmessageStruct;
//	IOboardStructDOUT_t IO_DOUTmessageStruct;
//	IOboardStructXAx_t IO_XAxmessageStruct;
//	uint8_t IOmessageArray[8];
//}IOboardPayload2;

/******************* IOboardStruct END ******************/

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
