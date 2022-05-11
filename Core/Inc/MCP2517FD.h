#ifndef MCP2517FD_H
#define MCP2517FD_H

#include <stdint.h>

typedef struct MCP2517FD_SFR_t
{
	uint32_t OSC;		// OSCILLATOR CONTROL REGISTER
	uint32_t IOCON;		// INPUT/OUTPUT CONTROL REGISTER
	uint32_t MCP_CRC;		// CRC REGISTER
	uint32_t ECCCON;	// ECC CONTROL REGISTER
	uint32_t ECCSTAT;	// ECC STATUS REGISTER

}MCP2517FD_SFR_t;

typedef struct MCP2517FD_CAN_Controller_t
{
	uint32_t CICON;		// CAN CONTROL REGISTER
	uint32_t CINBTCFG;	// NOMINAL BIT TIME CONFIGURATION REGISTER
	uint32_t CIDBTCFG;	// DATA BIT TIME CONFIGURATION REGISTER
	uint32_t CITDC;		// TRANSMITTER DELAY COMPENSATION REGISTER
	uint32_t CITBC;		// TIME BASE COUNTER REGISTER
	uint32_t CITSCON;	// TIME STAMP CONTROL REGISTER
	uint32_t CIVEC;		// INTERRUPT CODE REGISTER
	uint32_t CIINT;		// INTERRUPT REGISTER
	uint32_t CIRXIF;	// RECEIVE INTERRUPT STATUS REGISTER
	uint32_t CITXIF;	// RECEIVE OVERFLOW INTERRUPT STATUS REGISTER
	uint32_t CIRXOVIF;	// TRANSMIT INTERRUPT STATUS REGISTER
	uint32_t CITXATIF;	// TRANSMIT ATTEMPT INTERRUPT STATUS REGISTER
	uint32_t CITXREQ;	// TRANSMIT REQUEST REGISTER
	uint32_t CITREC;	// TRANSMIT/RECEIVE ERROR COUNT REGISTER
	uint32_t CIBDIAG0;	// BUS DIAGNOSTIC REGISTER 0
	uint32_t CIBDIAG1;	// BUS DIAGNOSTICS REGISTER 1
	uint32_t CITEFCON;	// TRANSMIT EVENT FIFO CONTROL REGISTER
	uint32_t CITEFSTA;	// TRANSMIT EVENT FIFO STATUS REGISTER
	uint32_t CITEFUA;	// TRANSMIT EVENT FIFO USER ADDRESS REGISTER

	uint32_t Reserved1;	//

	uint32_t CI1TXQCON;	// TRANSMIT QUEUE CONTROL REGISTER
	uint32_t CITXQSTA;	// TRANSMIT QUEUE STATUS REGISTER
	uint32_t CITXQUA;	// TRANSMIT QUEUE USER ADDRESS REGISTER

	uint32_t Reserved2;	//
	uint32_t Reserved3;	//
	uint32_t Reserved4;	//

	uint32_t CIFIFOSTA1;// FIFO STATUS REGISTER
	uint32_t CIFIFOUA1;	// FIFO USER ADDRESS REGISTER

	uint32_t CIFLTCON0;	// FILTER CONTROL REGISTER 0
	uint32_t CIFLTCON1;	// FILTER CONTROL REGISTER 1
	uint32_t CIFLTCON2;	// FILTER CONTROL REGISTER 2
	uint32_t CIFLTCON3;	// FILTER CONTROL REGISTER 3
	uint32_t CIFLTCON4;	// FILTER CONTROL REGISTER 4
	uint32_t CIFLTCON5;	// FILTER CONTROL REGISTER 5
	uint32_t CIFLTCON6;	// FILTER CONTROL REGISTER 6
	uint32_t CIFLTCON7;	// FILTER CONTROL REGISTER 7

}MCP2517FD_CAN_Controller_t;

typedef struct MCP2517FD_FIFOs_t
{
	uint32_t CIFIFOCON;	// FIFO CONTROL REGISTER
	uint32_t CIFIOFOSTA;// FIFO STATUS REGISTER
	uint32_t CIFIFOUA;	// FIFO USER ADDRESS REGISTER
	uint32_t CIFLTOBJ;	// FILTER OBJECT REGISTER
	uint32_t CIMASK;	// MASK REGISTER
}MCP2517FD_FIFOs_t;


#define can_id 2 //䫿 CS SPI

/*** Mode Codes ***/
typedef enum modes_t
{
	normal_canfd_mode 		= 0x0,
	sleep_mode 				= 0x1,
	low_power_mode			= 0x1,
	internal_loopback_mode 	= 0x2,
	listen_only_mode 		= 0x3,
	config_mode 			= 0x4,
	ext_loopback_mode 		= 0x5,
	normal_can2_0_mode 		= 0x6,
	restricted_op_mode 		= 0x7
}modes_t;

/*** CAN message structure ***/

typedef struct message_arbitration_t
{
	unsigned ID		: 11;
	unsigned RTR	: 1;
}arbitration_t;
typedef struct message_ctrl_t
{
	unsigned IDE	: 1;
	unsigned r0		: 1;
	unsigned DLC	: 4;
}ctrl_t;
typedef struct message_crc_t
{
	unsigned CAN_CRC	: 15;
	unsigned CAN_CRC_DEL: 1;
}crc_t;

typedef struct can_message_t
{
	unsigned SOF		: 1;
	arbitration_t message_arbitration;
	ctrl_t	message_ctrl;
	uint32_t DATA[2];
	crc_t message_crc;
	unsigned ACK		: 1;
	unsigned ACK_DEL	: 1;
	unsigned EOF		: 7;
	unsigned IFS		: 3;
}can_message_t;

can_message_t tx_msg;
can_message_t rx_msg;



#endif
