#ifndef MODBUS_ASCII_H_
#define MODBUS_ASCII_H_

#include "main.h"
#include "usart.h"
#include "signal_tables.h"

#define MAX_MODBUS_TABLE_VALUE		0x2D
#define MIN_MODBUS_TABLE_ADDRESS	0x0

// Modbus enums
typedef enum ascii_service_codes
{
	START_CODE = 0x3A,
	CR_CODE = 0x0D,
	LF_CODE = 0x0A
}ascii_service_codes_t;
typedef enum modbus_function_codes
{
	READ_COILS 				= 0x1,
	READ_DINs 				= 0x2,
	READ_HOLDING_REGs 		= 0x3,
	READ_INPUT_REGs 		= 0x4,
	WRITE_SINGLE_COIL		= 0x5,
	WRITE_SINGLE_REG		= 0x6,
	READ_EXCEPTION_STATUS 	= 0x7,
}modbus_function_codes_t;

// Modbus message struct
typedef struct ascii_hex_code
{
	uint8_t first_symbol;
	uint8_t second_symbol;
}ascii_hex_code_t;

typedef struct end_word
{
	uint8_t CR;	// Must be 0x0D (CR_CODE)
	uint8_t LF;	// Must be 0x0A (LF_CODE)
}end_word_t;
typedef struct modbus_ascii_request_msg
{
	uint8_t header;	// Must be 0x3A (START_CODE)
	uint8_t slave_address;
	uint8_t function;
	uint8_t start_addr_hi;
	uint8_t start_addr_lo;
	uint8_t quantity_hi;
	uint8_t quantity_lo;
	uint8_t lrc_check;
	end_word_t trailer;
}modbus_ascii_request_msg_struct_t;
typedef struct modbus_ascii_response_msg
{
	uint8_t header;	// Must be 0x3A (START_CODE)
	ascii_hex_code_t slave_addr;
	ascii_hex_code_t function;
	ascii_hex_code_t byte_count;
	ascii_hex_code_t lrc_check;
	end_word_t trailer;
}modbus_ascii_response_msg_struct_t;


typedef union mdb_ascii_req
{
	modbus_ascii_request_msg_struct_t ascii_message;
	uint8_t ascii_message_array[17];
}modbus_ascii_request_msg_t;

typedef struct reqest_info
{
	uint8_t slave_address;
	uint8_t func_code;
	uint16_t starting_address;
	uint16_t quantity;
}reqest_info_t;

typedef struct ascii_data
{
	ascii_hex_code_t data_hi;
	ascii_hex_code_t data_lo;
}ascii_data_t;

typedef union mdb_ascii_resp
{
	modbus_ascii_response_msg_struct_t ascii_message;
	uint8_t ascii_message_array[11];
}modbus_ascii_response_msg_t;

typedef union mdb_ascii_table
{
	impulse_drive_table_t imp_drv_table;
	io_board_table_t	io_board_table;

	uint8_t modbus_table[45];
}modbus_ascii_table_t;


uint32_t send_modbus_response(UART_HandleTypeDef* uart, reqest_info_t * req);
uint32_t receive_modbus_request(UART_HandleTypeDef* uart, reqest_info_t * req);
uint8_t lrc_generation(uint32_t size, uint8_t* byte_array);
uint32_t check_modbus_request(uint8_t * buffer,reqest_info_t * req);

ascii_hex_code_t HexASCII (uint8_t hex_value);
uint8_t ASCIIHex (ascii_hex_code_t* ascii_value);
void HexASCII2(uint8_t hex_value, uint8_t * ascii_buffer);

#endif /* MODBUS_ASCII_H_ */
