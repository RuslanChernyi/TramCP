/*
 * canfd_stm_config.c
 *
 *  Created on: 5 июн. 2022 г.
 *      Author: Kimo
 */

#include "main.h"
#include "canfd_stm.h"
#include "canfd_stm_config.h"

#include "drv_canfdspi_defines.h"
#include "drv_canfdspi_register.h"

extern UsedFIFOs canfd1_fifos;

void canfd_initUsedFIFOs(UsedFIFOs * used_fifos, spiCAN * spican)
{
	// Configure Receive Fifos
	used_fifos->ReceiveFIFOs.two.FIFOType= RECEIVE_FIFO;
	used_fifos->ReceiveFIFOs.two.FIFOsFilter.Filter_number = CAN_FILTER0;
	used_fifos->ReceiveFIFOs.two.FIFOsFilter.SID_value = 0xF;
	used_fifos->ReceiveFIFOs.two.FIFOsFilter.Mask_value = 0x7FF;
	used_fifos->ReceiveFIFOs.two.FIFOx = CAN_FIFO_CH2;
	canfd_configure_asReceiveFIFO(used_fifos->ReceiveFIFOs.two.FIFOx, &used_fifos->ReceiveFIFOs.two.FIFOxCON, spican);

	used_fifos->ReceiveFIFOs.three.FIFOType= RECEIVE_FIFO;
	used_fifos->ReceiveFIFOs.three.FIFOsFilter.Filter_number = CAN_FILTER1;
	used_fifos->ReceiveFIFOs.three.FIFOsFilter.SID_value = 0x10;
	used_fifos->ReceiveFIFOs.three.FIFOsFilter.Mask_value = 0x7FF;
	used_fifos->ReceiveFIFOs.three.FIFOx = CAN_FIFO_CH3;
	canfd_configure_asReceiveFIFO(used_fifos->ReceiveFIFOs.three.FIFOx, &used_fifos->ReceiveFIFOs.three.FIFOxCON, spican);

	used_fifos->ReceiveFIFOs.four.FIFOType= RECEIVE_FIFO;
	used_fifos->ReceiveFIFOs.four.FIFOsFilter.Filter_number = CAN_FILTER2;
	used_fifos->ReceiveFIFOs.four.FIFOsFilter.SID_value = 0x11;
	used_fifos->ReceiveFIFOs.four.FIFOsFilter.Mask_value = 0x7FF;
	used_fifos->ReceiveFIFOs.four.FIFOx = CAN_FIFO_CH4;
	canfd_configure_asReceiveFIFO(used_fifos->ReceiveFIFOs.four.FIFOx, &used_fifos->ReceiveFIFOs.four.FIFOxCON, spican);

	used_fifos->ReceiveFIFOs.five.FIFOType= RECEIVE_FIFO;
	used_fifos->ReceiveFIFOs.five.FIFOsFilter.Filter_number = CAN_FILTER3;
	used_fifos->ReceiveFIFOs.five.FIFOsFilter.SID_value = 0x14;
	used_fifos->ReceiveFIFOs.five.FIFOsFilter.Mask_value = 0x7FF;
	used_fifos->ReceiveFIFOs.five.FIFOx = CAN_FIFO_CH5;
	canfd_configure_asReceiveFIFO(used_fifos->ReceiveFIFOs.five.FIFOx, &used_fifos->ReceiveFIFOs.five.FIFOxCON, spican);

	used_fifos->ReceiveFIFOs.six.FIFOType= RECEIVE_FIFO;
	used_fifos->ReceiveFIFOs.six.FIFOsFilter.Filter_number = CAN_FILTER4;
	used_fifos->ReceiveFIFOs.six.FIFOsFilter.SID_value = 0x15;
	used_fifos->ReceiveFIFOs.six.FIFOsFilter.Mask_value = 0x7FF;
	used_fifos->ReceiveFIFOs.six.FIFOx = CAN_FIFO_CH6;
	canfd_configure_asReceiveFIFO(used_fifos->ReceiveFIFOs.six.FIFOx, &used_fifos->ReceiveFIFOs.six.FIFOxCON, spican);

	used_fifos->ReceiveFIFOs.seven.FIFOType= RECEIVE_FIFO;
	used_fifos->ReceiveFIFOs.seven.FIFOsFilter.Filter_number = CAN_FILTER5;
	used_fifos->ReceiveFIFOs.seven.FIFOsFilter.SID_value = 0x16;
	used_fifos->ReceiveFIFOs.seven.FIFOsFilter.Mask_value = 0x7FF;
	used_fifos->ReceiveFIFOs.seven.FIFOx = CAN_FIFO_CH7;
	canfd_configure_asReceiveFIFO(used_fifos->ReceiveFIFOs.seven.FIFOx, &used_fifos->ReceiveFIFOs.seven.FIFOxCON, spican);

	used_fifos->ReceiveFIFOs.eight.FIFOType= RECEIVE_FIFO;
	used_fifos->ReceiveFIFOs.eight.FIFOsFilter.Filter_number = CAN_FILTER6;
	used_fifos->ReceiveFIFOs.eight.FIFOsFilter.SID_value = 0x17;
	used_fifos->ReceiveFIFOs.eight.FIFOsFilter.Mask_value = 0x7FF;
	used_fifos->ReceiveFIFOs.eight.FIFOx = CAN_FIFO_CH8;
	canfd_configure_asReceiveFIFO(used_fifos->ReceiveFIFOs.eight.FIFOx, &used_fifos->ReceiveFIFOs.eight.FIFOxCON, spican);

	canfd_configure_Filters(used_fifos, spican);

	// Configure Transmit Fifos
	canfd_configure_TransmitEventFIFO(&used_fifos->TransmitFIFOs.TEF.TEFCON, spican);
	canfd_configure_TransmitQueue(&used_fifos->TransmitFIFOs.TQ.TEFCON, spican);
	used_fifos->TransmitFIFOs.one.FIFOType= RECEIVE_FIFO;
	used_fifos->TransmitFIFOs.one.FIFOsFilter.Filter_number = -1;
	used_fifos->TransmitFIFOs.one.FIFOsFilter.SID_value = -1;
	used_fifos->TransmitFIFOs.one.FIFOsFilter.Mask_value = -1;
	used_fifos->TransmitFIFOs.one.FIFOx = CAN_FIFO_CH1;
	canfd_configure_asTransmitFIFO(used_fifos->TransmitFIFOs.one.FIFOx, &used_fifos->TransmitFIFOs.one.FIFOxCON, spican);
}

