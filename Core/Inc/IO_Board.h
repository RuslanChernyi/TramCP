#ifndef IOBOARD_H_
#define IOBOARD_H_

#include "main.h"
#include "spi.h"
#include "can.h"

#define IOBOARD_N0	0

typedef enum ADCs
{
	CH2 = 2,
	CH3 = 3,
	CH4 = 4,
	CH5 = 5,
	CH7 = 7,
	CH14 = 14,
	CH15 = 15
}ADCs_t;

typedef enum XAs
{
	XA1 = 1,
	XA2
}XAs_t;


typedef struct IOboard_request
{
	uint8_t myboard_addr;
	uint8_t cmd;
	uint8_t element;
}IOboard_request_t;

typedef struct IOBoard
{
	uint32_t BoardNr;
	CAN_HandleTypeDef * hcan;
	uint32_t lastCommand;
	uint32_t currentCommand;
	uint32_t nextCommand;
	uint8_t RxBuffer[8];
	IOboard_request_t requestedData;
}IOBoard_t;

void IOboard1Init(void);
void IOboard2Init(void);
void IOboard3Init(void);
void IOboard4Init(void);


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

uint32_t CIO(IOBoard_t * IOBoard);
uint32_t get_response(IOBoard_t * IOBoard);
uint32_t RequestIO(IOBoard_t * IOBoard);
void place_into_table(IOBoard_t * IOBoard);



#endif /* IOBOARD_H_ */
