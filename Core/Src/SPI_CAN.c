/*
 * SPI_CAN.c
 *
 *  Created on: Feb 2, 2022
 *      Author: Kimo
 */

#include "spi.h"
#include "SPI_CAN.h"
#include "gpio.h"
#include "main.h"

typedef enum
{
	NO,
	YES
}answer;

uint8_t SPI1_MCPFD_TransferData(SPI_HandleTypeDef * hspi, uint8_t CSn, uint8_t * spiTxData, uint8_t * spiRxData, uint32_t spiTransferSize)
{
	uint32_t continueLoop = 0;
	uint32_t txCounter = 0;
	uint32_t rxCounter = 0;
	uint32_t unitsTransmited = 0;
	const uint32_t maxUnits = 16;

	set_CS_low(CSn);

	//Loop until spiTransferSize
	do{
		continueLoop = NO;
		unitsTransmited = 0;

		/*** Fill tranmsit FIFO ***/
		/** Check if transmit buffer is empty **/
		if((hspi->Instance->SR & 1<<1) != 0)	// TXE (bit 1)
		{
			while((txCounter < spiTransferSize) && (unitsTransmited != maxUnits))
			{
				HAL_SPI_Transmit(hspi, &spiTxData[txCounter], 1, 1);
				txCounter++;
				unitsTransmited++;
				continueLoop = YES;
			}
		}
		/*** Read as many bytes as were queued for transmission ***/
		while(txCounter != rxCounter)
		{
			while(!(hspi->Instance->SR & (1U<<0)) == 0);	//If RXNE == 0 - receive buffer is empty
			HAL_SPI_Receive(hspi, &spiRxData[rxCounter], 1, 1);
			rxCounter++;
			continueLoop = YES;
		}

		/*** Make sure data gets transmitted even if buffer wasn't empty at the start ***/
		if((txCounter > rxCounter) || (txCounter < spiTransferSize))
		{
				continueLoop = YES;
		}

	}while(continueLoop == YES);

	set_CS_high(CSn);

	return 0;
}

void set_CS_low(CS_pins CS_number)
{
	switch(CS_number)
	{
		case SS1:
			HAL_GPIO_WritePin(SS1_GPIO_Port, SS1_Pin, 0);
			break;
		case SS2:
			HAL_GPIO_WritePin(SS2_GPIO_Port, SS2_Pin, 0);
			break;
		case CS_CAN3:
			HAL_GPIO_WritePin(CAN3_CS_GPIO_Port, CAN3_CS_Pin, 0);
			break;
		case CS_CAN4:
			HAL_GPIO_WritePin(CAN4_CS_GPIO_Port, CAN4_CS_Pin, 0);
			break;
		case CS_CAN5:
			HAL_GPIO_WritePin(CAN5_CS_GPIO_Port, CAN5_CS_Pin, 0);
			break;
		case CS_CAN6:
			HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 0);
			break;
	}
}

void set_CS_high(CS_pins CS_number)
{
	switch(CS_number)
	{
		case SS1:
			HAL_GPIO_WritePin(SS1_GPIO_Port, SS1_Pin, 1);
			break;
		case SS2:
			HAL_GPIO_WritePin(SS2_GPIO_Port, SS2_Pin, 1);
			break;
		case CS_CAN3:
			HAL_GPIO_WritePin(CAN3_CS_GPIO_Port, CAN3_CS_Pin, 1);
			break;
		case CS_CAN4:
			HAL_GPIO_WritePin(CAN4_CS_GPIO_Port, CAN4_CS_Pin, 1);
			break;
		case CS_CAN5:
			HAL_GPIO_WritePin(CAN5_CS_GPIO_Port, CAN5_CS_Pin, 1);
			break;
		case CS_CAN6:
			HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 1);
			break;
	}
}
