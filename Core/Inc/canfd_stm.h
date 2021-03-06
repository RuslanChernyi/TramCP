/*
 * canfd_stm.h
 *
 *  Created on: Jun 1, 2022
 *      Author: Kimo
 */

#ifndef INC_CANFD_STM_H_
#define INC_CANFD_STM_H_

#include "drv_canfdspi_register.h"

//typedef struct mcp2517fd_t
//{
//	REG_CiCON CiCON;
//	REG_CiNBTCFG CiNBTCFG;
//	REG_CiDBTCFG CiDBTCFG;
//	REG_CiTDC CiTDC;
//	REG_CiTBC CiTBC;
//	REG_CiTSCON CiTSCON;
//	REG_CiVEC CiVEC;
//	REG_CiINT CiINT;
//
//	REG_CiRXIF CiRXIF;
//	REG_CiTXIF CiTXIF;
//	REG_CiRXOVIF CiRXOVIF;
//	REG_CiTXATIF CiTXATIF;
//	REG_CiTXREQ CiTXREQ;
//
//	REG_CiTREC CiTREC;
//	REG_CiBDIAG0 CiBDIAG0;
//	REG_CiBDIAG1 CiBDIAG1;
//	REG_CiTEFCON CiTEFCON;
//	REG_CiTEFSTA CiTEFSTA;
//	REG_CiTEFUA CiTEFUA;
//	uint32_t reserved;
//
//	REG_CiTXQCON CiTXQCON;
//	REG_CiTXQSTA CiTXQSTA;
//	REG_CiTXQUA CiTXQUA;
//
//	REG_CiFIFOCON CiFIFOCON1;
//	REG_CiFIFOSTA CiFIFOSTA1;
//	REG_CiFIFOUA CiFIFOUA1;
//
//	REG_CiFIFOCON CiFIFOCON2;
//	REG_CiFIFOSTA CiFIFOSTA2;
//	REG_CiFIFOUA CiFIFOUA2;
//
//	REG_CiFIFOCON CiFIFOCON3;
//	REG_CiFIFOSTA CiFIFOSTA3;
//	REG_CiFIFOUA CiFIFOUA3;
//
//	REG_CiFIFOCON CiFIFOCON4;
//	REG_CiFIFOSTA CiFIFOSTA4;
//	REG_CiFIFOUA CiFIFOUA4;
//
//	REG_CiFIFOCON CiFIFOCON5;
//	REG_CiFIFOSTA CiFIFOSTA5;
//	REG_CiFIFOUA CiFIFOUA5;
//
//	REG_CiFIFOCON CiFIFOCON6;
//	REG_CiFIFOSTA CiFIFOSTA6;
//	REG_CiFIFOUA CiFIFOUA6;
//
//	REG_CiFIFOCON CiFIFOCON7;
//	REG_CiFIFOSTA CiFIFOSTA7;
//	REG_CiFIFOUA CiFIFOUA7;
//
//	REG_CiFIFOCON CiFIFOCON8;
//	REG_CiFIFOSTA CiFIFOSTA8;
//	REG_CiFIFOUA CiFIFOUA8;
//
//	REG_CiFIFOCON CiFIFOCON9;
//	REG_CiFIFOSTA CiFIFOSTA9;
//	REG_CiFIFOUA CiFIFOUA9;
//
//	REG_CiFIFOCON CiFIFOCON10;
//	REG_CiFIFOSTA CiFIFOSTA10;
//	REG_CiFIFOUA CiFIFOUA10;
//
//	REG_CiFIFOCON CiFIFOCON11;
//	REG_CiFIFOSTA CiFIFOSTA11;
//	REG_CiFIFOUA CiFIFOUA11;
//
//	REG_CiFIFOCON CiFIFOCON12;
//	REG_CiFIFOSTA CiFIFOSTA12;
//	REG_CiFIFOUA CiFIFOUA12;
//
//	REG_CiFIFOCON CiFIFOCON13;
//	REG_CiFIFOSTA CiFIFOSTA13;
//	REG_CiFIFOUA CiFIFOUA13;
//
//	REG_CiFIFOCON CiFIFOCON14;
//	REG_CiFIFOSTA CiFIFOSTA14;
//	REG_CiFIFOUA CiFIFOUA14;
//
//	REG_CiFIFOCON CiFIFOCON15;
//	REG_CiFIFOSTA CiFIFOSTA15;
//	REG_CiFIFOUA CiFIFOUA15;
//
//	REG_CiFIFOCON CiFIFOCON16;
//	REG_CiFIFOSTA CiFIFOSTA16;
//	REG_CiFIFOUA CiFIFOUA16;
//
//	REG_CiFIFOCON CiFIFOCON17;
//	REG_CiFIFOSTA CiFIFOSTA17;
//	REG_CiFIFOUA CiFIFOUA17;
//
//	REG_CiFIFOCON CiFIFOCON18;
//	REG_CiFIFOSTA CiFIFOSTA18;
//	REG_CiFIFOUA CiFIFOUA18;
//
//	REG_CiFIFOCON CiFIFOCON19;
//	REG_CiFIFOSTA CiFIFOSTA19;
//	REG_CiFIFOUA CiFIFOUA19;
//
//	REG_CiFIFOCON CiFIFOCON20;
//	REG_CiFIFOSTA CiFIFOSTA20;
//	REG_CiFIFOUA CiFIFOUA20;
//
//	REG_CiFIFOCON CiFIFOCON21;
//	REG_CiFIFOSTA CiFIFOSTA21;
//	REG_CiFIFOUA CiFIFOUA21;
//
//	REG_CiFIFOCON CiFIFOCON22;
//	REG_CiFIFOSTA CiFIFOSTA22;
//	REG_CiFIFOUA CiFIFOUA22;
//
//	REG_CiFIFOCON CiFIFOCON23;
//	REG_CiFIFOSTA CiFIFOSTA23;
//	REG_CiFIFOUA CiFIFOUA23;
//
//	REG_CiFIFOCON CiFIFOCON24;
//	REG_CiFIFOSTA CiFIFOSTA24;
//	REG_CiFIFOUA CiFIFOUA24;
//
//	REG_CiFIFOCON CiFIFOCON25;
//	REG_CiFIFOSTA CiFIFOSTA25;
//	REG_CiFIFOUA CiFIFOUA25;
//
//	REG_CiFIFOCON CiFIFOCON26;
//	REG_CiFIFOSTA CiFIFOSTA26;
//	REG_CiFIFOUA CiFIFOUA26;
//
//	REG_CiFIFOCON CiFIFOCON27;
//	REG_CiFIFOSTA CiFIFOSTA27;
//	REG_CiFIFOUA CiFIFOUA27;
//
//	REG_CiFIFOCON CiFIFOCON28;
//	REG_CiFIFOSTA CiFIFOSTA28;
//	REG_CiFIFOUA CiFIFOUA28;
//
//	REG_CiFIFOCON CiFIFOCON29;
//	REG_CiFIFOSTA CiFIFOSTA29;
//	REG_CiFIFOUA CiFIFOUA29;
//
//	REG_CiFIFOCON CiFIFOCON30;
//	REG_CiFIFOSTA CiFIFOSTA30;
//	REG_CiFIFOUA CiFIFOUA30;
//
//	REG_CiFIFOCON CiFIFOCON31;
//	REG_CiFIFOSTA CiFIFOSTA31;
//	REG_CiFIFOUA CiFIFOUA31;
//
//	REG_CiFLTCON CiFLTCON0;
//	REG_CiFLTCON CiFLTCON1;
//	REG_CiFLTCON CiFLTCON2;
//	REG_CiFLTCON CiFLTCON3;
//	REG_CiFLTCON CiFLTCON4;
//	REG_CiFLTCON CiFLTCON5;
//	REG_CiFLTCON CiFLTCON6;
//	REG_CiFLTCON CiFLTCON7;
//
//	REG_CiFLTOBJ CiFLTOBJ0;
//	REG_CiMASK CiMASK0;
//
//	REG_CiFLTOBJ CiFLTOBJ1;
//	REG_CiMASK CiMASK1;
//
//	REG_CiFLTOBJ CiFLTOBJ2;
//	REG_CiMASK CiMASK2;
//
//	REG_CiFLTOBJ CiFLTOBJ3;
//	REG_CiMASK CiMASK3;
//
//	REG_CiFLTOBJ CiFLTOBJ4;
//	REG_CiMASK CiMASK4;
//
//	REG_CiFLTOBJ CiFLTOBJ5;
//	REG_CiMASK CiMASK5;
//
//	REG_CiFLTOBJ CiFLTOBJ6;
//	REG_CiMASK CiMASK6;
//
//	REG_CiFLTOBJ CiFLTOBJ7;
//	REG_CiMASK CiMASK7;
//
//	REG_CiFLTOBJ CiFLTOBJ8;
//	REG_CiMASK CiMASK8;
//
//	REG_CiFLTOBJ CiFLTOBJ9;
//	REG_CiMASK CiMASK9;
//
//	REG_CiFLTOBJ CiFLTOBJ10;
//	REG_CiMASK CiMASK10;
//
//	REG_CiFLTOBJ CiFLTOBJ11;
//	REG_CiMASK CiMASK11;
//
//	REG_CiFLTOBJ CiFLTOBJ12;
//	REG_CiMASK CiMASK12;
//
//	REG_CiFLTOBJ CiFLTOBJ13;
//	REG_CiMASK CiMASK13;
//
//	REG_CiFLTOBJ CiFLTOBJ14;
//	REG_CiMASK CiMASK14;
//
//	REG_CiFLTOBJ CiFLTOBJ15;
//	REG_CiMASK CiMASK15;
//
//	REG_CiFLTOBJ CiFLTOBJ16;
//	REG_CiMASK CiMASK16;
//
//	REG_CiFLTOBJ CiFLTOBJ17;
//	REG_CiMASK CiMASK17;
//
//	REG_CiFLTOBJ CiFLTOBJ18;
//	REG_CiMASK CiMASK18;
//
//	REG_CiFLTOBJ CiFLTOBJ19;
//	REG_CiMASK CiMASK19;
//
//	REG_CiFLTOBJ CiFLTOBJ20;
//	REG_CiMASK CiMASK20;
//
//	REG_CiFLTOBJ CiFLTOBJ21;
//	REG_CiMASK CiMASK21;
//
//	REG_CiFLTOBJ CiFLTOBJ22;
//	REG_CiMASK CiMASK22;
//
//	REG_CiFLTOBJ CiFLTOBJ23;
//	REG_CiMASK CiMASK23;
//
//	REG_CiFLTOBJ CiFLTOBJ24;
//	REG_CiMASK CiMASK24;
//
//	REG_CiFLTOBJ CiFLTOBJ25;
//	REG_CiMASK CiMASK25;
//
//	REG_CiFLTOBJ CiFLTOBJ26;
//	REG_CiMASK CiMASK26;
//
//	REG_CiFLTOBJ CiFLTOBJ27;
//	REG_CiMASK CiMASK27;
//
//	REG_CiFLTOBJ CiFLTOBJ28;
//	REG_CiMASK CiMASK28;
//
//	REG_CiFLTOBJ CiFLTOBJ29;
//	REG_CiMASK CiMASK29;
//
//	REG_CiFLTOBJ CiFLTOBJ30;
//	REG_CiMASK CiMASK30;
//
//	REG_CiFLTOBJ CiFLTOBJ31;
//	REG_CiMASK CiMASK31;
//
//	REG_OSC OSC;
//	REG_IOCON IOCON;
//	REG_CRC CRC_reg;
//	REG_ECCCON ECCCON;
//	REG_ECCSTA ECCSTA;
//
//}mcp2517fd;

