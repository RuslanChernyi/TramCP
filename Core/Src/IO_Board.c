#include "IO_Board.h"
#include <stdlib.h>
#include "modbus_ascii.h"

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
extern modbus_ascii_table_t MODBUS_Table;
extern uint8_t IOboardBuffer[20];
extern uint32_t readiness;
extern uint32_t is_io_set;
extern uint8_t Unfiltered_CAN_msgs[8];
extern uint32_t cmd_for_IOboard;
extern uint32_t IOboard_adc_in_process;
extern uint32_t IOADCs[7];
extern uint32_t processInDrive;

extern IOBoard_t io1;
extern IOBoard_t io2;
extern IOBoard_t io3;
extern IOBoard_t io4;

uint32_t receiveFromIO2(IOBoard_t * IOBoard);


void IOboard1Init(void)
{
	io1.BoardNr = 0x1;
	io1.hcan = &hcan2;
	io1.currentCommand = ASK_FOR_READINESS;
	io1.lastCommand = 0;
}

void askIO(CAN_HandleTypeDef * hcan, IOboard_request_t * req)
{
	uint8_t message_Payload[8] = {0};
	TxHeader.StdId = 0x01;
	TxHeader.ExtId = 0x00;
 	TxHeader.RTR = CAN_RTR_DATA;
 	TxHeader.IDE = CAN_ID_STD;
 	TxHeader.DLC = 8;
 	TxHeader.TransmitGlobalTime = DISABLE;
	message_Payload[0] = 0x31;
	message_Payload[1] = req->myboard_addr;
	message_Payload[2] = req->cmd;
	message_Payload[3] = req->element;
	message_Payload[4] = 0x3B;

	HAL_CAN_AddTxMessage(hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
}

uint32_t receiveFromIO(CAN_HandleTypeDef * hcan, uint8_t * receivedData, uint32_t boardID)
{
	// Check if something was received
	if(hcan->Instance->RF0R & (0x3<<0))
	{
		if((hcan->Instance->sFIFOMailBox->RIR>>21) == (boardID))
		{
			HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, receivedData);
		}
		else
		{
			HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, Unfiltered_CAN_msgs);
		}
	}
	else if(hcan->Instance->RF1R & (0x3<<0))
	{
		if((hcan->Instance->sFIFOMailBox->RIR>>21) == (boardID))
		{
			HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, receivedData);
		}
		else
		{
			HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, Unfiltered_CAN_msgs);
		}
	}
	else
	{
		return NO_PACKET_RECEIVED;
	}
}

