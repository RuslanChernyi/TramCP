/***																		***/
/*** Функция FD микросхемы MCP2517FD не используется 						***/
/*** Связь CAN проходит между контроллерами не поддерживающеми протокол FD 	***/

#include "stm32f407xx.h"
#include "stm32f4xx_hal.h"
#include "MCP2517FD.h"
#include "spi.h"
#include "SPI_CAN.h"

MCP2517FD_SFR_t canFD_SFR;

MCP2517FD_CAN_Controller_t canFD_control;

MCP2517FD_FIFOs_t canFD_fifo1;
MCP2517FD_FIFOs_t canFD_fifo2;
MCP2517FD_FIFOs_t canFD_fifo3;
MCP2517FD_FIFOs_t canFD_fifo4;
MCP2517FD_FIFOs_t canFD_fifo5;
MCP2517FD_FIFOs_t canFD_fifo6;
MCP2517FD_FIFOs_t canFD_fifo7;
MCP2517FD_FIFOs_t canFD_fifo8;
MCP2517FD_FIFOs_t canFD_fifo9;
MCP2517FD_FIFOs_t canFD_fifo10;
MCP2517FD_FIFOs_t canFD_fifo11;
MCP2517FD_FIFOs_t canFD_fifo12;
MCP2517FD_FIFOs_t canFD_fifo13;
MCP2517FD_FIFOs_t canFD_fifo14;
MCP2517FD_FIFOs_t canFD_fifo15;
MCP2517FD_FIFOs_t canFD_fifo16;
MCP2517FD_FIFOs_t canFD_fifo17;
MCP2517FD_FIFOs_t canFD_fifo18;
MCP2517FD_FIFOs_t canFD_fifo19;
MCP2517FD_FIFOs_t canFD_fifo20;
MCP2517FD_FIFOs_t canFD_fifo21;
MCP2517FD_FIFOs_t canFD_fifo22;
MCP2517FD_FIFOs_t canFD_fifo23;
MCP2517FD_FIFOs_t canFD_fifo24;
MCP2517FD_FIFOs_t canFD_fifo25;
MCP2517FD_FIFOs_t canFD_fifo26;
MCP2517FD_FIFOs_t canFD_fifo27;
MCP2517FD_FIFOs_t canFD_fifo28;
MCP2517FD_FIFOs_t canFD_fifo29;
MCP2517FD_FIFOs_t canFD_fifo30;
MCP2517FD_FIFOs_t canFD_fifo31;

/*** Configure FIFO ***/

/*** Configure filters ***/

/*** Transmit message ***/

/*** Load message to transmit FIFO ***/

/*** Load message to transmit queue ***/

/*** Request transmit from the transmit FIFO ***/

/*** Request transmit from the transmit queue ***/

/*** Check for ready transmit FIFO's ***/

/*** Set the transmit FIFO priority ***/

/*** Request for remote transmition ***/

/*** Reset transmit FIFO ***/

/*** Reset transmit queue ***/

/*** Filter received message ***/

/*** Read received message ***/

/*** Interrupt handling ***/

/*** Reset CAN controller ***/
HAL_StatusTypeDef can_reset(SPI_HandleTypeDef * hspi, CS_pins spiDev_number)
{
	header_spi address = {0};
	address.command = reset;			// Set command field of the header_spi struct
	address.address = 0x0;

	set_CS_low(spiDev_number);
	HAL_SPI_Transmit(hspi, (uint8_t*)&address, 2, 1);
	set_CS_high(spiDev_number);

	return HAL_OK;
}

/*** Send bytes ***/
HAL_StatusTypeDef send_bytes(SPI_HandleTypeDef * hspi, uint8_t * p_data, header_spi * address, uint16_t length, CS_pins spiDev_number)
{
	address->command = write;			// Set command field of the header_spi struct

	set_CS_low(spiDev_number);
	HAL_SPI_Transmit(hspi, (uint8_t*)address, 2, 1);
	HAL_SPI_Transmit(hspi, p_data, length, 1);
	set_CS_high(spiDev_number);

	return HAL_OK;
}

/*** Receive bytes ***/
uint8_t * receive_bytes(SPI_HandleTypeDef * hspi, header_spi * address, uint16_t length, CS_pins spiDev_number)
{
	static uint8_t received_data[8] = {0};	// Maximum there can be 8 bytes of data
	address->command = read;

	set_CS_low(spiDev_number);
	HAL_SPI_Transmit(hspi, (uint8_t*)address, 2, 1);
	HAL_SPI_Receive(hspi, received_data, length, 1);
	set_CS_high(spiDev_number);

	return received_data;
}

/*** Put in configuration mode ***/
HAL_StatusTypeDef putin_confmode(SPI_HandleTypeDef * hspi, CS_pins spiDev_number)
{
	uint8_t SFRreg[4] = {0};
	SFRreg[3] = config_mode;
	header_spi address = {0};
	address.command = write;			// Set command field of the header_spi struct
	address.address = 0x0;

	send_bytes(hspi, SFRreg, &address, sizeof(SFRreg)/8, spiDev_number);
	HAL_Delay(10);
	uint8_t * p_buffer = receive_bytes(hspi, &address, 4, spiDev_number);
	uint8_t receiveBuffer[8] = { *p_buffer, *(p_buffer+1), *(p_buffer+2), *(p_buffer+3), *(p_buffer+4), *(p_buffer+5), *(p_buffer+6), *(p_buffer+7)};
	uint8_t value_to_check = receiveBuffer[2] && 0x7<<5;
	if(value_to_check != config_mode)
	{
		return HAL_ERROR;
	}
	return HAL_OK;

}