typedef struct mcp_status_t
{
	REG_CiCON Configuration;
	REG_CiVEC Interrupt_vector;
	REG_CiINT Interrupt_flags;
	REG_CiTXIF Transmit_interrupt_status;
	REG_CiTXATIF Transmit_attempt_interrupt;
	REG_CiTXREQ	Transmit_request;
	REG_CiTREC Transmit_Receive_errorCount;
	REG_CiBDIAG0 BusDiagnostic_0;
	REG_CiBDIAG1 BusDiagnostic_1;
	REG_CiFIFOCON FIFO1_Configuration;
	REG_CiFIFOCON FIFO2_Configuration;
	REG_CiFIFOSTA FIFO1_Status;
	REG_CiFIFOSTA FIFO2_Status;
	REG_CiFIFOUA FIFO1_NextAddress;
	REG_CiFIFOUA FIFO2_NextAddress;

	REG_CiFLTCON Filter0;
	REG_CiFLTCON Filter1;
	REG_CiFLTCON Filter2;
	REG_CiFLTCON Filter3;
	REG_CiFLTCON Filter4;
	REG_CiFLTCON Filter5;
	REG_CiFLTCON Filter6;
	REG_CiFLTCON Filter7;

	REG_CiFLTOBJ FltObj0;
	REG_CiFLTOBJ FltObj1;
	REG_CiFLTOBJ FltObj2;
	REG_CiFLTOBJ FltObj3;
	REG_CiFLTOBJ FltObj4;
	REG_CiFLTOBJ FltObj5;

	REG_CiMASK Mask0;
	REG_CiMASK Mask1;
	REG_CiMASK Mask2;
	REG_CiMASK Mask3;
	REG_CiMASK Mask4;
	REG_CiMASK Mask5;

	REG_OSC	Oscillator_configuration_and_status;
	REG_IOCON GPIO_Status;

	uint8_t can_RAM[320];

}mcp_status;