void canfd_configure(UsedFIFOs * usedFIFO, spiCAN * spican)
{
	// Go to configuration mode
	spican_writeByte(cREGADDR_CiCON+3, 0x4, spican);
	canfd_reset(spican);
	canfd_configure_OSC(spican);
	canfd_configure_IO_INT(spican);
	canfd_configure_CiCON(spican);
	canfd_configure_Timings(spican);
	canfd_configure_Interrupts(spican);
	canfd_initUsedFIFOs(usedFIFO, spican);
	canfd_RAMInit(spican);
	// Go to Classic CAN mode
	spican_writeByte(cREGADDR_CiCON+3, CAN_CLASSIC_MODE, spican);	// CAN_EXTERNAL_LOOPBACK_MODE | CAN_CLASSIC_MODE
}

void canfd_configure_Filters(UsedFIFOs * usedFIFO, spiCAN * spican)
{
	// Disable Filters
	canfd_disable_Filters(spican);
	// Configure and enable filters
	canfd_configure_Filter(usedFIFO->ReceiveFIFOs.two.FIFOx, usedFIFO->ReceiveFIFOs.two.FIFOsFilter.Filter_number, usedFIFO->ReceiveFIFOs.two.FIFOsFilter.SID_value, usedFIFO->ReceiveFIFOs.two.FIFOsFilter.Mask_value, spican);
	canfd_configure_Filter(usedFIFO->ReceiveFIFOs.three.FIFOx, usedFIFO->ReceiveFIFOs.three.FIFOsFilter.Filter_number, usedFIFO->ReceiveFIFOs.three.FIFOsFilter.SID_value, usedFIFO->ReceiveFIFOs.three.FIFOsFilter.Mask_value, spican);
	canfd_configure_Filter(usedFIFO->ReceiveFIFOs.four.FIFOx, usedFIFO->ReceiveFIFOs.four.FIFOsFilter.Filter_number, usedFIFO->ReceiveFIFOs.four.FIFOsFilter.SID_value, usedFIFO->ReceiveFIFOs.four.FIFOsFilter.Mask_value, spican);
	canfd_configure_Filter(usedFIFO->ReceiveFIFOs.five.FIFOx, usedFIFO->ReceiveFIFOs.five.FIFOsFilter.Filter_number, usedFIFO->ReceiveFIFOs.five.FIFOsFilter.SID_value, usedFIFO->ReceiveFIFOs.five.FIFOsFilter.Mask_value, spican);
	canfd_configure_Filter(usedFIFO->ReceiveFIFOs.six.FIFOx, usedFIFO->ReceiveFIFOs.six.FIFOsFilter.Filter_number, usedFIFO->ReceiveFIFOs.six.FIFOsFilter.SID_value, usedFIFO->ReceiveFIFOs.six.FIFOsFilter.Mask_value, spican);
	canfd_configure_Filter(usedFIFO->ReceiveFIFOs.seven.FIFOx, usedFIFO->ReceiveFIFOs.seven.FIFOsFilter.Filter_number, usedFIFO->ReceiveFIFOs.seven.FIFOsFilter.SID_value, usedFIFO->ReceiveFIFOs.seven.FIFOsFilter.Mask_value, spican);
	canfd_configure_Filter(usedFIFO->ReceiveFIFOs.eight.FIFOx, usedFIFO->ReceiveFIFOs.eight.FIFOsFilter.Filter_number, usedFIFO->ReceiveFIFOs.eight.FIFOsFilter.SID_value, usedFIFO->ReceiveFIFOs.eight.FIFOsFilter.Mask_value, spican);
//
//	CAN_FILTEROBJ_ID filterID_2;
//	CAN_FILTEROBJ_ID filterID_3;
//	CAN_FILTEROBJ_ID filterID_4;
//	CAN_FILTEROBJ_ID filterID_5;
//	CAN_FILTEROBJ_ID filterID_6;
//	CAN_FILTEROBJ_ID filterID_7;
//	CAN_FILTEROBJ_ID filterID_8;
//
//	filterID_2.EXIDE = 0;
//	filterID_2.SID = 0xF;
//	filterID_2.EID = 0;
//	filterID_2.SID11 = 0;
//
//	filterID_3.EXIDE = 0;
//	filterID_3.SID = 0x10;
//	filterID_3.EID = 0;
//	filterID_3.SID11 = 0;
//
//	filterID_4.EXIDE = 0;
//	filterID_4.SID = 0x11;
//	filterID_4.EID = 0;
//	filterID_4.SID11 = 0;
//
//	filterID_5.EXIDE = 0;
//	filterID_5.SID = 0x14;
//	filterID_5.EID = 0;
//	filterID_5.SID11 = 0;
//
//	filterID_6.EXIDE = 0;
//	filterID_6.SID = 0x15;
//	filterID_6.EID = 0;
//	filterID_6.SID11 = 0;
//
//	filterID_7.EXIDE = 0;
//	filterID_7.SID = 0x16;
//	filterID_7.EID = 0;
//	filterID_7.SID11 = 0;
//
//	filterID_8.EXIDE = 0;
//	filterID_8.SID = 0x17;
//	filterID_8.EID = 0;
//	filterID_8.SID11 = 0;
//
//	canfd_configure_FilterObjectX(CAN_FILTER0, &filterID_2, spican);
//	canfd_configure_FilterObjectX(CAN_FILTER1, &filterID_3, spican);
//	canfd_configure_FilterObjectX(CAN_FILTER2, &filterID_4, spican);
//	canfd_configure_FilterObjectX(CAN_FILTER3, &filterID_5, spican);
//	canfd_configure_FilterObjectX(CAN_FILTER4, &filterID_6, spican);
//	canfd_configure_FilterObjectX(CAN_FILTER5, &filterID_7, spican);
//	canfd_configure_FilterObjectX(CAN_FILTER6, &filterID_8, spican);
//
//
//	CAN_MASKOBJ_ID filterMask_2;
//	CAN_MASKOBJ_ID filterMask_3;
//	CAN_MASKOBJ_ID filterMask_4;
//	CAN_MASKOBJ_ID filterMask_5;
//	CAN_MASKOBJ_ID filterMask_6;
//	CAN_MASKOBJ_ID filterMask_7;
//	CAN_MASKOBJ_ID filterMask_8;
//
//	filterMask_2.MIDE = 1;
//	filterMask_2.MSID11 = 1;
//	filterMask_2.MEID = 0x3FFFF;
//	filterMask_2.MSID = 0x7FF;
//
//	filterMask_3.MIDE = 1;
//	filterMask_3.MSID11 = 0;
//	filterMask_3.MEID = 0x3FFFF;
//	filterMask_3.MSID = 0x7FF;
//
//	filterMask_4.MIDE = 1;
//	filterMask_4.MSID11 = 0;
//	filterMask_4.MEID = 0x3FFFF;
//	filterMask_4.MSID = 0x7FF;
//
//	filterMask_5.MIDE = 1;
//	filterMask_5.MSID11 = 0;
//	filterMask_5.MEID = 0x3FFFF;
//	filterMask_5.MSID = 0x7FF;
//
//	filterMask_6.MIDE = 1;
//	filterMask_6.MSID11 = 0;
//	filterMask_6.MEID = 0x3FFFF;
//	filterMask_6.MSID = 0x7FF;
//
//	filterMask_7.MIDE = 1;
//	filterMask_7.MSID11 = 0;
//	filterMask_7.MEID = 0x3FFFF;
//	filterMask_7.MSID = 0x7FF;
//
//	filterMask_8.MIDE = 1;
//	filterMask_8.MSID11 = 0;
//	filterMask_8.MEID = 0x3FFFF;
//	filterMask_8.MSID = 0x7FF;
//
//	canfd_configure_FilterMaskX(CAN_FILTER0, &filterMask_2, spican);
//	canfd_configure_FilterMaskX(CAN_FILTER1, &filterMask_2, spican);
//	canfd_configure_FilterMaskX(CAN_FILTER2, &filterMask_2, spican);
//	canfd_configure_FilterMaskX(CAN_FILTER3, &filterMask_2, spican);
//	canfd_configure_FilterMaskX(CAN_FILTER4, &filterMask_2, spican);
//	canfd_configure_FilterMaskX(CAN_FILTER5, &filterMask_2, spican);
//	canfd_configure_FilterMaskX(CAN_FILTER3, &filterMask_3, spican);
//	canfd_configure_FilterMaskX(CAN_FILTER4, &filterMask_4, spican);
//	canfd_configure_FilterMaskX(CAN_FILTER5, &filterMask_5, spican);
//	canfd_configure_FilterMaskX(CAN_FILTER6, &filterMask_6, spican);
//	canfd_configure_FilterMaskX(CAN_FILTER7, &filterMask_7, spican);
//	canfd_configure_FilterMaskX(CAN_FILTER8, &filterMask_8, spican);
//
//	canfd1_fifos.two.fifo_filter_number 	= CAN_FILTER0;
//	canfd1_fifos.three.fifo_filter_number 	= CAN_FILTER1;
//	canfd1_fifos.four.fifo_filter_number 	= CAN_FILTER2;
//	canfd1_fifos.five.fifo_filter_number 	= CAN_FILTER3;
//	canfd1_fifos.six.fifo_filter_number 	= CAN_FILTER4;
//	canfd1_fifos.seven.fifo_filter_number 	= CAN_FILTER5;
//	canfd1_fifos.eight.fifo_filter_number 	= CAN_FILTER6;
//
//	canfd_configure_FilterConX(FLTCON1, CAN_FIFO_CH2, CAN_FIFO_CH3, CAN_FIFO_CH4, CAN_FIFO_CH5, spican);
//	canfd_configure_FilterConX(FLTCON2, CAN_FIFO_CH6, CAN_FIFO_CH7, CAN_FIFO_CH8, 0xFF, spican);
}

