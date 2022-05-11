#ifndef DRIVEBOARD_H_
#define DRIVEBOARD_H_

#include "main.h"
// Packet Discription

uint32_t askPacket(CAN_HandleTypeDef * hcan);
void placeIntoTable(void);
uint32_t receivePacket(CAN_HandleTypeDef * hcan);

#endif	/* DRIVEBOARD_H_ */
