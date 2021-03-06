#include "IO_Board.h"
#include <stdlib.h>
#include "modbus_ascii.h"
#include "drv_canfdspi_defines.h"
#include "drv_canfdspi_register.h"
#include "canfd_stm.h"
#include "canfd_stm_config.h"


#define NO_PACKET_RECEIVED	0x1

typedef enum processes
{
	ASK,
	RECEIVE,
	PLACE
}processes_t;

enum IOBoardCommands
{
	ASK_FOR_READINESS = 1,
	ASK_FOR_DIN,
	ASK_FOR_ADC,
	TURN_OFF_DOUT,
	TURN_OFF_YS,
	ASK_FOR_KEY_IS,
	ASK_FOR_ALL_DINS,
	ASK_FOR_PERIOD_XA
}IOBoardCommands_t;

extern CAN_TxHeaderTypeDef TxHeader;
extern CAN_RxHeaderTypeDef RxHeader;
extern uint8_t TxData[2];
extern uint8_t RxData_fifo[8];
extern uint8_t RxData_fifo1[8];
//extern modbus_ascii_table_t MODBUS_Table;
extern ModbusTable_t MODBUS_Table;
extern uint8_t IOboardBuffer[20];
extern uint32_t readiness;
extern uint32_t is_io_set;
extern uint8_t Unfiltered_CAN_msgs[8];
extern uint32_t cmd_for_IOboard;
extern uint32_t IOboard_adc_in_process;
extern uint32_t IOADCs[7];
extern uint32_t processInDrive;
extern uint8_t XA2_dir;

extern IOBoard_t io1;
extern IOBoard_t io2;
extern IOBoard_t io3;
extern IOBoard_t io4;

extern spiCAN spican1;
extern spiCAN spican2;
extern spiCAN spican3;
extern spiCAN spican4;

extern UsedFIFOs canfd1_fifos;
extern UsedFIFOs canfd3_fifos;
extern UsedFIFOs canfd4_fifos;

extern mcp_status canfd1_status;
extern mcp_status canfd2_status;
extern mcp_status canfd3_status;
extern mcp_status canfd4_status;

uint32_t receiveFromIO2(IOBoard_t * IOBoard);

void askIO(CAN_HandleTypeDef * hcan, IOboard_request_t * req);
uint32_t receiveFromIO(CAN_HandleTypeDef * hcan, uint8_t * receivedData, uint32_t boardID);
void placeIntoTableIO(uint8_t * receivedData, IOboard_request_t * req);
void IOboard_request(CAN_HandleTypeDef * hcan, uint32_t boardNumber, uint32_t cmd, IOboard_request_t * req);

void ask_for_readiness(IOBoard_t * IOBoard);
void ask_for_specific_DIN(IOBoard_t * IOBoard, uint8_t DIN);
void ask_for_specific_ADC(IOBoard_t * IOBoard, uint8_t ADCx);
void turn_on_specific_DOUT(IOBoard_t * IOBoard, uint8_t DOUT);
void turn_off_specific_DOUT(IOBoard_t * IOBoard, uint8_t DOUT);
void ask_for_specific_IS(IOBoard_t * IOBoard, uint8_t ISx);
void ask_DINs(IOBoard_t * IOBoard);
void ask_for_XA(IOBoard_t * IOBoard);
void set_address_of_IO(IOBoard_t * IOBoard, uint8_t NewAddr);
uint32_t get_response(IOBoard_t * IOBoard);
uint32_t RequestIO(IOBoard_t * IOBoard);
void place_into_table(IOBoard_t * IOBoard);


void IOboard1Init(void)
{
	io1.spican = &spican3;
	io1.used_fifos = &canfd3_fifos;
	io1.CAN_used = EXTERNAL_CAN;
	io1.External_FIFO_Channel = io1.used_fifos->ReceiveFIFOs.two.FIFOx;

	io1.BoardNr = 0;
	io1.messageID = 0xFU;
	io1.receivedID = 0xF0;
	io1.hcan = &hcan2;
	io1.lastCommand = 0;
	io1.currentCommand = 0;
	io1.nextCommand = 0;

}
void IOboard2Init(void)
{
	io2.spican = &spican3;
	io2.used_fifos = &canfd3_fifos;
	io2.CAN_used = EXTERNAL_CAN;
	io2.External_FIFO_Channel = io2.used_fifos->ReceiveFIFOs.three.FIFOx;

	io2.BoardNr = 1;
	io2.messageID = 0x10U;
	io2.receivedID = 0xF1;
	io2.hcan = &hcan2;
	io2.lastCommand = 0;
	io2.currentCommand = 0;
	io2.nextCommand = 0;

}
void IOboard3Init(void)
{
	io3.spican = &spican3;
	io3.used_fifos = &canfd3_fifos;
	io3.CAN_used = EXTERNAL_CAN;
	io3.External_FIFO_Channel = io3.used_fifos->ReceiveFIFOs.three.FIFOx;

	io3.BoardNr = 2;
	io3.messageID = 0x11U;
	io3.receivedID = 0xF2;
	io3.hcan = &hcan2;
	io3.lastCommand = 0;
	io3.currentCommand = 0;
	io3.nextCommand = 0;

}


