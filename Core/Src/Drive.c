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

//extern modbus_ascii_table_t MODBUS_Table;
extern new_MODBUSTable_uni_t New_MODBUS_Table;
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


void placeIntoTable(void)
{
	if((RxHeader.StdId >= 20) && (RxHeader.StdId < 30))
	{
		New_MODBUS_Table.byte_table[0] = first_packet[0];
		New_MODBUS_Table.byte_table[1] = second_packet[0];
		uint8_t x7 = (second_packet[4] & 0x7)>>1;
		uint8_t x7_placed_as_x6 = (second_packet[4] & x7);
		uint8_t x7_from_another_packet = (second_packet[1] & 0x1)<<7;
		uint8_t x7_with_x6 = x7_placed_as_x6 + x7_from_another_packet;
		New_MODBUS_Table.byte_table[2] |= x7_with_x6;

		New_MODBUS_Table.byte_table[3] |= (second_packet[5]) & 0x80;
		New_MODBUS_Table.byte_table[3] |= (second_packet[5]) & 0x40;
		New_MODBUS_Table.byte_table[3] |= ((second_packet[5]) & 0x8)<<2;
		New_MODBUS_Table.byte_table[3] |= ((second_packet[5]) & 0x2)<<3;
		New_MODBUS_Table.byte_table[3] |= ((second_packet[5]) & 0x1)<<3;
		New_MODBUS_Table.byte_table[3] |= ((second_packet[6]) & 0x20)>>3;
		New_MODBUS_Table.byte_table[3] |= ((second_packet[6]) & 0x4)>>1;
		New_MODBUS_Table.byte_table[3] |= ((second_packet[6]) & 0x2)>>1;


		New_MODBUS_Table.byte_table[4] |= (second_packet[6] & 0x1)<<7;
		New_MODBUS_Table.byte_table[4] |= (third_packet[4])>>1;

		New_MODBUS_Table.byte_table[5] |= (third_packet[4] & 0x1)<<7;
		New_MODBUS_Table.byte_table[5] |= (third_packet[5])>>1;

		New_MODBUS_Table.byte_table[6] |= (third_packet[6] & 0x1)<<7;
		New_MODBUS_Table.byte_table[6] |= (third_packet[7])<<6;

		New_MODBUS_Table.byte_table[8] = third_packet[0];
		New_MODBUS_Table.byte_table[9] = third_packet[1];

		New_MODBUS_Table.byte_table[10] = third_packet[2];
		New_MODBUS_Table.byte_table[11] = third_packet[3];

		New_MODBUS_Table.byte_table[12] = fourth_packet[0];
		New_MODBUS_Table.byte_table[13] = fourth_packet[1];

		New_MODBUS_Table.byte_table[14] = fourth_packet[2];
		New_MODBUS_Table.byte_table[15] = fourth_packet[3];

		New_MODBUS_Table.byte_table[16] = fourth_packet[4];
		New_MODBUS_Table.byte_table[17] = fourth_packet[5];

		New_MODBUS_Table.byte_table[18] = fourth_packet[6];
		New_MODBUS_Table.byte_table[19] = fourth_packet[7];
	}
	else if(RxHeader.StdId >= 30)
	{
		New_MODBUS_Table.byte_table[0+20] = first_packet[0];
		New_MODBUS_Table.byte_table[1+20] = second_packet[0];
		uint8_t x7 = (second_packet[4] & 0x7)>>1;
		uint8_t x7_placed_as_x6 = (second_packet[4] & x7);
		uint8_t x7_from_another_packet = (second_packet[1] & 0x1)<<7;
		uint8_t x7_with_x6 = x7_placed_as_x6 + x7_from_another_packet;
		New_MODBUS_Table.byte_table[2+20] |= x7_with_x6;

		New_MODBUS_Table.byte_table[3+20] |= (second_packet[5]) & 0x80;
		New_MODBUS_Table.byte_table[3+20] |= (second_packet[5]) & 0x40;
		New_MODBUS_Table.byte_table[3+20] |= ((second_packet[5]) & 0x8)<<2;
		New_MODBUS_Table.byte_table[3+20] |= ((second_packet[5]) & 0x2)<<3;
		New_MODBUS_Table.byte_table[3+20] |= ((second_packet[5]) & 0x1)<<3;
		New_MODBUS_Table.byte_table[3+20] |= ((second_packet[6]) & 0x20)>>3;
		New_MODBUS_Table.byte_table[3+20] |= ((second_packet[6]) & 0x4)>>1;
		New_MODBUS_Table.byte_table[3+20] |= ((second_packet[6]) & 0x2)>>1;


		New_MODBUS_Table.byte_table[4+20] |= (second_packet[6] & 0x1)<<7;
		New_MODBUS_Table.byte_table[4+20] |= (third_packet[4])>>1;

		New_MODBUS_Table.byte_table[5+20] |= (third_packet[4] & 0x1)<<7;
		New_MODBUS_Table.byte_table[5+20] |= (third_packet[5])>>1;

		New_MODBUS_Table.byte_table[6+20] |= (third_packet[6] & 0x1)<<7;
		New_MODBUS_Table.byte_table[6+20] |= (third_packet[7])<<6;

		New_MODBUS_Table.byte_table[8+20] = third_packet[0];
		New_MODBUS_Table.byte_table[9+20] = third_packet[1];

		New_MODBUS_Table.byte_table[10+20] = third_packet[2];
		New_MODBUS_Table.byte_table[11+20] = third_packet[3];

		New_MODBUS_Table.byte_table[12+20] = fourth_packet[0];
		New_MODBUS_Table.byte_table[13+20] = fourth_packet[1];

		New_MODBUS_Table.byte_table[14+20] = fourth_packet[2];
		New_MODBUS_Table.byte_table[15+20] = fourth_packet[3];

		New_MODBUS_Table.byte_table[16+20] = fourth_packet[4];
		New_MODBUS_Table.byte_table[17+20] = fourth_packet[5];

		New_MODBUS_Table.byte_table[18+20] = fourth_packet[6];
		New_MODBUS_Table.byte_table[19+20] = fourth_packet[7];
	}

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
		GET_RESPONSE,
		PLACE_INTO_TABLE
	}IO_pocesses_enum;
	switch(current_process)
	{
		case REQUEST:
			askPacket(&hcan1);
			current_process = GET_RESPONSE;
			go_to_the_next_block = NO;
			break;
		case GET_RESPONSE:
			if(HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
			{
				Error_Handler();
			}
			current_process = PLACE_INTO_TABLE;
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

