
#define SLAVE_DEVICE_ADDRESS		01
#define NO_START_BYTE				0x9
#define WRONG_SLAVE					0xA
#define WRONG_FUNCTION_CODE			0xB
#define NO_LF_CODE					0xD
#define NO_CR_CODE					0xE
#define WRONG_LRC					0xF
#define ADDRESS_LOWER_THAN_MIN		0x1
#define QUANTITY_BIGGER_THAN_MODBUS	0x2
#define BAD_QUANTITY_IN_RESPONSE	0x8
#define TIMEOUT_IN_UART_RECEIVE		0x7
#define STARTING_BIT				(0x1<<7)
#define PARITY_BIT					(0x1<<1)
#define ENDING_BIT					(0x1<<0)

#include "modbus_ascii.h"
#include <stdlib.h>
extern modbus_ascii_request_msg_t modbus_request_msg;
extern modbus_ascii_response_msg_t modbus_response_msg;
extern reqest_info_t request_info;
const char ASCII_Table[] ={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

extern modbus_ascii_table_t MODBUS_Table;
extern uint32_t error_notice;

// Receive a request
uint32_t receive_modbus_request(UART_HandleTypeDef* uart, reqest_info_t * req)
{
	if(uart->Instance->SR && (1U<<5))
	{
		uint32_t size = 8;
		int i = 0;
		uint8_t buffer[17] = {0};
		ascii_hex_code_t ascii_to_hex_temp = {0};
		uint32_t Timeout = 1;
		/*** Look for ':' (Start of modbus ascii message) ***/
		HAL_UART_Receive(uart, buffer, 1, Timeout);
		// Get request into buffer
		if(buffer[0] == ':')
		{
			/*** Get the rest of the message ***/
			HAL_UART_Receive(uart, &buffer[1], 16, 1);

			// Then parse the request
			// Check for starting byte (:)
			modbus_request_msg.ascii_message.header = buffer[0];
			if(!(modbus_request_msg.ascii_message.header == START_CODE))
			{
				return NO_START_BYTE;
			}
			// Check for slave address
			ascii_to_hex_temp.first_symbol = buffer[1];
			ascii_to_hex_temp.second_symbol = buffer[2];
			modbus_request_msg.ascii_message.slave_address = ASCIIHex(&ascii_to_hex_temp);
			req->slave_address = modbus_request_msg.ascii_message.slave_address;
			if(!(modbus_request_msg.ascii_message.slave_address == SLAVE_DEVICE_ADDRESS))
			{
				return WRONG_SLAVE;
			}
			// Check for Function code
			ascii_to_hex_temp.first_symbol = buffer[3];
			ascii_to_hex_temp.second_symbol = buffer[4];
			modbus_request_msg.ascii_message.function = ASCIIHex(&ascii_to_hex_temp);
			req->func_code = modbus_request_msg.ascii_message.function;
			if((modbus_request_msg.ascii_message.function < 1) || (modbus_request_msg.ascii_message.function > 8))
			{
				return WRONG_FUNCTION_CODE;
			}
			// Get starting address
			ascii_to_hex_temp.first_symbol = buffer[5];
			ascii_to_hex_temp.second_symbol = buffer[6];
			modbus_request_msg.ascii_message.start_addr_hi = ASCIIHex(&ascii_to_hex_temp);
			ascii_to_hex_temp.first_symbol = buffer[7];
			ascii_to_hex_temp.second_symbol = buffer[8];
			modbus_request_msg.ascii_message.start_addr_lo = ASCIIHex(&ascii_to_hex_temp);
			req->starting_address = modbus_request_msg.ascii_message.start_addr_hi<<4;
			req->starting_address |= modbus_request_msg.ascii_message.start_addr_lo;
			if(req->starting_address < MIN_MODBUS_TABLE_ADDRESS)
			{
				req->starting_address = MIN_MODBUS_TABLE_ADDRESS;
				error_notice = ADDRESS_LOWER_THAN_MIN;
			}
			// Get quantity
			ascii_to_hex_temp.first_symbol = buffer[9];
			ascii_to_hex_temp.second_symbol = buffer[10];
			modbus_request_msg.ascii_message.quantity_hi = ASCIIHex(&ascii_to_hex_temp);
			ascii_to_hex_temp.first_symbol = buffer[11];
			ascii_to_hex_temp.second_symbol = buffer[12];
			modbus_request_msg.ascii_message.quantity_lo = ASCIIHex(&ascii_to_hex_temp);
			req->quantity =	modbus_request_msg.ascii_message.quantity_hi<<4;
			req->quantity |= modbus_request_msg.ascii_message.quantity_lo;
			if(req->quantity > MAX_MODBUS_TABLE_VALUE)
			{
				req->quantity = MAX_MODBUS_TABLE_VALUE;
				error_notice = QUANTITY_BIGGER_THAN_MODBUS;
			}
			// Check error(LRC)
			ascii_to_hex_temp.first_symbol = buffer[13];
			ascii_to_hex_temp.second_symbol = buffer[14];
			modbus_request_msg.ascii_message.lrc_check = ASCIIHex(&ascii_to_hex_temp);
			uint8_t myLRCcheck = lrc_generation(6, &modbus_request_msg.ascii_message_array[1]);
			if(modbus_request_msg.ascii_message.lrc_check != myLRCcheck)
			{
				return WRONG_LRC;
			}

			// Check for end bytes
			if(buffer[15] == CR_CODE)
			{
				modbus_request_msg.ascii_message.trailer.CR = buffer[15];
				if(buffer[16] == LF_CODE)
				{
					modbus_request_msg.ascii_message.trailer.LF = buffer[16];
					return HAL_OK;
				}
				else
				{
					return NO_LF_CODE;
				}
			}
			else
			{
				return NO_CR_CODE;
			}
		}
	}
	else
	{
	}

	return HAL_ERROR;
}

uint32_t check_modbus_request(uint8_t * buffer,reqest_info_t * req)
{
	uint32_t size = 8;
	int i = 0;
	ascii_hex_code_t ascii_to_hex_temp = {0};
	/*** Look for ':' (Start of modbus ascii message) ***/
	// Get request into buffer
	if(buffer[0] == ':')
	{
		// Parse the request
		// Check for starting byte (:)
		modbus_request_msg.ascii_message.header = buffer[0];
		if(!(modbus_request_msg.ascii_message.header == START_CODE))
		{
			return NO_START_BYTE;
		}
		// Check for slave address
		ascii_to_hex_temp.first_symbol = buffer[1];
		ascii_to_hex_temp.second_symbol = buffer[2];
		modbus_request_msg.ascii_message.slave_address = ASCIIHex(&ascii_to_hex_temp);
		req->slave_address = modbus_request_msg.ascii_message.slave_address;
		if(!(modbus_request_msg.ascii_message.slave_address == SLAVE_DEVICE_ADDRESS))
		{
			return WRONG_SLAVE;
		}
		// Check for Function code
		ascii_to_hex_temp.first_symbol = buffer[3];
		ascii_to_hex_temp.second_symbol = buffer[4];
		modbus_request_msg.ascii_message.function = ASCIIHex(&ascii_to_hex_temp);
		req->func_code = modbus_request_msg.ascii_message.function;
		if((modbus_request_msg.ascii_message.function < 1) || (modbus_request_msg.ascii_message.function > 8))
		{
			return WRONG_FUNCTION_CODE;
		}
		// Get starting address
		ascii_to_hex_temp.first_symbol = buffer[5];
		ascii_to_hex_temp.second_symbol = buffer[6];
		modbus_request_msg.ascii_message.start_addr_hi = ASCIIHex(&ascii_to_hex_temp);
		ascii_to_hex_temp.first_symbol = buffer[7];
		ascii_to_hex_temp.second_symbol = buffer[8];
		modbus_request_msg.ascii_message.start_addr_lo = ASCIIHex(&ascii_to_hex_temp);
		req->starting_address = modbus_request_msg.ascii_message.start_addr_hi<<4;
		req->starting_address |= modbus_request_msg.ascii_message.start_addr_lo;
		if(req->starting_address < MIN_MODBUS_TABLE_ADDRESS)
		{
			req->starting_address = MIN_MODBUS_TABLE_ADDRESS;
			error_notice = ADDRESS_LOWER_THAN_MIN;
		}
		// Get quantity
		ascii_to_hex_temp.first_symbol = buffer[9];
		ascii_to_hex_temp.second_symbol = buffer[10];
		modbus_request_msg.ascii_message.quantity_hi = ASCIIHex(&ascii_to_hex_temp);
		ascii_to_hex_temp.first_symbol = buffer[11];
		ascii_to_hex_temp.second_symbol = buffer[12];
		modbus_request_msg.ascii_message.quantity_lo = ASCIIHex(&ascii_to_hex_temp);
		req->quantity =	modbus_request_msg.ascii_message.quantity_hi<<4;
		req->quantity |= modbus_request_msg.ascii_message.quantity_lo;
		if(req->quantity > MAX_MODBUS_TABLE_VALUE)
		{
			req->quantity = MAX_MODBUS_TABLE_VALUE;
			error_notice = QUANTITY_BIGGER_THAN_MODBUS;
		}
		// Check error(LRC)
		ascii_to_hex_temp.first_symbol = buffer[13];
		ascii_to_hex_temp.second_symbol = buffer[14];
		modbus_request_msg.ascii_message.lrc_check = ASCIIHex(&ascii_to_hex_temp);
		uint8_t myLRCcheck = lrc_generation(6, &modbus_request_msg.ascii_message_array[1]);
		if(modbus_request_msg.ascii_message.lrc_check != myLRCcheck)
		{
			return WRONG_LRC;
		}

		// Check for end bytes
		if(buffer[15] == CR_CODE)
		{
			modbus_request_msg.ascii_message.trailer.CR = buffer[15];
			if(buffer[16] == LF_CODE)
			{
				modbus_request_msg.ascii_message.trailer.LF = buffer[16];
				return HAL_OK;
			}
			else
			{
				return NO_LF_CODE;
			}
		}
		else
		{
			return NO_CR_CODE;
		}
	}

	return HAL_ERROR;
}
// Check the request
uint8_t lrc_generation(uint32_t size, uint8_t* byte_array)
{
	uint32_t LRC = 0;
	//Add all bytes in the message, excluding the starting 'colon' and ending CRLF
	for(int i = 0; i < size; i++)
	{
		LRC += byte_array[i];
	}
	LRC = -(LRC - 0xFF) + 1;
	return (uint8_t) LRC;
}

// Create Error reply

// Create normal reply

// Send reply
uint32_t send_modbus_response(UART_HandleTypeDef* uart, reqest_info_t * req)
{
	if(req->quantity > 0)
	{
		uint8_t ascii_buffer[11 + req->quantity*4];
		/*** Get Data from Modbus table to a buffer ***/
		uint8_t buffer[req->quantity];
		int m = 0;
		for(int n = req->starting_address; n < req->quantity; n++)
		{
			buffer[m] = MODBUS_Table.modbus_table[n];
			m++;
		}
		/*** Start forming ASCII message ***/
		ascii_buffer[0] = START_CODE;
		HexASCII2(req->slave_address, &ascii_buffer[1]);
		HexASCII2(req->func_code, &ascii_buffer[3]);
		HexASCII2(req->quantity*2, &ascii_buffer[5]);
		int j = 7;
		for(int i = req->starting_address; i < req->quantity; i++)
		{

			ascii_buffer[j] = '0';
			j++;
			ascii_buffer[j] = '0';
			j++;
			HexASCII2(MODBUS_Table.modbus_table[i], &ascii_buffer[j]);
			j += 2;
		}
//		/*** LRC Calculation ***/
		uint8_t LRC = 0;
		LRC = req->slave_address;
		LRC += req->func_code;
		LRC += req->quantity*2;
		for(int i = 0; i < req->quantity; i++)
		{
			LRC += buffer[i];
		}
		LRC = -(LRC - 0xFF) + 1;
		LRC = (uint8_t) LRC;
		/*********************/
		HexASCII2(LRC, &ascii_buffer[j]);
		j += 2;
		ascii_buffer[j] = CR_CODE;
		j++;
		ascii_buffer[j] = LF_CODE;
		/*** End of forming ASCII message ***/

		/*** Turn on the RS485 transsceiver ***/
		HAL_GPIO_WritePin(U1DE_GPIO_Port, U1DE_Pin, 1);
		/*** Transmit message ***/
		HAL_UART_Transmit(uart, ascii_buffer, sizeof(ascii_buffer), 10);
		/*** Turn on the RS485 transsceiver ***/
		__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
		HAL_GPIO_WritePin(U1DE_GPIO_Port, U1DE_Pin, 0);
		return HAL_OK;
	}
	else
	{
		return BAD_QUANTITY_IN_RESPONSE;
	}
}

// Working with ascii
ascii_hex_code_t HexASCII (uint8_t hex_value)
{
	ascii_hex_code_t hex_ascii_conv_val = {0};
	hex_ascii_conv_val.first_symbol = ASCII_Table[(hex_value & 0xF0)>>4];		// Chop first 4 bits
	hex_ascii_conv_val.second_symbol = ASCII_Table[hex_value & 0x0F];			// Chop second 4 bits
	return hex_ascii_conv_val;
}

void HexASCII2(uint8_t hex_value, uint8_t * ascii_buffer)
{
	*(ascii_buffer+1) = ASCII_Table[(hex_value & 0x0F)];
	*(ascii_buffer) = ASCII_Table[(hex_value & 0xF0)>>4];
}

uint8_t ASCIIHex (ascii_hex_code_t * ascii_value)
{
	uint8_t ascii_hex_conv_val = 0;

	if (ascii_value->first_symbol >= 'A')	// Check the higher 8-bit register value
	{
		ascii_hex_conv_val = (ascii_value->first_symbol - 0x37)<<4;
	}
	else
	{
		ascii_hex_conv_val = (ascii_value->first_symbol - 0x30)<<4;
	}
	if (ascii_value->second_symbol >= 'A')		// Check the lower 8-bit register value
	{
		ascii_hex_conv_val |= (ascii_value->second_symbol - 0x37);
	}
	else
	{
		ascii_hex_conv_val |= (ascii_value->second_symbol - 0x30);
	}
	return ascii_hex_conv_val;
}

