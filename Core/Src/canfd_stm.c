/*
 * canfd_stm.c
 *
 *  Created on: Jun 1, 2022
 *      Author: Kimo
 */

#include "drv_canfdspi_api.h"
#include "drv_canfdspi_defines.h"
#include "drv_canfdspi_register.h"
#include "can.h"
#include "main.h"
#include "canfd_stm.h"

//extern uint8_t spi_receive_blahbuffer[4];
typedef struct mcpfd_struct_t
{
	REG_CiCON cicon;
	REG_CiNBTCFG cinbtcfg;
	REG_CiDBTCFG cidbtcfg;
	REG_CiTDC citdc;
	REG_CiTSCON citscon;
	REG_CiVEC civec;
	CAN_INT_FLAGS int_flags;
	CAN_INT_ENABLES int_enables;
	REG_CiINT ciint;
	REG_CiINTFLAG ciintflag;
	REG_CiINTENABLE ciintenable;
	REG_CiTREC citrec;
	REG_CiBDIAG0 cibdiag0;
	REG_CiBDIAG1 cibdiag1;
	REG_CiTEFCON citefcon;
	REG_CiTEFSTA citefsta;
	REG_CiTXQCON citxqcon;

	REG_OSC osc;
	REG_IOCON iocon;
	REG_CRC crc;
	REG_ECCCON ecccon;
	REG_ECCSTA eccsta;
}mcpfd_struct;

mcpfd_struct mcp2517fd_CAN6 = {0};
extern uint8_t spi_receive_blahbuffer[4];

uint32_t Configure_CAN()
{
	uint8_t buffer[6] = {0};
	uint16_t writeCommand = 0;

	writeCommand = (/*cREGADDR_OSC*/ /*cREGADDR_IOCON*/ cREGADDR_OSC & 0x0FFF) | (cINSTRUCTION_READ << 12);
	buffer[0] = writeCommand >> 8;
	buffer[1] = writeCommand & 0xFF;

	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 0);
	SPI_Transmit(buffer, 6, SPI2);
	SPI_Receive(spi_receive_blahbuffer, 4, SPI2);
	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 1);

	for(int i = 0; i < sizeof(spi_receive_blahbuffer); i++)
	{
		mcp2517fd_CAN6.osc.byte[i] = spi_receive_blahbuffer[i];
	}

	REG_OSC oscon = {0};
	oscon.bF.OscDisable = 0;
	oscon.bF.PllEnable = 0;
	oscon.bF.SCLKDIV = 0;
	oscon.bF.CLKODIV = 00;

	writeCommand = (/*cREGADDR_OSC*/ /*cREGADDR_IOCON*/ cREGADDR_OSC & 0x0FFF) | (cINSTRUCTION_WRITE << 12);
	buffer[0] = writeCommand >> 8;
	buffer[1] = writeCommand & 0xFF;
	for(int i = 0; i < sizeof(oscon.byte); i++)
	{
		buffer[i+2] = oscon.byte[i];
	}
	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 0);
	SPI_Transmit(buffer, 6, SPI2);
	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 1);

	for(int i = 0; i < sizeof(buffer); i++)
	{
		buffer[i] = 0;
	}


	writeCommand = (/*cREGADDR_OSC*/ /*cREGADDR_IOCON*/ cREGADDR_OSC & 0x0FFF) | (cINSTRUCTION_READ << 12);
	buffer[0] = writeCommand >> 8;
	buffer[1] = writeCommand & 0xFF;

	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 0);
	SPI_Transmit(buffer, 6, SPI2);
	SPI_Receive(spi_receive_blahbuffer, 4, SPI2);
	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 1);

	for(int i = 0; i < sizeof(spi_receive_blahbuffer); i++)
	{
		mcp2517fd_CAN6.osc.byte[i] = spi_receive_blahbuffer[i];
	}






	writeCommand = (/*cREGADDR_OSC*/ /*cREGADDR_IOCON*/ cREGADDR_CiCON & 0x0FFF) | (cINSTRUCTION_READ << 12);
	buffer[0] = writeCommand >> 8;
	buffer[1] = writeCommand & 0xFF;

	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 0);
	SPI_Transmit(buffer, 6, SPI2);
	SPI_Receive(spi_receive_blahbuffer, 4, SPI2);
	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 1);

	for(int i = 0; i < sizeof(spi_receive_blahbuffer); i++)
	{
		mcp2517fd_CAN6.cicon.byte[i] = spi_receive_blahbuffer[i];
	}

	for(int i = 0; i < sizeof(buffer); i++)
	{
		buffer[i] = 0;
	}

	REG_CiCON cicon = {0};
	cicon.bF.RequestOpMode = 0x6;
	cicon.bF.DNetFilterCount = 0;
	cicon.bF.IsoCrcEnable = 1;
	cicon.bF.ProtocolExceptionEventDisable = 1;
	cicon.bF.WakeUpFilterEnable = 0;
	cicon.bF.WakeUpFilterTime = 0;
	cicon.bF.BitRateSwitchDisable = 1;
	cicon.bF.RestrictReTxAttempts = 0;
	cicon.bF.EsiInGatewayMode = 0;
	cicon.bF.SystemErrorToListenOnly = 1;
	cicon.bF.StoreInTEF = 1;
	cicon.bF.TXQEnable = 1;
	cicon.bF.AbortAllTx = 0;
	cicon.bF.TxBandWidthSharing = 0;
	writeCommand = (/*cREGADDR_OSC*/ /*cREGADDR_IOCON*/ cREGADDR_CiCON & 0x0FFF) | (cINSTRUCTION_WRITE << 12);
	buffer[0] = writeCommand >> 8;
	buffer[1] = writeCommand & 0xFF;
	for(int i = 0; i < sizeof(cicon.byte); i++)
	{
		buffer[i+2] = cicon.byte[i];
	}
	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 0);
	SPI_Transmit(buffer, 6, SPI2);
	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 1);

	for(int i = 0; i < sizeof(buffer); i++)
	{
		buffer[i] = 0;
	}

	writeCommand = (/*cREGADDR_OSC*/ /*cREGADDR_IOCON*/ cREGADDR_CiCON & 0x0FFF) | (cINSTRUCTION_READ << 12);
	buffer[0] = writeCommand >> 8;
	buffer[1] = writeCommand & 0xFF;

	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 0);
	SPI_Transmit(buffer, 6, SPI2);
	SPI_Receive(spi_receive_blahbuffer, 4, SPI2);
	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 1);
	for(int i = 0; i < sizeof(spi_receive_blahbuffer); i++)
	{
		mcp2517fd_CAN6.cicon.byte[i] = spi_receive_blahbuffer[i];
	}
}