void placeIntoTableIO(uint8_t * receivedData, IOboard_request_t * req)
{
	if(RxHeader.StdId == 119)
	{
		switch(receivedData[2])
		{
			case 1:
				readiness = receivedData[3];
				break;
			case 2:
				switch(req->element)
				{
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
						MODBUS_Table.modbus_table[0x19] |= receivedData[4]<<8-req->element;
						break;
					case 6:
					case 7:
					case 8:
					case 9:
					case 10:
					case 11:
					case 12:
					case 13:
						MODBUS_Table.modbus_table[0x1A] |= receivedData[4]<<13-req->element;
						break;
					case 14:
					case 15:
					case 16:
					case 17:
						MODBUS_Table.modbus_table[0x1B] |= receivedData[4]<<21-req->element;
					default:
						break;
				}
			case 3:
				switch(req->element)
				{
					case CH2:
						MODBUS_Table.modbus_table[0x1C] = receivedData[6];
						MODBUS_Table.modbus_table[0x1D] = receivedData[5];
						break;
					case CH3:
						MODBUS_Table.modbus_table[0x1E] = receivedData[6];
						MODBUS_Table.modbus_table[0x1F] = receivedData[5];
						break;
					case CH4:
						MODBUS_Table.modbus_table[0x20] = receivedData[6];
						MODBUS_Table.modbus_table[0x21] = receivedData[5];
						break;
					case CH5:
						MODBUS_Table.modbus_table[0x22] = receivedData[6];
						MODBUS_Table.modbus_table[0x23] = receivedData[5];
						break;
					case CH7:
						MODBUS_Table.modbus_table[0x24] = receivedData[6];
						MODBUS_Table.modbus_table[0x25] = receivedData[5];
						break;
					case CH14:
						MODBUS_Table.modbus_table[0x26] = receivedData[6];
						MODBUS_Table.modbus_table[0x27] = receivedData[5];
						break;
					case CH15:
						MODBUS_Table.modbus_table[0x28] = receivedData[6];
						MODBUS_Table.modbus_table[0x29] = receivedData[5];
						break;
					default:
						break;
				}
			case 4:
				is_io_set = receivedData[4];
				break;
			case 5:
				is_io_set = receivedData[3];
				break;
			case 6:/*** !!!!!!!!!!!!!!!!!!!! ***/
				break;
			case 7:
				MODBUS_Table.modbus_table[0x19] = receivedData[3];
				MODBUS_Table.modbus_table[0x1A] = receivedData[4];
				MODBUS_Table.modbus_table[0x1B] = receivedData[5];
			case 8:
				switch(receivedData[3])
				{
					case XA1:
						MODBUS_Table.modbus_table[0x2A] = receivedData[5];
						MODBUS_Table.modbus_table[0x2B] = receivedData[4];
						break;
					case XA2:
						MODBUS_Table.modbus_table[0x2C] = receivedData[5];
						MODBUS_Table.modbus_table[0x2D] = receivedData[4];
						break;
				}
				break;
		}
	}
}

void IOboard_request(CAN_HandleTypeDef * hcan, uint32_t boardNumber, uint32_t cmd, IOboard_request_t * req)
{
	uint32_t increment;
	switch(cmd)
	{
		case ASK_FOR_READINESS:
			req->cmd = cmd;
			req->myboard_addr = boardNumber;
			req->element = 0;	// Get the ADC channel
			askIO(&hcan2, req);
			break;
		case ASK_FOR_DIN:

			increment = 0;
			break;
		case ASK_FOR_ADC:
			/*** When checking ADC channels increment command after CH15 **/
			req->cmd = cmd;
			req->myboard_addr = boardNumber;
			req->element = IOADCs[IOboard_adc_in_process];	// Get the ADC channel
			askIO(&hcan2, req);
			if(IOboard_adc_in_process > 5)	// if ADC channel reached CH15 increament command and nulify ADC channel
			{
				IOboard_adc_in_process = 0;
			}
			else
			{
				IOboard_adc_in_process++;
			}
			break;
		case TURN_OFF_DOUT:
			break;
		case TURN_OFF_YS:
			break;
		case ASK_FOR_KEY_IS:
			break;
		case ASK_FOR_ALL_DINS:
			req->cmd = cmd;
			req->myboard_addr = boardNumber;
			req->element = 0;
			askIO(&hcan2, req);
			break;
		case ASK_FOR_PERIOD_XA:
			break;
		default:
			cmd_for_IOboard = 0;
			break;
	}
}


uint32_t CIO(IOBoard_t * IOBoard)
{
	static uint32_t current_process = 0;
//	static IOboard_request_t request_info = {0};
//	static uint8_t receivedData[8] = {0};

	typedef enum IO_pocesses
	{
		REQUEST,
		GET_RESPONSE,
		PLACE_INTO_TABLE
	}IO_pocesses_enum;
	switch(current_process)
	{
		case REQUEST:

			current_process = GET_RESPONSE;
			break;
		case GET_RESPONSE:
			receiveFromIO2(IOBoard);
			current_process = PLACE_INTO_TABLE;
			break;
		case PLACE_INTO_TABLE:
			placeIntoTableIO(IOBoard->RxBuffer, &(IOBoard->requestedData));
			current_process = REQUEST;
			break;
	}
}

//void IOboard_request2(IOBoard_t * IOBoard)
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


