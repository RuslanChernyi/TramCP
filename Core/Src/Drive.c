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

static uint32_t request(CAN_HandleTypeDef * hcan, uint8_t packet_to_ask)
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
static void receive(CAN_HandleTypeDef * hcan)
{
	uint32_t id = (hcan->Instance->sFIFOMailBox[1].RIR)>>21;
	switch(id)
	{
		case 20:
			HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, RxData_fifo);
			for(int i = 0; i < 8; i++)
			{
				first_packet[i] = RxData_fifo[i];
			}
			break;
		case 21:
			HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, RxData_fifo);
			for(int i = 0; i < 8; i++)
			{
				second_packet[i] = RxData_fifo[i];
			}
			break;
		case 22:
			HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, RxData_fifo);
			for(int i = 0; i < 8; i++)
			{
				third_packet[i] = RxData_fifo[i];
			}
			break;
		case 23:
			HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, RxData_fifo);
			for(int i = 0; i < 8; i++)
			{
				fourth_packet[i] = RxData_fifo[i];
			}
			break;
		default:
			hcan->Instance->RF1R &= ~(1U<<4);
			break;
	}
}

static void place(void)
{
	if((RxHeader.StdId >= 20) && (RxHeader.StdId < 30))
	{
		New_MODBUS_Table.bit_table.idtA_struct.byte_1.TXDI 		= 0;//(first_packet[0] & 0x80)>>7;
		New_MODBUS_Table.bit_table.idtA_struct.byte_1.TXDU 		= 0;//(first_packet[0] & 0x40)>>6;
		New_MODBUS_Table.bit_table.idtA_struct.byte_1.TXDVT 	= 0;//(first_packet[0] & 0x20)>>5;
		New_MODBUS_Table.bit_table.idtA_struct.byte_1.TXDVT1 	= 0;//(first_packet[0] & 0x10)>>4;
		New_MODBUS_Table.bit_table.idtA_struct.byte_1.DVT 		= 0;//(first_packet[0] & 0x08)>>3;
		New_MODBUS_Table.bit_table.idtA_struct.byte_1.DPIT 		= 0;//(first_packet[0] & 0x04)>>2;
		New_MODBUS_Table.bit_table.idtA_struct.byte_1.UMAX2 	= 0;//(first_packet[0] & 0x02)>>1;
		New_MODBUS_Table.bit_table.idtA_struct.byte_1.DI 		= 0;//(first_packet[0] & 0x01)>>0;

		New_MODBUS_Table.bit_table.idtA_struct.byte_2.YDET 		= (second_packet[0] & 0x80)>>7;
		New_MODBUS_Table.bit_table.idtA_struct.byte_2.YRT1 		= (second_packet[0] & 0x40)>>6;
		New_MODBUS_Table.bit_table.idtA_struct.byte_2.MT2 		= (second_packet[0] & 0x20)>>5;
		New_MODBUS_Table.bit_table.idtA_struct.byte_2.MT1 		= (second_packet[0] & 0x10)>>4;
		New_MODBUS_Table.bit_table.idtA_struct.byte_2.YK04 		= (second_packet[0] & 0x08)>>3;
		New_MODBUS_Table.bit_table.idtA_struct.byte_2.YYB 		= (second_packet[0] & 0x04)>>2;
		New_MODBUS_Table.bit_table.idtA_struct.byte_2.YKL3 		= (second_packet[0] & 0x02)>>1;
		New_MODBUS_Table.bit_table.idtA_struct.byte_2.YKL1		= (second_packet[0] & 0x01)>>0;

		New_MODBUS_Table.bit_table.idtA_struct.byte_3.YRT2 		= (second_packet[1] & 0x01)>>0;
		New_MODBUS_Table.bit_table.idtA_struct.byte_3.XSQ5 		= (second_packet[4] & 0x80)>>7;
		New_MODBUS_Table.bit_table.idtA_struct.byte_3.XZ3 		= (second_packet[4] & 0x20)>>5;
		New_MODBUS_Table.bit_table.idtA_struct.byte_3.XZ2 		= (second_packet[4] & 0x10)>>4;
		New_MODBUS_Table.bit_table.idtA_struct.byte_3.XZ1 		= (second_packet[4] & 0x08)>>3;
		New_MODBUS_Table.bit_table.idtA_struct.byte_3.XH 		= (second_packet[4] & 0x04)>>2;
		New_MODBUS_Table.bit_table.idtA_struct.byte_3.XZ 		= (second_packet[4] & 0x02)>>1;
		New_MODBUS_Table.bit_table.idtA_struct.byte_3.XP 		= (second_packet[4] & 0x01)>>0;

		New_MODBUS_Table.bit_table.idtA_struct.byte_4.XKL3 		= (second_packet[5] & 0x80)>>7;
		New_MODBUS_Table.bit_table.idtA_struct.byte_4.XKL1 		= (second_packet[5] & 0x40)>>6;
		New_MODBUS_Table.bit_table.idtA_struct.byte_4.XKYB 		= (second_packet[5] & 0x08)>>3;
		New_MODBUS_Table.bit_table.idtA_struct.byte_4.XK01 		= (second_packet[5] & 0x02)>>1;
		New_MODBUS_Table.bit_table.idtA_struct.byte_4.XT 		= (second_packet[5] & 0x01)>>0;
		New_MODBUS_Table.bit_table.idtA_struct.byte_4.XET3 		= (second_packet[6] & 0x20)>>5;
		New_MODBUS_Table.bit_table.idtA_struct.byte_4.XMT 		= (second_packet[6] & 0x04)>>2;
		New_MODBUS_Table.bit_table.idtA_struct.byte_4.XET1 		= (second_packet[6] & 0x02)>>1;


		New_MODBUS_Table.bit_table.idtA_struct.byte_5.XAT 		= (second_packet[6] & 0x01)>>0;
		New_MODBUS_Table.bit_table.idtA_struct.byte_5.XDVT5 	= (third_packet[4] & 0x80)>>7;
		New_MODBUS_Table.bit_table.idtA_struct.byte_5.XDVT4 	= (third_packet[4] & 0x40)>>6;
		New_MODBUS_Table.bit_table.idtA_struct.byte_5.XDVT3 	= (third_packet[4] & 0x20)>>5;
		New_MODBUS_Table.bit_table.idtA_struct.byte_5.XDVT2 	= (third_packet[4] & 0x10)>>4;
		New_MODBUS_Table.bit_table.idtA_struct.byte_5.XDVT1 	= (third_packet[4] & 0x08)>>3;
		New_MODBUS_Table.bit_table.idtA_struct.byte_5.TZ 		= (third_packet[4] & 0x04)>>2;
		New_MODBUS_Table.bit_table.idtA_struct.byte_5.XVENT 	= (third_packet[4] & 0x02)>>1;

		New_MODBUS_Table.bit_table.idtA_struct.byte_6.XKT 		= (third_packet[4] & 0x01)>>0;
		New_MODBUS_Table.bit_table.idtA_struct.byte_6.LED_VD13 	= (third_packet[5] & 0x80)>>7;
		New_MODBUS_Table.bit_table.idtA_struct.byte_6.LED_VD12 	= (third_packet[5] & 0x40)>>6;
		New_MODBUS_Table.bit_table.idtA_struct.byte_6.YVENT 	= (third_packet[5] & 0x20)>>5;
		New_MODBUS_Table.bit_table.idtA_struct.byte_6.XDU 		= (third_packet[5] & 0x10)>>4;
		New_MODBUS_Table.bit_table.idtA_struct.byte_6.XDI 		= (third_packet[5] & 0x08)>>3;
		New_MODBUS_Table.bit_table.idtA_struct.byte_6.XDVT 		= (third_packet[5] & 0x04)>>2;
		New_MODBUS_Table.bit_table.idtA_struct.byte_6.XDVT7 	= (third_packet[5] & 0x02)>>1;

		New_MODBUS_Table.bit_table.idtA_struct.byte_7.XDVT6 	= (third_packet[5] & 0x01)>>0;
		New_MODBUS_Table.bit_table.idtA_struct.byte_7.XV0 		= (third_packet[6] & 0x01)>>0;

		New_MODBUS_Table.bit_table.idtA_struct.U[0] 			=	third_packet[0];
		New_MODBUS_Table.bit_table.idtA_struct.U[1] 			=	third_packet[1];
		New_MODBUS_Table.bit_table.idtA_struct.I[0] 			=	third_packet[2];
		New_MODBUS_Table.bit_table.idtA_struct.I[1] 			=	third_packet[3];
		New_MODBUS_Table.bit_table.idtA_struct.XTT[0] 			=	fourth_packet[0];
		New_MODBUS_Table.bit_table.idtA_struct.XTT[1] 			=	fourth_packet[1];
		New_MODBUS_Table.bit_table.idtA_struct.XTH[0] 			=	fourth_packet[2];
		New_MODBUS_Table.bit_table.idtA_struct.XTH[1] 			=	fourth_packet[3];
		New_MODBUS_Table.bit_table.idtA_struct.UY[0] 			=	fourth_packet[4];
		New_MODBUS_Table.bit_table.idtA_struct.UY[1] 			=	fourth_packet[5];
		New_MODBUS_Table.bit_table.idtA_struct.CI[0] 			=	fourth_packet[6];
		New_MODBUS_Table.bit_table.idtA_struct.CI[1] 			=	fourth_packet[7];

	}
	else if(RxHeader.StdId >= 30)
	{
		New_MODBUS_Table.bit_table.idtB_struct.byte_1.TXDI 		= (first_packet[0] & 0x80)>>7;
		New_MODBUS_Table.bit_table.idtB_struct.byte_1.TXDU 		= (first_packet[0] & 0x40)>>6;
		New_MODBUS_Table.bit_table.idtB_struct.byte_1.TXDVT 	= (first_packet[0] & 0x20)>>5;
		New_MODBUS_Table.bit_table.idtB_struct.byte_1.TXDVT1 	= (first_packet[0] & 0x10)>>4;
		New_MODBUS_Table.bit_table.idtB_struct.byte_1.DVT 		= (first_packet[0] & 0x08)>>3;
		New_MODBUS_Table.bit_table.idtB_struct.byte_1.DPIT 		= (first_packet[0] & 0x04)>>2;
		New_MODBUS_Table.bit_table.idtB_struct.byte_1.UMAX2 	= (first_packet[0] & 0x02)>>1;
		New_MODBUS_Table.bit_table.idtB_struct.byte_1.DI 		= (first_packet[0] & 0x01)>>0;

		New_MODBUS_Table.bit_table.idtB_struct.byte_2.YDET 		= (second_packet[0] & 0x80)>>7;
		New_MODBUS_Table.bit_table.idtB_struct.byte_2.YRT1 		= (second_packet[0] & 0x40)>>6;
		New_MODBUS_Table.bit_table.idtB_struct.byte_2.MT2 		= (second_packet[0] & 0x20)>>5;
		New_MODBUS_Table.bit_table.idtB_struct.byte_2.MT1 		= (second_packet[0] & 0x10)>>4;
		New_MODBUS_Table.bit_table.idtB_struct.byte_2.YK04 		= (second_packet[0] & 0x08)>>3;
		New_MODBUS_Table.bit_table.idtB_struct.byte_2.YYB 		= (second_packet[0] & 0x04)>>2;
		New_MODBUS_Table.bit_table.idtB_struct.byte_2.YKL3 		= (second_packet[0] & 0x02)>>1;
		New_MODBUS_Table.bit_table.idtB_struct.byte_2.YKL1		= (second_packet[0] & 0x01)>>0;

		New_MODBUS_Table.bit_table.idtB_struct.byte_3.YRT2 		= (second_packet[1] & 0x01)>>0;
		New_MODBUS_Table.bit_table.idtB_struct.byte_3.XSQ5 		= (second_packet[4] & 0x80)>>7;
		New_MODBUS_Table.bit_table.idtB_struct.byte_3.XZ3 		= (second_packet[4] & 0x20)>>5;
		New_MODBUS_Table.bit_table.idtB_struct.byte_3.XZ2 		= (second_packet[4] & 0x10)>>4;
		New_MODBUS_Table.bit_table.idtB_struct.byte_3.XZ1 		= (second_packet[4] & 0x08)>>3;
		New_MODBUS_Table.bit_table.idtB_struct.byte_3.XH 		= (second_packet[4] & 0x04)>>2;
		New_MODBUS_Table.bit_table.idtB_struct.byte_3.XZ 		= (second_packet[4] & 0x02)>>1;
		New_MODBUS_Table.bit_table.idtB_struct.byte_3.XP 		= (second_packet[4] & 0x01)>>0;

		New_MODBUS_Table.bit_table.idtB_struct.byte_4.XKL3 		= (second_packet[5] & 0x80)>>7;
		New_MODBUS_Table.bit_table.idtB_struct.byte_4.XKL1 		= (second_packet[5] & 0x40)>>6;
		New_MODBUS_Table.bit_table.idtB_struct.byte_4.XKYB 		= (second_packet[5] & 0x08)>>3;
		New_MODBUS_Table.bit_table.idtB_struct.byte_4.XK01 		= (second_packet[5] & 0x02)>>1;
		New_MODBUS_Table.bit_table.idtB_struct.byte_4.XT 		= (second_packet[5] & 0x01)>>0;
		New_MODBUS_Table.bit_table.idtB_struct.byte_4.XET3 		= (second_packet[6] & 0x20)>>5;
		New_MODBUS_Table.bit_table.idtB_struct.byte_4.XMT 		= (second_packet[6] & 0x04)>>2;
		New_MODBUS_Table.bit_table.idtB_struct.byte_4.XET1 		= (second_packet[6] & 0x02)>>1;


		New_MODBUS_Table.bit_table.idtB_struct.byte_5.XAT 		= (second_packet[6] & 0x01)>>0;
		New_MODBUS_Table.bit_table.idtB_struct.byte_5.XDVT5 	= (third_packet[4] & 0x80)>>7;
		New_MODBUS_Table.bit_table.idtB_struct.byte_5.XDVT4 	= (third_packet[4] & 0x40)>>6;
		New_MODBUS_Table.bit_table.idtB_struct.byte_5.XDVT3 	= (third_packet[4] & 0x20)>>5;
		New_MODBUS_Table.bit_table.idtB_struct.byte_5.XDVT2 	= (third_packet[4] & 0x10)>>4;
		New_MODBUS_Table.bit_table.idtB_struct.byte_5.XDVT1 	= (third_packet[4] & 0x08)>>3;
		New_MODBUS_Table.bit_table.idtB_struct.byte_5.TZ 		= (third_packet[4] & 0x04)>>2;
		New_MODBUS_Table.bit_table.idtB_struct.byte_5.XVENT 	= (third_packet[4] & 0x02)>>1;

		New_MODBUS_Table.bit_table.idtB_struct.byte_6.XKT 		= (third_packet[4] & 0x01)>>0;
		New_MODBUS_Table.bit_table.idtB_struct.byte_6.LED_VD13 	= (third_packet[5] & 0x80)>>7;
		New_MODBUS_Table.bit_table.idtB_struct.byte_6.LED_VD12 	= (third_packet[5] & 0x40)>>6;
		New_MODBUS_Table.bit_table.idtB_struct.byte_6.YVENT 	= (third_packet[5] & 0x20)>>5;
		New_MODBUS_Table.bit_table.idtB_struct.byte_6.XDU 		= (third_packet[5] & 0x10)>>4;
		New_MODBUS_Table.bit_table.idtB_struct.byte_6.XDI 		= (third_packet[5] & 0x08)>>3;
		New_MODBUS_Table.bit_table.idtB_struct.byte_6.XDVT 		= (third_packet[5] & 0x04)>>2;
		New_MODBUS_Table.bit_table.idtB_struct.byte_6.XDVT7 	= (third_packet[5] & 0x02)>>1;

		New_MODBUS_Table.bit_table.idtB_struct.byte_7.XDVT6 	= (third_packet[5] & 0x01)>>0;
		New_MODBUS_Table.bit_table.idtB_struct.byte_7.XV0 		= (third_packet[6] & 0x01)>>0;

		New_MODBUS_Table.bit_table.idtB_struct.U[0] 			=	third_packet[0];
		New_MODBUS_Table.bit_table.idtB_struct.U[1] 			=	third_packet[1];
		New_MODBUS_Table.bit_table.idtB_struct.I[0] 			=	third_packet[2];
		New_MODBUS_Table.bit_table.idtB_struct.I[1] 			=	third_packet[3];
		New_MODBUS_Table.bit_table.idtB_struct.XTT[0] 			=	fourth_packet[0];
		New_MODBUS_Table.bit_table.idtB_struct.XTT[1] 			=	fourth_packet[1];
		New_MODBUS_Table.bit_table.idtB_struct.XTH[0] 			=	fourth_packet[2];
		New_MODBUS_Table.bit_table.idtB_struct.XTH[1] 			=	fourth_packet[3];
		New_MODBUS_Table.bit_table.idtB_struct.UY[0] 			=	fourth_packet[4];
		New_MODBUS_Table.bit_table.idtB_struct.UY[1] 			=	fourth_packet[5];
		New_MODBUS_Table.bit_table.idtB_struct.CI[0] 			=	fourth_packet[6];
		New_MODBUS_Table.bit_table.idtB_struct.CI[1] 			=	fourth_packet[7];
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
			request(&hcan2, packet_to_ask);
			current_process = GET_RESPONSE;
			go_to_the_next_block = NO;
			break;
		case GET_RESPONSE:
			receive(&hcan2);
			current_process = PLACE_INTO_TABLE;

			break;
		case PLACE_INTO_TABLE:
			if(packet_to_ask >= 23)
			{
				place();
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