void canfd_configure_Filter(CAN_FIFO_CHANNEL FIFOx, CAN_FILTER FilterX, uint16_t SID, uint16_t Mask, spiCAN * spican)
{
	CAN_FILTEROBJ_ID filter_object = {0};
	CAN_MASKOBJ_ID	 filter_mask   = {0};
	REG_CiFLTCON_BYTE filter_configuration = {0};

	filter_object.SID = SID;	// Set the Standard ID you want to receive with FIFOx
	filter_object.EXIDE = 0;	// Disable Extended ID, as we don't need it here

	filter_mask.MSID = Mask;	// Set Mask for Standard ID (if bit n is 0, it is ignored when filtering)
	filter_mask.MEID = 0;		// Set Mask for Extended ID to 0 as we don't need it
	filter_mask.MIDE = 1;		// Make filter to receive only either SIDs or EIDs (depends on EXIDE)

	canfd_configure_FilterObjectX(FilterX, &filter_object, spican);	// Write object settings into mcp
	canfd_configure_FilterMaskX	 (FilterX, &filter_mask, spican);	// Write mask settings into mcp

	/*** Enable Filter ***/
	switch(FilterX)
	{
		case CAN_FILTER0:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON0) + 0, filter_configuration.byte, spican);
			break;
		case CAN_FILTER4:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON1) + 0, filter_configuration.byte, spican);
			break;
		case CAN_FILTER8:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON2) + 0, filter_configuration.byte, spican);
			break;
		case CAN_FILTER12:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON3) + 0, filter_configuration.byte, spican);
			break;
		case CAN_FILTER16:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON4) + 0, filter_configuration.byte, spican);
			break;
		case CAN_FILTER20:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON5) + 0, filter_configuration.byte, spican);
			break;
		case CAN_FILTER24:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON6) + 0, filter_configuration.byte, spican);
			break;
		case CAN_FILTER28:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON7) + 0, filter_configuration.byte, spican);
			break;

		case CAN_FILTER1:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON0) + 1, filter_configuration.byte, spican);
			break;
		case CAN_FILTER5:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON1) + 1, filter_configuration.byte, spican);
			break;
		case CAN_FILTER9:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON2) + 1, filter_configuration.byte, spican);
			break;
		case CAN_FILTER13:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON3) + 1, filter_configuration.byte, spican);
			break;
		case CAN_FILTER17:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON4) + 1, filter_configuration.byte, spican);
			break;
		case CAN_FILTER21:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON5) + 1, filter_configuration.byte, spican);
			break;
		case CAN_FILTER25:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON6) + 1, filter_configuration.byte, spican);
			break;
		case CAN_FILTER29:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON7) + 1, filter_configuration.byte, spican);
			break;

		case CAN_FILTER2:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON0) + 2, filter_configuration.byte, spican);
			break;
		case CAN_FILTER6:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON1) + 2, filter_configuration.byte, spican);
			break;
		case CAN_FILTER10:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON2) + 2, filter_configuration.byte, spican);
			break;
		case CAN_FILTER14:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON3) + 2, filter_configuration.byte, spican);
			break;
		case CAN_FILTER18:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON4) + 2, filter_configuration.byte, spican);
			break;
		case CAN_FILTER22:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON5) + 2, filter_configuration.byte, spican);
			break;
		case CAN_FILTER26:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON6) + 2, filter_configuration.byte, spican);
			break;
		case CAN_FILTER30:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON7) + 2, filter_configuration.byte, spican);
			break;

		case CAN_FILTER3:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON0) + 3, filter_configuration.byte, spican);
			break;
		case CAN_FILTER7:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON1) + 3, filter_configuration.byte, spican);
			break;
		case CAN_FILTER11:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON2) + 3, filter_configuration.byte, spican);
			break;
		case CAN_FILTER15:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON3) + 3, filter_configuration.byte, spican);
			break;
		case CAN_FILTER19:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON4) + 3, filter_configuration.byte, spican);
			break;
		case CAN_FILTER23:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON5) + 3, filter_configuration.byte, spican);
			break;
		case CAN_FILTER27:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON6) + 3, filter_configuration.byte, spican);
			break;
		case CAN_FILTER31:
			filter_configuration.bF.BufferPointer = FIFOx;
			filter_configuration.bF.Enable = 1;
			spican_writeByte(cREGADDR_CiFLTCON + (4 * FLTCON7) + 3, filter_configuration.byte, spican);
			break;
	}
}