typedef enum
{
	RECEIVE_FIFO,
	TRANSMIT_FIFO
}fifo_type;

typedef struct
{
	uint32_t Filter_number;
	uint32_t SID_value;
	uint32_t Mask_value;
}Filter_t;

typedef struct
{
	REG_CiFIFOCON FIFOxCON;
	REG_CiFIFOSTA FIFOxSTA;
	REG_CiFIFOUA  FIFOxUA;
	fifo_type FIFOType;
	CAN_FIFO_CHANNEL FIFOx;
	Filter_t FIFOsFilter;
}FIFO_t;

typedef struct
{
	REG_CiTEFCON TEFCON;
	REG_CiTEFSTA TEFSTA;
	REG_CiTEFUA	 TEFUA;
}TransmitEventFIFO;

typedef struct
{
	REG_CiTXQCON TEFCON;
	REG_CiTXQSTA TEFSTA;
	REG_CiTXQUA	 TEFUA;
}TransmitQueue;

typedef struct UsedFIFOs_t
{
	struct
	{
		TransmitEventFIFO TEF;
		TransmitQueue TQ;
		FIFO_t one;
	}TransmitFIFOs;
	struct
	{
		FIFO_t two;
		FIFO_t three;
		FIFO_t four;
		FIFO_t five;
		FIFO_t six;
		FIFO_t seven;
		FIFO_t eight;
	}ReceiveFIFOs;
}UsedFIFOs;

