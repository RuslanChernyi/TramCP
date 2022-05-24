#ifndef DRIVEBOARD_H_
#define DRIVEBOARD_H_

#include "main.h"
// Packet Discription

uint32_t askPacket(CAN_HandleTypeDef * hcan, uint8_t packet_to_ask);
void placeIntoTable(void);
uint32_t receivePacket(CAN_HandleTypeDef * hcan);
uint32_t CDR(void);


#endif	/* DRIVEBOARD_H_ */