void canfd_configure_OSC(spiCAN * spican)
{
	REG_OSC osc = {0};
	osc.bF.PllEnable = 0;
	osc.bF.SCLKDIV = 0;
	osc.bF.CLKODIV = 0x3;
	spican_write32bitReg(cREGADDR_OSC, osc.byte, spican);
}

void canfd_configure_IO_INT(spiCAN * spican)
{
	REG_IOCON iocon = {0};
	iocon.bF.XcrSTBYEnable = 0;
	iocon.bF.PinMode0 = 0;
	iocon.bF.PinMode1 = 0;
	iocon.bF.SOFOutputEnable = 0;
	iocon.bF.INTPinOpenDrain = 0;

	spican_write32bitReg(cREGADDR_IOCON, iocon.byte, spican);
}

void canfd_configure_CiCON(spiCAN * spican)
{
	REG_CiCON cicon = {0};
	cicon.bF.DNetFilterCount = CAN_DNET_FILTER_DISABLE;
	cicon.bF.IsoCrcEnable = 1;
	cicon.bF.ProtocolExceptionEventDisable = 1;
	cicon.bF.WakeUpFilterEnable = 0;
	cicon.bF.WakeUpFilterTime = 0;
	cicon.bF.BitRateSwitchDisable = 1;
	cicon.bF.RestrictReTxAttempts = 1;
	cicon.bF.EsiInGatewayMode = 0;
	cicon.bF.SystemErrorToListenOnly = 1;
	cicon.bF.StoreInTEF = 0;
	cicon.bF.TXQEnable = 0;
	cicon.bF.RequestOpMode = 0x4;
	cicon.bF.AbortAllTx = 0;
	cicon.bF.TxBandWidthSharing = 0;

	spican_write32bitReg(cREGADDR_CiCON, cicon.byte, spican);
}