//void askIO(CAN_HandleTypeDef * hcan, IOboard_request_t * req)
//{
//	uint8_t message_Payload[8] = {0};
//	TxHeader.StdId = 0x01;
//	TxHeader.ExtId = 0x00;
// 	TxHeader.RTR = CAN_RTR_DATA;
// 	TxHeader.IDE = CAN_ID_STD;
// 	TxHeader.DLC = 8;
// 	TxHeader.TransmitGlobalTime = DISABLE;
//	message_Payload[0] = 0x31;
//	message_Payload[1] = req->myboard_addr;
//	message_Payload[2] = req->cmd;
//	message_Payload[3] = req->element;
//	message_Payload[4] = 0x3B;
//	HAL_CAN_AddTxMessage(hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
//}
//
//uint32_t receiveFromIO(CAN_HandleTypeDef * hcan, uint8_t * receivedData, uint32_t boardID)
//{
//	// Check if something was received
//	if(hcan->Instance->RF0R & (0x3<<0))
//	{
//		if((hcan->Instance->sFIFOMailBox->RIR>>21) == (boardID))
//		{
//			HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, receivedData);
//		}
//		else
//		{
//			HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, Unfiltered_CAN_msgs);
//		}
//	}
//	else if(hcan->Instance->RF1R & (0x3<<0))
//	{
//		if((hcan->Instance->sFIFOMailBox->RIR>>21) == (boardID))
//		{
//			HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, receivedData);
//		}
//		else
//		{
//			HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, Unfiltered_CAN_msgs);
//		}
//	}
//	else
//	{
//		return NO_PACKET_RECEIVED;
//	}
//}
//
//void IOboard_request(CAN_HandleTypeDef * hcan, uint32_t boardNumber, uint32_t cmd, IOboard_request_t * req)
//{
//	uint32_t increment;
//	switch(cmd)
//	{
//		case ASK_FOR_READINESS:
//			req->cmd = cmd;
//			req->myboard_addr = boardNumber;
//			req->element = 0;	// Get the ADC channel
//			askIO(&hcan2, req);
//			break;
//		case ASK_FOR_DIN:
//
//			increment = 0;
//			break;
//		case ASK_FOR_ADC:
//			/*** When checking ADC channels increment command after CH15 **/
//			req->cmd = cmd;
//			req->myboard_addr = boardNumber;
//			req->element = IOADCs[IOboard_adc_in_process];	// Get the ADC channel
//			askIO(&hcan2, req);
//			if(IOboard_adc_in_process > 5)	// if ADC channel reached CH15 increament command and nulify ADC channel
//			{
//				IOboard_adc_in_process = 0;
//			}
//			else
//			{
//				IOboard_adc_in_process++;
//			}
//			break;
//		case TURN_OFF_DOUT:
//			break;
//		case TURN_OFF_YS:
//			break;
//		case ASK_FOR_KEY_IS:
//			break;
//		case ASK_FOR_ALL_DINS:
//			req->cmd = cmd;
//			req->myboard_addr = boardNumber;
//			req->element = 0;
//			askIO(&hcan2, req);
//			break;
//		case ASK_FOR_PERIOD_XA:
//			break;
//		default:
//			cmd_for_IOboard = 0;
//			break;
//	}
//}


uint32_t CIO(IOBoard_t * IOBoard)
{
	static uint32_t current_process = 0;
	uint32_t go_to_the_next_block = 0;
	typedef enum
	{
		NO,
		YES
	}go_next_enum;
	typedef enum IO_pocesses
	{
		REQUEST,
		GET_RESPONSE,
		PLACE_INTO_TABLE
	}IO_pocesses_enum;
	switch(current_process)
	{
		case REQUEST:
			go_to_the_next_block = RequestIO(IOBoard);
			current_process = GET_RESPONSE;
			break;
		case GET_RESPONSE:
			 get_response(IOBoard);
			current_process = PLACE_INTO_TABLE;
			break;
		case PLACE_INTO_TABLE:
			place_into_table(IOBoard);
			current_process = REQUEST;
			break;
	}
	return go_to_the_next_block;
}