void spican_writeByte(uint32_t address, uint8_t message)
{
	uint8_t buffer[3] = {0};
	uint16_t writeCommand = (address & 0x0FFF) | (cINSTRUCTION_WRITE << 12);
	buffer[0] = writeCommand >> 8;
	buffer[1] = writeCommand & 0xFF;
	buffer[2] = message;

	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 0);
	SPI_Transmit(buffer, 3, SPI2);
	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 1);
}

uint8_t spican_readByte(uint32_t address)
{
	uint8_t buffer[3] = {0};
	uint16_t writeCommand = (address & 0x0FFF) | (cINSTRUCTION_READ << 12);
	buffer[0] = writeCommand >> 8;
	buffer[1] = writeCommand & 0xFF;

	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 0);
	SPI_Transmit(buffer, 3, SPI2);
	SPI_Receive(&buffer[2], 1, SPI2);
	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 1);

	return buffer[2];
}

void spican_read32bitReg(uint32_t address, uint8_t * reg_buffer)
{
	int8_t buffer[6] = {0};
	uint16_t writeCommand = (address & 0x0FFF) | (cINSTRUCTION_READ << 12);
	buffer[0] = writeCommand >> 8;
	buffer[1] = writeCommand & 0xFF;

	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 0);
	SPI_Transmit(buffer, 6, SPI2);
	SPI_Receive(reg_buffer, 4, SPI2);
	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 1);

}

void spican_readBytes(uint32_t address, uint8_t * rx_buffer, uint32_t size)
{
	int8_t buffer[2 + size];
	uint16_t writeCommand = (address & 0x0FFF) | (cINSTRUCTION_READ << 12);
	buffer[0] = writeCommand >> 8;
	buffer[1] = writeCommand & 0xFF;
	for(int i = 2; i < sizeof(buffer); i++)
	{
		buffer[i] = 0;
	}

	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 0);
	SPI_Transmit(buffer, sizeof(buffer), SPI2);
	SPI_Receive(rx_buffer, size, SPI2);
	HAL_GPIO_WritePin(CAN6_CS_GPIO_Port, CAN6_CS_Pin, 1);

}