void canfd_configure_Timings(spiCAN * spican)
{
	REG_CiNBTCFG nom_bit_time_con = {0};
	REG_CiDBTCFG data_bit_time_con = {0};
	REG_CiTDC transmit_delay_compensation = {0};

	nom_bit_time_con.bF.BRP = 0;
	nom_bit_time_con.bF.TSEG1 = 62;
	nom_bit_time_con.bF.TSEG2 = 15;
	nom_bit_time_con.bF.SJW = 15;

//	data_bit_time_con.bF.BRP = 0;
//	data_bit_time_con.bF.TSEG1 = 62;
//	data_bit_time_con.bF.TSEG2 = 15;
//	data_bit_time_con.bF.SJW = 15;

	transmit_delay_compensation.bF.TDCOffset = 63;
	transmit_delay_compensation.bF.TDCMode = 0x2;

	spican_write32bitReg(cREGADDR_CiNBTCFG, nom_bit_time_con.byte, spican);
	spican_write32bitReg(cREGADDR_CiDBTCFG, data_bit_time_con.byte, spican);
	spican_write32bitReg(cREGADDR_CiTDC, transmit_delay_compensation.byte, spican);
}

void canfd_configure_Interrupts(spiCAN * spican)
{
	REG_CiINT ciint = {0};

	ciint.bF.IE.TXIE = 1;
	ciint.bF.IE.RXIE = 1;
	ciint.bF.IE.TBCIE = 1;
	ciint.bF.IE.MODIE = 1;
	ciint.bF.IE.TEFIE = 1;
	ciint.bF.IE.ECCIE = 1;
	ciint.bF.IE.SPICRCIE = 1;
	ciint.bF.IE.TXATIE = 1;
	ciint.bF.IE.RXOVIE = 1;
	ciint.bF.IE.SERRIE = 1;
	ciint.bF.IE.CERRIE = 1;
	ciint.bF.IE.WAKIE = 1;
	ciint.bF.IE.IVMIE = 1;

	spican_write32bitReg(cREGADDR_CiINT, ciint.byte, spican);
}