uint32_t get_response(IOBoard_t * IOBoard)
{
	if(IOBoard->CAN_used == INTERNAL_CAN)
	{
		// Check if something was received
		if(IOBoard->hcan->Instance->RF0R & (0x3<<0))
		{
			if((IOBoard->hcan->Instance->sFIFOMailBox->RIR>>21) == (IOBoard->receivedID))
			{
				HAL_CAN_GetRxMessage(IOBoard->hcan, CAN_RX_FIFO0, &RxHeader, IOBoard->RxBuffer);
			}
			else
			{
				HAL_CAN_GetRxMessage(IOBoard->hcan, CAN_RX_FIFO0, &RxHeader, Unfiltered_CAN_msgs);
			}
		}
		else if(IOBoard->hcan->Instance->RF1R & (0x3<<0))
		{
			if((IOBoard->hcan->Instance->sFIFOMailBox->RIR>>21) == (IOBoard->receivedID))
			{
				HAL_CAN_GetRxMessage(IOBoard->hcan, CAN_RX_FIFO1, &RxHeader, IOBoard->RxBuffer);
			}
			else
			{
				HAL_CAN_GetRxMessage(IOBoard->hcan, CAN_RX_FIFO1, &RxHeader, Unfiltered_CAN_msgs);
			}
		}
		else
		{
			return NO_PACKET_RECEIVED;
		}
	}
	else if(IOBoard->CAN_used == EXTERNAL_CAN)
	{
		CAN_RX_MSGOBJ received_message = {0};
		received_message = canfd_receive(IOBoard->used_fifos->ReceiveFIFOs.two.FIFOx, IOBoard->spican);
		for(int i = 0; i < 8; i++)
		{
			IOBoard->RxBuffer[i] = received_message.bF.message[i];
		}
	}

	return 0;
}

