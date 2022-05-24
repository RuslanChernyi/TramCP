#include "Drive.h"
#include "modbus_ascii.h"
#include "systick.h"
#include "main.h"
#include "can.h"

#define NO_PACKET_RECEIVED	0x1
#define STID (0x7FF<<15)

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

uint32_t askPacket(CAN_HandleTypeDef * hcan, uint8_t packet_to_ask)
{
	CAN_TxHeaderTypeDef TxHeader;
	// Form a request
	TxHeader.StdId = packet_to_ask;
	TxHeader.ExtId = 0x00;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;

	uint8_t dummy[8] = {0};
	// Send a request drive board for a packet
	HAL_CAN_AddTxMessage(hcan, &TxHeader, dummy, (uint32_t*)CAN_TX_MAILBOX0);

	return HAL_OK;
}

/*** Notice:
 *			receivePacket function was replaced by interrupt handling (HAL_CAN_RxFifo0MsgPendingCallback in main)
 */
void get_packet(CAN_HandleTypeDef * hcan)
{
	uint8_t id = (hcan->Instance->sFIFOMailBox[0].RIR & STID)>>21;
	switch(id)
	{
		case 20:
			HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData_fifo);
			for(int i = 0; i < 8; i++)
			{
				first_packet[i] = RxData_fifo[i];
			}
			break;
		case 21:
			HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData_fifo);
			for(int i = 0; i < 8; i++)
			{
				second_packet[i] = RxData_fifo[i];
			}
			break;
		case 22:
			HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData_fifo);
			for(int i = 0; i < 8; i++)
			{
				third_packet[i] = RxData_fifo[i];
			}
			break;
		case 23:
			HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData_fifo);
			for(int i = 0; i < 8; i++)
			{
				fourth_packet[i] = RxData_fifo[i];
			}
			break;
		default:
			break;
	}
}

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
	static uint8_t packet_to_ask = 20;
	typedef enum
	{
		NO,
		YES
	}go_next_enum;
	uint32_t packet = 0;
	typedef enum IO_pocesses
	{
		REQUEST,
		GET_RESPONSE,
		PLACE_INTO_TABLE
	}IO_pocesses_enum;
	switch(current_process)
	{
		case REQUEST:
			askPacket(&hcan2, packet_to_ask);
			current_process = GET_RESPONSE;
			go_to_the_next_block = NO;
			break;
		case GET_RESPONSE:
			get_packet(&hcan2);
			current_process = PLACE_INTO_TABLE;

			break;
		case PLACE_INTO_TABLE:
			if(packet_to_ask >= 23)
			{
				placeIntoTable();
				go_to_the_next_block = YES;
				packet_to_ask = 20;
			}
			else
			{
				packet_to_ask++;
			}
			current_process = REQUEST;
			break;
	}
	return go_to_the_next_block;
}

