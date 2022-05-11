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

void askIO(CAN_HandleTypeDef * hcan, IOboard_request_t * req);

uint32_t receiveFromIO(CAN_HandleTypeDef * hcan, uint8_t * receivedData, uint32_t boardID);
void placeIntoTableIO(uint8_t * receivedData, IOboard_request_t * req);
void IOboard_request(CAN_HandleTypeDef * hcan, uint32_t boardNumber, uint32_t cmd, IOboard_request_t * req);
void getADC(IOBoard_t * IOBoard,uint32_t adc_ch);

#endif /* IOBOARD_H_ */