typedef struct spiCAN_t
{
	SPI_TypeDef * SPIx;

	DMA_TypeDef * DMAx;
	DMA_Stream_TypeDef * SPI_TX_DMAStreamX;
	DMA_Stream_TypeDef * SPI_RX_DMAStreamX;

	uint32_t CS_Pin;
	GPIO_TypeDef * CS_Port;

	uint32_t CLKO_Pin;
	GPIO_TypeDef * CLKO_Port;

	uint32_t INT_Pin;
	GPIO_TypeDef * INT_Port;

	uint32_t INT0_Pin;
	GPIO_TypeDef * INT0_Port;

	uint32_t INT1_Pin;
	GPIO_TypeDef * INT1_Port;

}spiCAN;

typedef union canMsg_t
{
	struct
	{
		CAN_MSGOBJ_ID id;
		CAN_TX_MSGOBJ_CTRL ctrl;
		uint8_t message[8];
	};
	uint8_t byte[16];
}canMsg;


uint32_t canfd_transmit(CAN_TxHeaderTypeDef * TxHeader, uint8_t * message, uint32_t FIFOx, spiCAN * spican);
CAN_RX_MSGOBJ canfd_receive(uint32_t FIFOx, spiCAN * spican);

void canfd_RAMInit(spiCAN * spican);
void canfd_reset(spiCAN * spican);

uint32_t canfd_checkIfFIFOisNotFull(uint32_t FIFOx, spiCAN * spican);
uint32_t canfd_checkIfFIFOisNotEmpty(uint32_t FIFOx, spiCAN * spican);
REG_CiTXREQ canfd_requestTransmission(uint32_t FIFOx, spiCAN * spican);
void canfd_incrementFIFOandRequestTransmission(uint32_t FIFOx, spiCAN * spican);
REG_CiFIFOSTA canfd_getFIFOstatus(uint32_t FIFOx, spiCAN * spican);
void canfd_resetFIFO(uint32_t FIFOx, REG_CiFIFOCON * fifocon, spiCAN * spican);
void canfd_getStatus(mcp_status * candf_status, spiCAN * spican);

uint32_t Configure_CAN();
// Write
void spican_writeByte(uint32_t address, uint8_t message, spiCAN * spican);

void spican_write8bitArray(uint32_t address, uint8_t * message, uint32_t size, spiCAN * spican);

void spican_write32bitReg(uint32_t address, uint8_t * message, spiCAN * spican);

// Read
uint8_t spican_readByte(uint32_t address, spiCAN * spican);
uint8_t spican_readByte_withDMA(uint32_t address, spiCAN * spican);

void spican_read32bitReg(uint32_t address, uint8_t * reg_buffer, spiCAN * spican);
void spican_read32bitReg_withDMA(uint32_t address, uint8_t * reg_buffer, spiCAN * spican);

void spican_readBytes(uint32_t address, uint8_t * rx_buffer, uint32_t size, spiCAN * spican);
void spican_readBytes_withDMA(uint32_t address, uint8_t * rx_buffer, uint32_t size, spiCAN * spican);



void spiCAN1_Init();
void spiCAN2_Init();
void spiCAN3_Init();
void spiCAN4_Init();
#endif /* INC_CANFD_STM_H_ */