void ask_for_readiness(IOBoard_t * IOBoard)						/*** 1 ***/
{
	uint8_t message_Payload[8] = {0};

	IOBoard->requestedData.cmd = 0x1;
	IOBoard->requestedData.element = 0;

	TxHeader.StdId = IOBoard->messageID;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	message_Payload[0] = 0x31;
	message_Payload[1] = IOBoard->BoardNr;
	message_Payload[2] = IOBoard->requestedData.cmd;
	message_Payload[3] = IOBoard->requestedData.element;
	message_Payload[4] = 0x3B;
	if(IOBoard->CAN_used == INTERNAL_CAN)
	{
		HAL_CAN_AddTxMessage(IOBoard->hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
	}
	else if(IOBoard->CAN_used == EXTERNAL_CAN)
	{
		canfd_transmit(&TxHeader, message_Payload, IOBoard->used_fifos->TransmitFIFOs.one.FIFOx, IOBoard->spican);

	}
}
void ask_for_specific_DIN(IOBoard_t * IOBoard, uint8_t DIN)		/*** 2 ***/
{
	uint8_t message_Payload[8] = {0};

	IOBoard->requestedData.cmd = 0x2;
	IOBoard->requestedData.element = DIN;

	TxHeader.StdId = IOBoard->messageID;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	message_Payload[0] = 0x31;
	message_Payload[1] = IOBoard->BoardNr;
	message_Payload[2] = IOBoard->requestedData.cmd;
	message_Payload[3] = IOBoard->requestedData.element;
	message_Payload[4] = 0x3B;

	if(IOBoard->CAN_used == INTERNAL_CAN)
	{
		HAL_CAN_AddTxMessage(IOBoard->hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
	}
	else if(IOBoard->CAN_used == EXTERNAL_CAN)
	{
		canfd_transmit(&TxHeader, message_Payload, IOBoard->used_fifos->TransmitFIFOs.one.FIFOx, IOBoard->spican);

	}
}
void ask_for_specific_ADC(IOBoard_t * IOBoard, uint8_t ADCx)	/*** 3 ***/
{
	uint8_t message_Payload[8] = {0};

	IOBoard->requestedData.cmd = 0x3;
	IOBoard->requestedData.element = ADCx;

	TxHeader.StdId = IOBoard->messageID;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	message_Payload[0] = 0x31;
	message_Payload[1] = IOBoard->BoardNr;
	message_Payload[2] = IOBoard->requestedData.cmd;
	message_Payload[3] = IOBoard->requestedData.element;
	message_Payload[4] = 0x3B;

	if(IOBoard->CAN_used == INTERNAL_CAN)
	{
		HAL_CAN_AddTxMessage(IOBoard->hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
	}
	else if(IOBoard->CAN_used == EXTERNAL_CAN)
	{
		canfd_transmit(&TxHeader, message_Payload, IOBoard->used_fifos->TransmitFIFOs.one.FIFOx, IOBoard->spican);

	}
}
void turn_on_specific_DOUT(IOBoard_t * IOBoard, uint8_t DOUT)	/*** 4 ***/
{
	uint8_t message_Payload[8] = {0};

	IOBoard->requestedData.cmd = 0x4;
	IOBoard->requestedData.element = DOUT;

	TxHeader.StdId = IOBoard->messageID;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	message_Payload[0] = 0x31;
	message_Payload[1] = IOBoard->BoardNr;
	message_Payload[2] = IOBoard->requestedData.cmd;
	message_Payload[3] = IOBoard->requestedData.element;
	message_Payload[4] = 0x3B;

	if(IOBoard->CAN_used == INTERNAL_CAN)
	{
		HAL_CAN_AddTxMessage(IOBoard->hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
	}
	else if(IOBoard->CAN_used == EXTERNAL_CAN)
	{
		canfd_transmit(&TxHeader, message_Payload, IOBoard->used_fifos->TransmitFIFOs.one.FIFOx, IOBoard->spican);

	}
}
void turn_off_specific_DOUT(IOBoard_t * IOBoard, uint8_t DOUT)	/*** 5 ***/
{
	uint8_t message_Payload[8] = {0};

	IOBoard->requestedData.cmd = 0x5;
	IOBoard->requestedData.element = DOUT;

	TxHeader.StdId = IOBoard->messageID;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	message_Payload[0] = 0x31;
	message_Payload[1] = IOBoard->BoardNr;
	message_Payload[2] = IOBoard->requestedData.cmd;
	message_Payload[3] = IOBoard->requestedData.element;
	message_Payload[4] = 0x3B;

	if(IOBoard->CAN_used == INTERNAL_CAN)
	{
		HAL_CAN_AddTxMessage(IOBoard->hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
	}
	else if(IOBoard->CAN_used == EXTERNAL_CAN)
	{
		canfd_transmit(&TxHeader, message_Payload, IOBoard->used_fifos->TransmitFIFOs.one.FIFOx, IOBoard->spican);

	}
}
void ask_for_specific_IS(IOBoard_t * IOBoard, uint8_t ISx)		/*** 6 ***/
{
	uint8_t message_Payload[8] = {0};

	IOBoard->requestedData.cmd = 0x6;
	IOBoard->requestedData.element = ISx;

	TxHeader.StdId = IOBoard->messageID;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	message_Payload[0] = 0x31;
	message_Payload[1] = IOBoard->BoardNr;
	message_Payload[2] = IOBoard->requestedData.cmd;
	message_Payload[3] = IOBoard->requestedData.element;
	message_Payload[4] = 0x3B;

	if(IOBoard->CAN_used == INTERNAL_CAN)
	{
		HAL_CAN_AddTxMessage(IOBoard->hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
	}
	else if(IOBoard->CAN_used == EXTERNAL_CAN)
	{
		canfd_transmit(&TxHeader, message_Payload, IOBoard->used_fifos->TransmitFIFOs.one.FIFOx, IOBoard->spican);

	}
}
void ask_DINs(IOBoard_t * IOBoard)								/*** 7 ***/
{
	uint8_t message_Payload[8] = {0};

	IOBoard->requestedData.cmd = 0x7;
	IOBoard->requestedData.element = 0;

	TxHeader.StdId = IOBoard->messageID;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	message_Payload[0] = 0x31;
	message_Payload[1] = IOBoard->BoardNr;
	message_Payload[2] = IOBoard->requestedData.cmd;
	message_Payload[3] = IOBoard->requestedData.element;
	message_Payload[4] = 0x3B;

	if(IOBoard->CAN_used == INTERNAL_CAN)
	{
		HAL_CAN_AddTxMessage(IOBoard->hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
	}
	else if(IOBoard->CAN_used == EXTERNAL_CAN)
	{
		canfd_transmit(&TxHeader, message_Payload, IOBoard->used_fifos->TransmitFIFOs.one.FIFOx, IOBoard->spican);

	}
}
void ask_for_XA(IOBoard_t * IOBoard)							/*** 8 ***/
{
	uint8_t message_Payload[8] = {0};

	IOBoard->requestedData.cmd = 0x8;

	TxHeader.StdId = IOBoard->messageID;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	message_Payload[0] = 0x31;
	message_Payload[1] = IOBoard->BoardNr;
	message_Payload[2] = IOBoard->requestedData.cmd;
	message_Payload[3] = 0;
	message_Payload[4] = 0x3B;

	if(IOBoard->CAN_used == INTERNAL_CAN)
	{
		HAL_CAN_AddTxMessage(IOBoard->hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
	}
	else if(IOBoard->CAN_used == EXTERNAL_CAN)
	{
		canfd_transmit(&TxHeader, message_Payload, IOBoard->used_fifos->TransmitFIFOs.one.FIFOx, IOBoard->spican);

	}
}
void set_address_of_IO(IOBoard_t * IOBoard, uint8_t NewAddr)	/*** 9 ***/
{
	uint8_t message_Payload[8] = {0};

	IOBoard->requestedData.cmd = 0x9;
	IOBoard->requestedData.element = NewAddr;

	TxHeader.StdId = IOBoard->messageID;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	message_Payload[0] = 0x31;
	message_Payload[1] = IOBoard->BoardNr;
	message_Payload[2] = IOBoard->requestedData.cmd;
	message_Payload[3] = IOBoard->requestedData.element;
	message_Payload[4] = 0x3B;

	if(IOBoard->CAN_used == INTERNAL_CAN)
	{
		HAL_CAN_AddTxMessage(IOBoard->hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
	}
	else if(IOBoard->CAN_used == EXTERNAL_CAN)
	{
		canfd_transmit(&TxHeader, message_Payload, IOBoard->used_fifos->TransmitFIFOs.one.FIFOx, IOBoard->spican);

	}
}

uint32_t ask_ADCs(IOBoard_t * IOBoard)
{
	static uint8_t ADCx = 0;
	uint32_t go_next = 0;
	ask_for_specific_ADC(IOBoard, IOADCs[ADCx]);
	ADCx++;
	if(ADCx >= (sizeof(IOADCs)>>2))	//sizeof(IOADCs)/4 = (sizeof(IOADCs)>>2)
	{
		ADCx = 0;
		go_next = 1;
	}
	return go_next;
}
uint32_t ask_ISs(IOBoard_t * IOBoard)
{
	static uint8_t ISx = 1;
	uint32_t go_next = 0;
	ask_for_specific_IS(IOBoard, ISx);
	ISx++;
	if(ISx > 10)
	{
		ISx = 1;
		go_next = 1;
	}
	return go_next;
}

uint32_t RequestIO(IOBoard_t * IOBoard)
{
	static uint32_t next_request = 0;
	static uint32_t go_to_the_next_board = 0;
	uint32_t go_next = 0;
	typedef enum requests
	{
		DINs,
		ISs,
		ADCs,
		XAs
	}requests_enum;
	typedef enum
	{
		NO,
		YES
	}go_next_enum;

	switch(next_request)
	{
		case DINs:
			ask_DINs(IOBoard);
			next_request = ISs;
			go_to_the_next_board = NO;
			break;
		case ISs:
			go_next = ask_ISs(IOBoard);
			if(go_next == YES)
			{
				next_request = ADCs;
				go_to_the_next_board = NO;
			}
			break;
		case ADCs:
			go_next = ask_ADCs(IOBoard);
			if(go_next == YES)
			{
				next_request = XAs;
				go_to_the_next_board = NO;
			}
			break;
		case XAs:
			ask_for_XA(IOBoard);
			next_request = DINs;
			go_to_the_next_board = YES;
			break;
		default:
			break;
	}
	return go_to_the_next_board;
}

void place_DINs(IOBoard_t * IOBoard)
{
	uint32_t board = IOBoard->RxBuffer[1];
	enum
	{
		BOARD1,
		BOARD2,
		BOARD3
	};
	switch(board)
	{
		case BOARD1:

			MODBUS_Table.bit_table.iobt1_struct.DIN.signal_for_close_lights 			= (IOBoard->RxBuffer[4] & 0x80)>>7;	//X1
			MODBUS_Table.bit_table.iobt1_struct.DIN.signal_for_far_lights 				= (IOBoard->RxBuffer[4] & 0x40)>>6;	//X2
			MODBUS_Table.bit_table.iobt1_struct.DIN.signal_for_PT_lights 				= (IOBoard->RxBuffer[3] & 0x80)>>7;	//X3
			MODBUS_Table.bit_table.iobt1_struct.DIN.signal_for_half_heating 			= (IOBoard->RxBuffer[3] & 0x40)>>6;	//X4
			MODBUS_Table.bit_table.iobt1_struct.DIN.signal_for_full_heating 			= (IOBoard->RxBuffer[3] & 0x20)>>5;	//X5
			MODBUS_Table.bit_table.iobt1_struct.DIN.signal_for_left_indication 			= (IOBoard->RxBuffer[3] & 0x10)>>4;	//X6
			MODBUS_Table.bit_table.iobt1_struct.DIN.signal_for_right_indication 		= (IOBoard->RxBuffer[3] & 0x08)>>3;	//X7
			MODBUS_Table.bit_table.iobt1_struct.DIN.signal_for_work_with_PZ 			= (IOBoard->RxBuffer[3] & 0x04)>>2;	//X8
			MODBUS_Table.bit_table.iobt1_struct.DIN.signal_for_air_conditioning_for_KB 	= (IOBoard->RxBuffer[3] & 0x02)>>1;	//X9
			MODBUS_Table.bit_table.iobt1_struct.DIN.signal_for_air_conditioning_for_KC 	= (IOBoard->RxBuffer[3] & 0x01)>>0;	//X10

			MODBUS_Table.bit_table.iobt1_struct.DOUT.indication_close_lights 			= (IOBoard->RxBuffer[5] & 0x80)>>7;	//KS1
			MODBUS_Table.bit_table.iobt1_struct.DOUT.indication_far_lights 				= (IOBoard->RxBuffer[5] & 0x40)>>6;	//KS2
			MODBUS_Table.bit_table.iobt1_struct.DOUT.indication_air_conditioning_for_KB = (IOBoard->RxBuffer[5] & 0x20)>>5;	//KS3
			MODBUS_Table.bit_table.iobt1_struct.DOUT.indication_half_heating 			= (IOBoard->RxBuffer[5] & 0x10)>>4;	//KS4
			MODBUS_Table.bit_table.iobt1_struct.DOUT.indication_full_heating 			= (IOBoard->RxBuffer[5] & 0x08)>>3;	//KS5
			MODBUS_Table.bit_table.iobt1_struct.DOUT.indication_air_conditioning_for_KC = (IOBoard->RxBuffer[5] & 0x04)>>2;	//KS6
			MODBUS_Table.bit_table.iobt1_struct.DOUT.indication_left 					= (IOBoard->RxBuffer[5] & 0x02)>>1;	//KS7
			MODBUS_Table.bit_table.iobt1_struct.DOUT.indication_right 					= (IOBoard->RxBuffer[5] & 0x01)>>0;	//KS8
			MODBUS_Table.bit_table.iobt1_struct.DOUT.indication_cabin_fan 				= (IOBoard->RxBuffer[6] & 0x80)>>7;	//KS9
			MODBUS_Table.bit_table.iobt1_struct.DOUT.indication_warning_doors 			= (IOBoard->RxBuffer[6] & 0x40)>>6;	//KS10
			break;
		case BOARD2:

			MODBUS_Table.bit_table.iobt2_struct.DIN.signal_for_driver_passanger_out1 	= (IOBoard->RxBuffer[4] & 0x80)>>7;	//X1
			MODBUS_Table.bit_table.iobt2_struct.DIN.signal_for_driver_passanger_out2 	= (IOBoard->RxBuffer[4] & 0x40)>>6;	//X2
			MODBUS_Table.bit_table.iobt2_struct.DIN.signal_for_driver_passanger_out3 	= (IOBoard->RxBuffer[3] & 0x80)>>7;	//X3
			MODBUS_Table.bit_table.iobt2_struct.DIN.signal_for_apparel 					= (IOBoard->RxBuffer[3] & 0x40)>>6;	//X4
			MODBUS_Table.bit_table.iobt2_struct.DIN.signal_for_invalid_near_door 		= (IOBoard->RxBuffer[3] & 0x20)>>5;	//X5
			MODBUS_Table.bit_table.iobt2_struct.DIN.signal_for_invalid_cabin			= (IOBoard->RxBuffer[3] & 0x10)>>4;	//X6
			MODBUS_Table.bit_table.iobt2_struct.DIN.signal_for_door1_opened 			= (IOBoard->RxBuffer[3] & 0x08)>>3;	//X7
			MODBUS_Table.bit_table.iobt2_struct.DIN.signal_for_door2_opened				= (IOBoard->RxBuffer[3] & 0x04)>>2;	//X8
			MODBUS_Table.bit_table.iobt2_struct.DIN.signal_for_door3_opened 			= (IOBoard->RxBuffer[3] & 0x02)>>1;	//X9
			MODBUS_Table.bit_table.iobt2_struct.DIN.signal_for_hv_circuit_on 			= (IOBoard->RxBuffer[3] & 0x01)>>0;	//X10

			MODBUS_Table.bit_table.iobt2_struct.DIN.signal_for_washing 					= (IOBoard->RxBuffer[5] & 0x80)>>7;	//KS1
			MODBUS_Table.bit_table.iobt2_struct.DIN.signal_for_arrow_under_current 		= (IOBoard->RxBuffer[5] & 0x40)>>6;	//KS2
			MODBUS_Table.bit_table.iobt2_struct.DIN.signal_for_arrow_without_current 	= (IOBoard->RxBuffer[5] & 0x20)>>5;	//KS3
			MODBUS_Table.bit_table.iobt2_struct.DIN.signal_for_master_cabin 			= (IOBoard->RxBuffer[5] & 0x10)>>4;	//KS4
			MODBUS_Table.bit_table.iobt2_struct.DIN.signal_for_slave_cabin 				= (IOBoard->RxBuffer[5] & 0x08)>>3;	//KS5
			MODBUS_Table.bit_table.iobt2_struct.DIN.signal_for_tram_disconect_1 		= (IOBoard->RxBuffer[5] & 0x04)>>2;	//KS6
			MODBUS_Table.bit_table.iobt2_struct.DIN.signal_for_static_KKB_power_on 		= (IOBoard->RxBuffer[5] & 0x02)>>1;	//KS7
			MODBUS_Table.bit_table.iobt2_struct.DIN.signal_for_static_KC_power_on 		= (IOBoard->RxBuffer[5] & 0x01)>>0;	//KS8
			MODBUS_Table.bit_table.iobt2_struct.DIN.signal_for_tram_disconect_2 		= (IOBoard->RxBuffer[6] & 0x80)>>7;	//KS9
			MODBUS_Table.bit_table.iobt2_struct.DOUT.indication_control_on				= (IOBoard->RxBuffer[6] & 0x20)>>5;	//KS11
			break;
		case BOARD3:

			MODBUS_Table.bit_table.iobt3_struct.DIN.signal_for_dimension_lights 		= (IOBoard->RxBuffer[4] & 0x80)>>7;	//X1
			MODBUS_Table.bit_table.iobt3_struct.DIN.signal_for_turn_on_half_lighting 	= (IOBoard->RxBuffer[4] & 0x40)>>6;	//X2
			MODBUS_Table.bit_table.iobt3_struct.DIN.signal_for_turn_on_full_lighting 	= (IOBoard->RxBuffer[3] & 0x80)>>7;	//X3
			MODBUS_Table.bit_table.iobt3_struct.DIN.signal_for_turn_on_control 			= (IOBoard->RxBuffer[3] & 0x40)>>6;	//X4
			MODBUS_Table.bit_table.iobt3_struct.DIN.signal_for_unauthorized_PZ_access 	= (IOBoard->RxBuffer[3] & 0x20)>>5;	//X5
			MODBUS_Table.bit_table.iobt3_struct.DIN.signal_for_fire						= (IOBoard->RxBuffer[3] & 0x10)>>4;	//X6
			MODBUS_Table.bit_table.iobt3_struct.DIN.signal_for_LS_state					= (IOBoard->RxBuffer[3] & 0x08)>>3;	//X7

			MODBUS_Table.bit_table.iobt3_struct.DOUT.indication_dimension_lights 		= (IOBoard->RxBuffer[5] & 0x80)>>7;	//KS1
			MODBUS_Table.bit_table.iobt3_struct.DOUT.indication_half_lighting 			= (IOBoard->RxBuffer[5] & 0x40)>>6;	//KS2
			MODBUS_Table.bit_table.iobt3_struct.DOUT.indication_full_lighting 			= (IOBoard->RxBuffer[5] & 0x20)>>5;	//KS3
			MODBUS_Table.bit_table.iobt3_struct.DOUT.indication_emergency_lighting 		= (IOBoard->RxBuffer[5] & 0x10)>>4;	//KS4
			MODBUS_Table.bit_table.iobt3_struct.DOUT.indication_control_startup 		= (IOBoard->RxBuffer[5] & 0x08)>>3;	//KS5
			MODBUS_Table.bit_table.iobt3_struct.DOUT.indication_fire 					= (IOBoard->RxBuffer[5] & 0x04)>>2;	//KS6
			MODBUS_Table.bit_table.iobt3_struct.DOUT.indication_control_working 		= (IOBoard->RxBuffer[5] & 0x02)>>1;	//KS7
			MODBUS_Table.bit_table.iobt3_struct.DOUT.indication_AT_cabin12_power_on 	= (IOBoard->RxBuffer[5] & 0x01)>>0;	//KS8
			MODBUS_Table.bit_table.iobt3_struct.DOUT.indication_E1_cabin12_power_on 	= (IOBoard->RxBuffer[6] & 0x80)>>7;	//KS9
			MODBUS_Table.bit_table.iobt3_struct.DOUT.indication_PZ_power_up 			= (IOBoard->RxBuffer[6] & 0x40)>>6;	//KS10
			break;
		defalut:
			break;
	}
}
void place_ISs(IOBoard_t * IOBoard)
{
	return;
}
void place_ADCs(IOBoard_t * IOBoard)
{
	uint32_t ADC_ch = IOBoard->RxBuffer[3];
	uint32_t board 	= IOBoard->RxBuffer[1];
	enum
	{
		BOARD1,
		BOARD2,
		BOARD3
	};
	switch(board)
	{
		case BOARD1:
			switch(ADC_ch)
			{
//				case CH2:
//					MODBUS_Table.modbus_table[0x1C] = IOBoard->RxBuffer[6];
//					MODBUS_Table.modbus_table[0x1D] = IOBoard->RxBuffer[5];
//					break;
//				case CH3:
//					MODBUS_Table.modbus_table[0x1E] = IOBoard->RxBuffer[6];
//					MODBUS_Table.modbus_table[0x1F] = IOBoard->RxBuffer[5];
//					break;
				case CH4:
					MODBUS_Table.bit_table.iobt1_struct.analog.tachogenerator_phase1[1] = IOBoard->RxBuffer[6];
					MODBUS_Table.bit_table.iobt1_struct.analog.tachogenerator_phase1[0] = IOBoard->RxBuffer[5];
					break;
				case CH5:
					MODBUS_Table.bit_table.iobt1_struct.analog.tachogenerator_phase2[1] = IOBoard->RxBuffer[6];
					MODBUS_Table.bit_table.iobt1_struct.analog.tachogenerator_phase2[0] = IOBoard->RxBuffer[5];
					break;
//				case CH7:
//					MODBUS_Table.modbus_table[0x24] = IOBoard->RxBuffer[6];
//					MODBUS_Table.modbus_table[0x25] = IOBoard->RxBuffer[5];
//					break;
//				case CH14:
//					MODBUS_Table.modbus_table[0x26] = IOBoard->RxBuffer[6];
//					MODBUS_Table.modbus_table[0x27] = IOBoard->RxBuffer[5];
//					break;
//				case CH15:
//					MODBUS_Table.modbus_table[0x28] = IOBoard->RxBuffer[6];
//					MODBUS_Table.modbus_table[0x29] = IOBoard->RxBuffer[5];
//					break;
				default:
					break;
			}
			break;
		case BOARD2:
			switch(ADC_ch)
			{
//				case CH2:
//					MODBUS_Table.modbus_table[0x1C] = IOBoard->RxBuffer[6];
//					MODBUS_Table.modbus_table[0x1D] = IOBoard->RxBuffer[5];
//					break;
//				case CH3:
//					MODBUS_Table.modbus_table[0x1E] = IOBoard->RxBuffer[6];
//					MODBUS_Table.modbus_table[0x1F] = IOBoard->RxBuffer[5];
//					break;
//				case CH4:
//					New_MODBUS_Table.bit_table.iobt1_struct.analog.tachogenerator_phase1 = IOBoard->RxBuffer[6];
//					New_MODBUS_Table.bit_table.iobt1_struct.analog.tachogenerator_phase1 = IOBoard->RxBuffer[5];
//					break;
				case CH5:
					MODBUS_Table.bit_table.iobt2_struct.analog.battery_voltage[1] = IOBoard->RxBuffer[6];
					MODBUS_Table.bit_table.iobt2_struct.analog.battery_voltage[0] = IOBoard->RxBuffer[5];
					break;
//				case CH7:
//					MODBUS_Table.modbus_table[0x24] = IOBoard->RxBuffer[6];
//					MODBUS_Table.modbus_table[0x25] = IOBoard->RxBuffer[5];
//					break;
//				case CH14:
//					MODBUS_Table.modbus_table[0x26] = IOBoard->RxBuffer[6];
//					MODBUS_Table.modbus_table[0x27] = IOBoard->RxBuffer[5];
//					break;
//				case CH15:
//					MODBUS_Table.modbus_table[0x28] = IOBoard->RxBuffer[6];
//					MODBUS_Table.modbus_table[0x29] = IOBoard->RxBuffer[5];
//					break;
				default:
					break;
			}
			break;
		case BOARD3:

			break;
		defalut:
			break;
	}

}
void place_XAs(IOBoard_t * IOBoard)
{
	uint32_t XAx = IOBoard->RxBuffer[3];
	uint32_t board 	= IOBoard->RxBuffer[1];
	enum
	{
		BOARD1,
		BOARD2,
		BOARD3
	};
	switch(board)
	{
		case BOARD1:
			XA2_dir = IOBoard->RxBuffer[3];
			MODBUS_Table.bit_table.iobt1_struct.analog.tachogenerator_phase1[1] = IOBoard->RxBuffer[5];
			MODBUS_Table.bit_table.iobt1_struct.analog.tachogenerator_phase1[0] = IOBoard->RxBuffer[4];
			break;
		case BOARD2:

			break;
		case BOARD3:

			break;
		defalut:
			break;
	}
}

void place_into_table(IOBoard_t * IOBoard)
{
	typedef enum requests
		{
			DINs = 	7,
			ISs = 	6,
			ADCs = 	3,
			XAs = 	8
		}requests_enum;
	uint32_t cmd = IOBoard->RxBuffer[2];
	switch(cmd)
	{
		case DINs:
			place_DINs(IOBoard);
			break;
		case ISs:
			place_ISs(IOBoard);
			break;
		case ADCs:
			place_ADCs(IOBoard);
			break;
		case XAs:
			place_XAs(IOBoard);
			break;
	}
}
