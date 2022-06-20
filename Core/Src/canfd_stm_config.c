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

void canfd_configure(spiCAN * spican)
{

	// Go to configuration mode
	spican_writeByte(cREGADDR_CiCON+3, 0x4, spican);
	canfd_reset(spican);

	canfd_configure_OSC(spican);
	canfd_configure_IO_INT(spican);
	canfd_configure_CiCON(spican);
	canfd_configure_Timings(spican);
	canfd_configure_Interrupts(spican);
	canfd_configure_TransmitEventFIFO(spican);
	canfd_configure_TransmitQueue(spican);
// Configure Transmit FIFOs
	canfd_configure_asTransmitFIFO(CAN_FIFO_CH2, &canfd1_fifos.two, spican);
// Configure Receive FIFOs
	canfd_configure_asReceiveFIFO(CAN_FIFO_CH1, &canfd1_fifos.one, spican);

	canfd_configure_Filters(spican);

	canfd_RAMInit(spican);
	// Go to Classic CAN mode
	spican_writeByte(cREGADDR_CiCON+3, CAN_CLASSIC_MODE, spican);	// CAN_EXTERNAL_LOOPBACK_MODE | CAN_CLASSIC_MODE
}

void canfd_configure_Filters(spiCAN * spican)
{
	// Disable Filters
	canfd_configure_FilterConX(FLTCON1, 0xFF, 0xFF, 0xFF, 0xFF, spican);

	CAN_FILTEROBJ_ID filterID_1;
	filterID_1.EXIDE = 0;
	filterID_1.SID = 0x1;
	filterID_1.EID = 0;
	filterID_1.SID11 = 0;
	canfd_configure_FilterObjectX(CAN_FILTER1, &filterID_1, spican);
	CAN_MASKOBJ_ID filterMask_1;
	filterMask_1.MIDE = 0;
	filterMask_1.MSID11 = 1;
	filterMask_1.MEID = 0x3FFFF;
	filterMask_1.MSID = 0x7FF;
	canfd_configure_FilterMaskX(CAN_FILTER1, &filterMask_1, spican);

	canfd1_fifos.one.fifo_filter_number = CAN_FILTER1;
	canfd_configure_FilterConX(FLTCON1, CAN_FIFO_CH1, 0xFF, 0xFF, 0xFF, spican);

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

void canfd_configure_TransmitEventFIFO(spiCAN * spican)
{
	REG_CiTEFCON citefcon = {0};

	citefcon.bF.TEFNEIE = 0;
	citefcon.bF.TEFHFIE = 0;
	citefcon.bF.TEFHFIE = 0;
	citefcon.bF.TEFOVIE = 0;
	citefcon.bF.TimeStampEnable = 0;
	citefcon.bF.UINC = 0;
	citefcon.bF.FRESET = 0;
	citefcon.bF.FifoSize = 0x0;

	spican_write32bitReg(cREGADDR_CiTEFCON, citefcon.byte, spican);
}

void canfd_configure_TransmitQueue(spiCAN * spican)
{
	REG_CiTXQCON citxqcon = {0};

	citxqcon.txBF.TxNotFullIE = 1;
	citxqcon.txBF.TxEmptyIE = 1;
	citxqcon.txBF.TxAttemptIE = 1;
	citxqcon.txBF.TxEnable = 1;
	citxqcon.txBF.UINC = 0;
	citxqcon.txBF.TxRequest = 0;
	citxqcon.txBF.FRESET = 0;
	citxqcon.txBF.TxPriority = 0;
	citxqcon.txBF.TxAttempts = 0x1;
	citxqcon.txBF.FifoSize = 0x0;
	citxqcon.txBF.PayLoadSize = 0;

	spican_write32bitReg(cREGADDR_CiTXQCON, citxqcon.byte, spican);
}

void canfd_configure_asReceiveFIFO(uint32_t FIFOx, FIFO_t * fifocon, spiCAN * spican)
{
	fifocon->FIFOxCON.rxBF.RxNotEmptyIE = 1;
	fifocon->FIFOxCON.rxBF.RxHalfFullIE = 1;
	fifocon->FIFOxCON.rxBF.RxFullIE = 1;
	fifocon->FIFOxCON.rxBF.RxOverFlowIE = 1;
	fifocon->FIFOxCON.rxBF.RxTimeStampEnable = 0;
	fifocon->FIFOxCON.rxBF.UINC = 0;
	fifocon->FIFOxCON.rxBF.TxEnable = 0;
	fifocon->FIFOxCON.rxBF.FRESET = 0;
	fifocon->FIFOxCON.rxBF.FifoSize = 0x0;
	fifocon->FIFOxCON.rxBF.PayLoadSize = 0;

	fifocon->fifotype = RECEIVE_FIFO;
	fifocon->fifo_number = FIFOx;

	uint32_t FIFOctrl_address = cREGADDR_CiFIFOCON + (CiFIFO_OFFSET * FIFOx);
	spican_write32bitReg(FIFOctrl_address, fifocon->FIFOxCON.byte, spican);
	spican_read32bitReg_withDMA(FIFOctrl_address, fifocon->FIFOxCON.byte, spican);
}

void canfd_configure_asTransmitFIFO(uint32_t FIFOx, FIFO_t * fifocon, spiCAN * spican)
{
	fifocon->FIFOxCON.txBF.TxNotFullIE = 1;
	fifocon->FIFOxCON.txBF.TxHalfFullIE = 1;
	fifocon->FIFOxCON.txBF.TxEmptyIE = 1;
	fifocon->FIFOxCON.txBF.TxAttemptIE = 1;
	fifocon->FIFOxCON.txBF.RTREnable = 0;
	fifocon->FIFOxCON.txBF.TxRequest = 0;
	fifocon->FIFOxCON.txBF.TxPriority = 0;
	fifocon->FIFOxCON.txBF.TxAttempts = 1;
	fifocon->FIFOxCON.txBF.UINC = 0;
	fifocon->FIFOxCON.txBF.TxEnable = 1;
	fifocon->FIFOxCON.txBF.FRESET = 0;
	fifocon->FIFOxCON.txBF.FifoSize = 0x0;
	fifocon->FIFOxCON.txBF.PayLoadSize = 0;

	fifocon->fifotype = TRANSMIT_FIFO;
	fifocon->fifo_number = FIFOx;

	uint32_t FIFOctrl_address = cREGADDR_CiFIFOCON + (CiFIFO_OFFSET * FIFOx);
	spican_write32bitReg(FIFOctrl_address, fifocon->FIFOxCON.byte, spican);
	spican_read32bitReg_withDMA(FIFOctrl_address, fifocon->FIFOxCON.byte, spican);
}

void canfd_configure_FilterConX(uint32_t FilterConX, uint32_t FIFO_for_filter0, uint32_t FIFO_for_filter1, uint32_t FIFO_for_filter2, uint32_t FIFO_for_filter3, spiCAN * spican)
{
	REG_CiFLTCON filter = {0};
	if(FIFO_for_filter0 > 31)
	{
		filter.bF.FLTEN0 = 0;
	}
	else
	{
		filter.bF.F0BP = FIFO_for_filter0;
		filter.bF.FLTEN0 = 1;
	}

	if(FIFO_for_filter1 > 31)
	{
		filter.bF.FLTEN1 = 0;
	}
	else
	{
		filter.bF.F1BP = FIFO_for_filter1;
		filter.bF.FLTEN1 = 1;
	}

	if(FIFO_for_filter2 > 31)
	{
		filter.bF.FLTEN2 = 0;
	}
	else
	{
		filter.bF.F2BP = FIFO_for_filter2;
		filter.bF.FLTEN2 = 1;
	}

	if(FIFO_for_filter3 > 31)
	{
		filter.bF.FLTEN3 = 0;
	}
	else
	{
		filter.bF.F3BP = FIFO_for_filter3;
		filter.bF.FLTEN3 = 1;
	}

	spican_write32bitReg(cREGADDR_CiFLTCON + (4 * FilterConX), filter.byte, spican);
}

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
