#ifndef SPI_CAN_H
#define SPI_CAN_H

typedef enum spi_to_can_instruction
{
	reset,
	read = 0x3,
	write = 0x2,
	read_crc = 0xB,
	write_crc = 0xA,
	write_safe = 0xC
}command;

typedef enum CS_pins
{
	SS1,
	SS2,
	CS_CAN3,
	CS_CAN4,
	CS_CAN5,
	CS_CAN6
}CS_pins;

typedef struct header_spi
{
	unsigned command : 4;
	unsigned address : 12;
}header_spi;

//typedef struct spi_to_can_message
//{
//	unsigned address : 12;
//	uint8_t * p_to_data;
//	uint32_t data_size;
//}spi_to_can_message;

HAL_StatusTypeDef putin_confmode(SPI_HandleTypeDef * hspi, CS_pins spiDev_number);
HAL_StatusTypeDef can_reset(SPI_HandleTypeDef * hspi, CS_pins spiDev_number);
uint8_t SPI1_MCPFD_TransferData(SPI_HandleTypeDef * hspi, uint8_t CSn, uint8_t * spiTxData, uint8_t * spiRxData, uint32_t spiTransferSize);

void set_CS_low(CS_pins CS_number);
void set_CS_high(CS_pins CS_number);

#endif
