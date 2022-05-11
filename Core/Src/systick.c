/*
 * systick.c
 *
 *  Created on: 7 нояб. 2021 г.
 *      Author: Kimo
 */
#include "systick.h"

void systick_delay (int delay)
{
	/*** Configure systick ***/
	/* Reload with number of clocks per millisecond */
	SysTick->LOAD = SYSTICK_LOAD_VAL;
	/* Clear systick current value register */
	SysTick->VAL = 0;
	/* Enable systick and select internal clock source */
	SysTick->CTRL = SYSTICK_COUNTER_EN | SYSTICK_CSOURCE;

	for(int i = 0; i < delay; i++)
	{
		/* Wait until the COUTNFLAG is set */
		while(!(SysTick->CTRL & SYSTICK_COUNTFLAG) );
	}
	SysTick->CTRL = 0;

}

