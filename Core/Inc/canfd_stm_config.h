/*
 * canfd_stm_config.h
 *
 *  Created on: 5 июн. 2022 г.
 *      Author: Kimo
 */
#include "drv_canfdspi_defines.h"


#ifndef INC_CANFD_STM_CONFIG_H_
#define INC_CANFD_STM_CONFIG_H_

void canfd_configure(UsedFIFOs * usedFIFO, spiCAN * spican);
void canfd_initUsedFIFOs(UsedFIFOs * used_fifos, spiCAN * spican);

void canfd_configure_OSC(spiCAN * spican);
void canfd_configure_IO_INT(spiCAN * spican);
void canfd_configure_CiCON(spiCAN * spican);
void canfd_configure_Filters(UsedFIFOs * usedFIFO, spiCAN * spican);
void canfd_configure_Filter(CAN_FIFO_CHANNEL FIFOx, CAN_FILTER FilterX, uint16_t SID, uint16_t Mask, spiCAN * spican);
void canfd_configure_Timings(spiCAN * spican);
void canfd_configure_Interrupts(spiCAN * spican);
void canfd_configure_TransmitEventFIFO(REG_CiTEFCON * citefcon, spiCAN * spican);
void canfd_configure_TransmitQueue(REG_CiTXQCON * citxqcon, spiCAN * spican);
void canfd_configure_asReceiveFIFO(uint32_t FIFOx, REG_CiFIFOCON * fifocon, spiCAN * spican);
void canfd_configure_asTransmitFIFO(uint32_t FIFOx, REG_CiFIFOCON * fifocon, spiCAN * spican);
void canfd_disable_Filters(spiCAN * spican);

//void canfd_configure_FilterConX(uint32_t FilterConX, uint32_t FIFO_for_filter0, uint32_t FIFO_for_filter1, uint32_t FIFO_for_filter2, uint32_t FIFO_for_filter3, spiCAN * spican);
void canfd_configure_FilterObjectX(uint32_t FilterX, CAN_FILTEROBJ_ID * filterId, spiCAN * spican);
void canfd_configure_FilterMaskX(uint32_t FilterX, CAN_MASKOBJ_ID * filterMask, spiCAN * spican);




#endif /* INC_CANFD_STM_CONFIG_H_ */
