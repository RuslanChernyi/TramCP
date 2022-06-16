/*
 * DMA.c
 *
 *  Created on: 7 июн. 2022 г.
 *      Author: Kimo
 */
#include "main.h"
#include "DMA.h"

void DMA_SPI1RXInit(void)
{
	uint8_t dummy_buffer[4];
	// DMA configuration for SPI1_Rx
	// Enable DMA2 clocking
	RCC->AHB1ENR |= (1U<<22);
	// Disable the stream
	DMA2_Stream0->CR &= ~(1U<<0);	// Disable the stream0
	while((DMA2_Stream0->CR & (1U<<0))) // Check that stream is disabled, if not disable again
	{
		DMA2_Stream0->CR &= ~(1U<<0);	// Disable the stream0
	}
	// Clear dedicated stream status bits

	// Set peripheral port register address in the DMA_SxPAR register.
	DMA2_Stream0->PAR = (uint32_t)&(SPI1->DR);
	// Set memory address in the DMA_SxMA0R register.
	DMA2_Stream0->M0AR =(uint32_t)dummy_buffer;
	// Configure number of items to be transferred in the DMA_SxNDTR register.
	DMA2_Stream0->NDTR = 1;
	//Select channel for a stream
	DMA2_Stream0->CR |= 0x3<<25;	// Select channel 3 for stream 0 by writing to CHSEL[2:0]
	// Set a peripheral controller in DMA_CR register
	DMA2_Stream0->CR |= (1U<<5);	// Set a PFCTRL bit to enable peripheral flow controller
	// Configure stream priority
	DMA2_Stream0->CR |= (0x2U<<16); // Select high priority for this stream
	// Select direction for a stream
	DMA2_Stream0->CR &= ~(0x3<<6);	// Direction: Peripheral-to-memory. Select by writing into DIR[1:0]
	// Enable memory increment
	DMA2_Stream0->CR |= (1U<<10); // Set MINC
	// Configure memory data size
	DMA2_Stream0->CR &= ~(0x3<<13); // Set memory size to byte(8-bit)
	// Enable SPI1 DMA controller
	SPI1->CR2 |= (1U<<0);	// Rx Buffer DMA enable
}

void DMA_SPI2RXInit(void)
{
	uint8_t dummy_buffer[4];
	// DMA configuration for SPI1_Rx
	// Enable DMA1 clocking
	RCC->AHB1ENR |= (1U<<21);	//!!! CHange Clocking for DMA1
	// Disable the stream
	DMA1_Stream3->CR &= ~(1U<<0);	// Disable the stream3
	while((DMA1_Stream3->CR & (1U<<0))) // Check that stream is disabled, if not disable again
	{
		DMA1_Stream3->CR &= ~(1U<<0);	// Disable the stream0
	}
	// Clear dedicated stream status bits

	// Set peripheral port register address in the DMA_SxPAR register.
	DMA1_Stream3->PAR = (uint32_t)&(SPI2->DR);
	// Set memory address in the DMA_SxMA0R register.
	DMA1_Stream3->M0AR =(uint32_t)dummy_buffer;
	// Configure number of items to be transferred in the DMA_SxNDTR register.
	DMA1_Stream3->NDTR = 1;
	//Select channel for a stream
	DMA1_Stream3->CR |= 0x0<<25;	// Select channel 0 for stream 3 by writing to CHSEL[2:0]
	// Set a peripheral controller in DMA_CR register
	DMA1_Stream3->CR |= (1U<<5);	// Set a PFCTRL bit to enable peripheral flow controller
	// Configure stream priority
	DMA1_Stream3->CR |= (0x2U<<16); // Select high priority for this stream
	// Select direction for a stream
	DMA1_Stream3->CR &= ~(0x3<<6);	// Direction: Peripheral-to-memory. Select by writing into DIR[1:0]
	// Enable memory increment
	DMA1_Stream3->CR |= (1U<<10); // Set MINC
	// Configure memory data size
	DMA1_Stream3->CR &= ~(0x3<<13); // Set memory size to byte(8-bit)
	// Enable SPI1 DMA controller
	SPI2->CR2 |= (1U<<0);	// Rx Buffer DMA enable
}