void canfd_configure_TransmitEventFIFO(REG_CiTEFCON * citefcon, spiCAN * spican)
{
	citefcon->bF.TEFNEIE = 0;
	citefcon->bF.TEFHFIE = 0;
	citefcon->bF.TEFHFIE = 0;
	citefcon->bF.TEFOVIE = 0;
	citefcon->bF.TimeStampEnable = 0;
	citefcon->bF.UINC = 0;
	citefcon->bF.FRESET = 0;
	citefcon->bF.FifoSize = 0x0;

	spican_write32bitReg(cREGADDR_CiTEFCON, citefcon->byte, spican);
}

void canfd_configure_TransmitQueue(REG_CiTXQCON * citxqcon, spiCAN * spican)
{
	citxqcon->txBF.TxNotFullIE = 1;
	citxqcon->txBF.TxEmptyIE = 1;
	citxqcon->txBF.TxAttemptIE = 1;
	citxqcon->txBF.TxEnable = 1;
	citxqcon->txBF.UINC = 0;
	citxqcon->txBF.TxRequest = 0;
	citxqcon->txBF.FRESET = 0;
	citxqcon->txBF.TxPriority = 0;
	citxqcon->txBF.TxAttempts = 0x1;
	citxqcon->txBF.FifoSize = 0x0;
	citxqcon->txBF.PayLoadSize = 0;

	spican_write32bitReg(cREGADDR_CiTXQCON, citxqcon->byte, spican);
}