uint32_t receiveFromIO2(IOBoard_t * IOBoard)
{
	// Check if something was received
	if(IOBoard->hcan->Instance->RF0R & (0x3<<0))
	{
		if((IOBoard->hcan->Instance->sFIFOMailBox->RIR>>21) == (IOBoard->BoardNr))
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
		if((IOBoard->hcan->Instance->sFIFOMailBox->RIR>>21) == (IOBoard->BoardNr))
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

void ask_for_readiness(IOBoard_t * IOBoard)						/*** 1 ***/
{
	uint8_t message_Payload[8] = {0};

	IOBoard->requestedData.cmd = 0x1;
	IOBoard->requestedData.element = 0;

	TxHeader.StdId = 0x01;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	message_Payload[0] = 0x31;
	message_Payload[1] = IOBoard->requestedData.myboard_addr;
	message_Payload[2] = IOBoard->requestedData.cmd;
	message_Payload[3] = IOBoard->requestedData.element;
	message_Payload[4] = 0x3B;

	HAL_CAN_AddTxMessage(IOBoard->hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
}
void ask_for_specific_DIN(IOBoard_t * IOBoard, uint8_t DIN)		/*** 2 ***/
{
	uint8_t message_Payload[8] = {0};

	IOBoard->requestedData.cmd = 0x2;
	IOBoard->requestedData.element = DIN;

	TxHeader.StdId = 0x01;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	message_Payload[0] = 0x31;
	message_Payload[1] = IOBoard->requestedData.myboard_addr;
	message_Payload[2] = IOBoard->requestedData.cmd;
	message_Payload[3] = IOBoard->requestedData.element;
	message_Payload[4] = 0x3B;

	HAL_CAN_AddTxMessage(IOBoard->hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
}
void ask_for_specific_ADC(IOBoard_t * IOBoard, uint8_t ADCx)	/*** 3 ***/
{
	uint8_t message_Payload[8] = {0};

	IOBoard->requestedData.cmd = 0x3;
	IOBoard->requestedData.element = ADCx;

	TxHeader.StdId = 0x01;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	message_Payload[0] = 0x31;
	message_Payload[1] = IOBoard->requestedData.myboard_addr;
	message_Payload[2] = IOBoard->requestedData.cmd;
	message_Payload[3] = IOBoard->requestedData.element;
	message_Payload[4] = 0x3B;

	HAL_CAN_AddTxMessage(IOBoard->hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
}
void turn_on_specific_DOUT(IOBoard_t * IOBoard, uint8_t DOUT)	/*** 4 ***/
{
	uint8_t message_Payload[8] = {0};

	IOBoard->requestedData.cmd = 0x4;
	IOBoard->requestedData.element = DOUT;

	TxHeader.StdId = 0x01;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	message_Payload[0] = 0x31;
	message_Payload[1] = IOBoard->requestedData.myboard_addr;
	message_Payload[2] = IOBoard->requestedData.cmd;
	message_Payload[3] = IOBoard->requestedData.element;
	message_Payload[4] = 0x3B;

	HAL_CAN_AddTxMessage(IOBoard->hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
}
void turn_off_specific_DOUT(IOBoard_t * IOBoard, uint8_t DOUT)	/*** 5 ***/
{
	uint8_t message_Payload[8] = {0};

	IOBoard->requestedData.cmd = 0x5;
	IOBoard->requestedData.element = DOUT;

	TxHeader.StdId = 0x01;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	message_Payload[0] = 0x31;
	message_Payload[1] = IOBoard->requestedData.myboard_addr;
	message_Payload[2] = IOBoard->requestedData.cmd;
	message_Payload[3] = IOBoard->requestedData.element;
	message_Payload[4] = 0x3B;

	HAL_CAN_AddTxMessage(IOBoard->hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
}
void ask_for_specific_IS(IOBoard_t * IOBoard, uint8_t ISx)		/*** 6 ***/
{
	uint8_t message_Payload[8] = {0};

	IOBoard->requestedData.cmd = 0x6;
	IOBoard->requestedData.element = ISx;

	TxHeader.StdId = 0x01;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	message_Payload[0] = 0x31;
	message_Payload[1] = IOBoard->requestedData.myboard_addr;
	message_Payload[2] = IOBoard->requestedData.cmd;
	message_Payload[3] = IOBoard->requestedData.element;
	message_Payload[4] = 0x3B;

	HAL_CAN_AddTxMessage(IOBoard->hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
}
void ask_DINs(IOBoard_t * IOBoard)								/*** 7 ***/
{
	uint8_t message_Payload[8] = {0};

	IOBoard->requestedData.cmd = 0x7;
	IOBoard->requestedData.element = 0;

	TxHeader.StdId = 0x01;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	message_Payload[0] = 0x31;
	message_Payload[1] = IOBoard->requestedData.myboard_addr;
	message_Payload[2] = IOBoard->requestedData.cmd;
	message_Payload[3] = IOBoard->requestedData.element;
	message_Payload[4] = 0x3B;

	HAL_CAN_AddTxMessage(IOBoard->hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
}
void ask_for_specific_XA(IOBoard_t * IOBoard, uint8_t XAx)		/*** 8 ***/
{
	uint8_t message_Payload[8] = {0};

	IOBoard->requestedData.cmd = 0x8;
	IOBoard->requestedData.element = XAx;

	TxHeader.StdId = 0x01;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	message_Payload[0] = 0x31;
	message_Payload[1] = IOBoard->requestedData.myboard_addr;
	message_Payload[2] = IOBoard->requestedData.cmd;
	message_Payload[3] = IOBoard->requestedData.element;
	message_Payload[4] = 0x3B;

	HAL_CAN_AddTxMessage(IOBoard->hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
}
void set_address_of_IO(IOBoard_t * IOBoard, uint8_t NewAddr)	/*** 9 ***/
{
	uint8_t message_Payload[8] = {0};

	IOBoard->requestedData.cmd = 0x9;
	IOBoard->requestedData.element = NewAddr;

	TxHeader.StdId = 0x01;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	message_Payload[0] = 0x31;
	message_Payload[1] = IOBoard->requestedData.myboard_addr;
	message_Payload[2] = IOBoard->requestedData.cmd;
	message_Payload[3] = IOBoard->requestedData.element;
	message_Payload[4] = 0x3B;

	HAL_CAN_AddTxMessage(IOBoard->hcan, &TxHeader, message_Payload, (uint32_t*)CAN_TX_MAILBOX0);
}

void ask_ADCs(IOBoard_t * IOBoard)
{
	static uint8_t ADCx = 0;
	ask_for_specific_ADC(IOBoard, IOADCs[ADCx]);
	ADCx++;
	if(ADCx >= sizeof(IOADCs))
	{
		ADCx = 0;
	}
}
void ask_ISs(IOBoard_t * IOBoard)
{
	static uint8_t ISx = 0;
	ask_for_specific_IS(IOBoard, ISx);
	ISx++;
}
void ask_XAs(IOBoard_t * IOBoard)
{
	static uint8_t XAx = 0;
	ask_for_specific_XA(IOBoard, XAx);
	XAx++;
}

void RequestIO(IOBoard_t * IOBoard)
{
	static uint32_t next_request = 0;
	typedef enum requests
	{
		DINs,
		ISs,
		ADCs,
		XAs
	}requests_enum;

	switch(next_request)
	{
		case DINs:
			ask_DINs(IOBoard);
			next_request = ISs;
			break;
		case ISs:
			ask_ISs(IOBoard);
			next_request = ADCs;
			break;
		case ADCs:
			ask_ISs(IOBoard);
			next_request = XAs;
			break;
		case XAs:
			ask_ISs(IOBoard);
			next_request = DINs;
			break;
		default:
			break;
	}
}

