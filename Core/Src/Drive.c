#include "Drive.h"
#include "modbus_ascii.h"
#include "systick.h"
#include "main.h"
#include "can.h"

#define NO_PACKET_RECEIVED	0x1

typedef enum processes
{
	ASK,
	RECEIVE,
	PLACE
}processes_t;

extern CAN_RxHeaderTypeDef RxHeader;
extern uint8_t RxData_fifo[8];

extern uint8_t first_packet[8];
extern uint8_t second_packet[8];
extern uint8_t third_packet[8];
extern uint8_t fourth_packet[8];

extern modbus_ascii_table_t MODBUS_Table;

extern uint32_t processInIO;

extern uint32_t allow_placement;

uint32_t askPacket(CAN_HandleTypeDef * hcan)
{
	CAN_TxHeaderTypeDef TxHeader;
	// Form a request
	TxHeader.StdId = 0x14;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_REMOTE;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;

	uint8_t transmit_dummy = 0;
	// Send a request drive board for a packet
	HAL_CAN_AddTxMessage(hcan, &TxHeader, &transmit_dummy, (uint32_t*)CAN_TX_MAILBOX0);
	return HAL_OK;
}

/*** Notice:
 *			receivePacket function was replaced by interrupt handling (HAL_CAN_RxFifo0MsgPendingCallback in main)
 */

//uint32_t receivePacket(CAN_HandleTypeDef * hcan)
//{
//	// Get the packet from the FIFO
//	if(hcan->Instance->RF1R & (3U<<0))
//	{
//		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, RxData_fifo);
//		for(int i = 0; i < 8; i++)
//		{
//			fourth_packet[i] = RxData_fifo[i];
//		}
//	}
//	else if(hcan->Instance->RF0R & (3U<<0))
//	{
//		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData_fifo);
//		for(int i = 0; i < 8; i++)
//		{
//			first_packet[i] = RxData_fifo[i];
//		}
//		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData_fifo);
//		for(int i = 0; i < 8; i++)
//		{
//			second_packet[i] = RxData_fifo[i];
//		}
//		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData_fifo);
//		for(int i = 0; i < 8; i++)
//		{
//			third_packet[i] = RxData_fifo[i];
//		}
//	}
//	else
//	{
//		return NO_PACKET_RECEIVED;
//	}
//}

void placeIntoTable(void)
{
	MODBUS_Table.modbus_table[0x00] = first_packet[0];
	MODBUS_Table.modbus_table[0x01] = second_packet[0];
	MODBUS_Table.modbus_table[0x02] = second_packet[1];
	MODBUS_Table.modbus_table[0x03] = second_packet[2];
	MODBUS_Table.modbus_table[0x04] = second_packet[3];
	MODBUS_Table.modbus_table[0x05] = second_packet[4];
	MODBUS_Table.modbus_table[0x06] = second_packet[5];
	MODBUS_Table.modbus_table[0x07] = second_packet[6];
	MODBUS_Table.modbus_table[0x08] = second_packet[7];
	MODBUS_Table.imp_drv_table.U[0] = third_packet[0];
	MODBUS_Table.imp_drv_table.U[1] = third_packet[1];
	MODBUS_Table.imp_drv_table.I[0] = third_packet[2];
	MODBUS_Table.imp_drv_table.I[1] = third_packet[3];
	MODBUS_Table.modbus_table[0x09] = third_packet[4];
	MODBUS_Table.modbus_table[0x0A] = third_packet[5];
	MODBUS_Table.modbus_table[0x0B] = third_packet[6];
	MODBUS_Table.modbus_table[0x0C] = third_packet[7];
	MODBUS_Table.imp_drv_table.XTT[0] = fourth_packet[0];
	MODBUS_Table.imp_drv_table.XTT[1] = fourth_packet[1];
	MODBUS_Table.imp_drv_table.XTH[0] = fourth_packet[2];
	MODBUS_Table.imp_drv_table.XTH[1] = fourth_packet[3];
	MODBUS_Table.imp_drv_table.UY[0] = fourth_packet[4];
	MODBUS_Table.imp_drv_table.UY[1] = fourth_packet[5];
	MODBUS_Table.imp_drv_table.CI[0] = fourth_packet[6];
	MODBUS_Table.imp_drv_table.CI[1] = fourth_packet[7];
}

uint32_t CDR(void)
{
	static uint32_t current_process = 0;
	uint32_t go_to_the_next_block = 0;
	static uint32_t timeout_counter = 0;

	typedef enum
	{
		NO,
		YES
	}go_next_enum;
	typedef enum IO_pocesses
	{
		REQUEST,
		PLACE_INTO_TABLE
	}IO_pocesses_enum;
	switch(current_process)
	{
		case REQUEST:
			askPacket(&hcan1);
			current_process = PLACE_INTO_TABLE;
			go_to_the_next_block = NO;
			break;
		case PLACE_INTO_TABLE:
			if((allow_placement == YES) || timeout_counter < 5)
			{
				placeIntoTable();
				current_process = REQUEST;
				go_to_the_next_block = YES;
				allow_placement = NO;
				timeout_counter = 0;
			}
			else
			{
				go_to_the_next_block = NO;
				timeout_counter++;
			}
			break;
	}
	return go_to_the_next_block;
}