void canfd_configure_asReceiveFIFO(uint32_t FIFOx, REG_CiFIFOCON * fifocon, spiCAN * spican)
{
	fifocon->rxBF.RxNotEmptyIE = 1;
	fifocon->rxBF.RxHalfFullIE = 1;
	fifocon->rxBF.RxFullIE = 1;
	fifocon->rxBF.RxOverFlowIE = 1;
	fifocon->rxBF.RxTimeStampEnable = 0;
	fifocon->rxBF.UINC = 0;
	fifocon->rxBF.TxEnable = 0;
	fifocon->rxBF.FRESET = 0;
	fifocon->rxBF.FifoSize = 0x0;
	fifocon->rxBF.PayLoadSize = 0;

	uint32_t FIFOctrl_address = cREGADDR_CiFIFOCON + (CiFIFO_OFFSET * FIFOx);
	spican_write32bitReg(FIFOctrl_address, fifocon->byte, spican);
	spican_read32bitReg_withDMA(FIFOctrl_address, fifocon->byte, spican);
}

void canfd_configure_asTransmitFIFO(uint32_t FIFOx, REG_CiFIFOCON * fifocon, spiCAN * spican)
{
	fifocon->txBF.TxNotFullIE = 1;
	fifocon->txBF.TxHalfFullIE = 1;
	fifocon->txBF.TxEmptyIE = 1;
	fifocon->txBF.TxAttemptIE = 1;
	fifocon->txBF.RTREnable = 0;
	fifocon->txBF.TxRequest = 0;
	fifocon->txBF.TxPriority = 0;
	fifocon->txBF.TxAttempts = 1;
	fifocon->txBF.UINC = 0;
	fifocon->txBF.TxEnable = 1;
	fifocon->txBF.FRESET = 0;
	fifocon->txBF.FifoSize = 0x0;
	fifocon->txBF.PayLoadSize = 0;

	uint32_t FIFOctrl_address = cREGADDR_CiFIFOCON + (CiFIFO_OFFSET * FIFOx);
	spican_write32bitReg(FIFOctrl_address, fifocon->byte, spican);
	spican_read32bitReg_withDMA(FIFOctrl_address, fifocon->byte, spican);
}

void canfd_disable_Filters(spiCAN * spican)
{
	uint8_t zero_array[28] = {0};
	spican_write8bitArray(cREGADDR_CiFLTCON, zero_array, sizeof(zero_array), spican);

}

//void canfd_configure_FilterConX(uint32_t FilterConX, uint32_t FIFO_for_filter0, uint32_t FIFO_for_filter1, uint32_t FIFO_for_filter2, uint32_t FIFO_for_filter3, spiCAN * spican)
//{
//	REG_CiFLTCON filter = {0};
//	if(FIFO_for_filter0 > 31)
//	{
//		filter.bF.FLTEN0 = 0;
//	}
//	else
//	{
//		filter.bF.F0BP = FIFO_for_filter0;
//		filter.bF.FLTEN0 = 1;
//	}
//
//	if(FIFO_for_filter1 > 31)
//	{
//		filter.bF.FLTEN1 = 0;
//	}
//	else
//	{
//		filter.bF.F1BP = FIFO_for_filter1;
//		filter.bF.FLTEN1 = 1;
//	}
//
//	if(FIFO_for_filter2 > 31)
//	{
//		filter.bF.FLTEN2 = 0;
//	}
//	else
//	{
//		filter.bF.F2BP = FIFO_for_filter2;
//		filter.bF.FLTEN2 = 1;
//	}
//
//	if(FIFO_for_filter3 > 31)
//	{
//		filter.bF.FLTEN3 = 0;
//	}
//	else
//	{
//		filter.bF.F3BP = FIFO_for_filter3;
//		filter.bF.FLTEN3 = 1;
//	}
//
//	spican_write32bitReg(cREGADDR_CiFLTCON + (4 * FilterConX), filter.byte, spican);
//	spican_read32bitReg_withDMA(cREGADDR_CiFLTCON + (4 * FilterConX), filter.byte, spican);
//}

void canfd_configure_FilterObjectX(uint32_t FilterX, CAN_FILTEROBJ_ID * filterId, spiCAN * spican)
{
	REG_CiFLTOBJ cifltobj = {0};

	cifltobj.bF.EID = filterId->EID;
	cifltobj.bF.EXIDE = filterId->EXIDE;
	cifltobj.bF.SID = filterId->SID;
	cifltobj.bF.SID11 = filterId->SID11;

	spican_write32bitReg(cREGADDR_CiFLTOBJ + (CiFILTER_OFFSET * FilterX), cifltobj.byte, spican);
	spican_read32bitReg_withDMA(cREGADDR_CiFLTOBJ + (CiFILTER_OFFSET * FilterX), cifltobj.byte, spican);
	return;
}

void canfd_configure_FilterMaskX(uint32_t FilterX, CAN_MASKOBJ_ID * filterMask, spiCAN * spican)
{
	REG_CiMASK cimask = {0};

	cimask.bF.MEID = filterMask->MEID;
	cimask.bF.MIDE = filterMask->MIDE;
	cimask.bF.MSID = filterMask->MSID;
	cimask.bF.MSID11 = filterMask->MSID11;

	spican_write32bitReg(cREGADDR_CiMASK + (CiFILTER_OFFSET * FilterX), cimask.byte, spican);
}
