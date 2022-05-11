/*
 * systick.h
 *
 *  Created on: 7 нояб. 2021 г.
 *      Author: Kimo
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "stm32f4xx.h"

#define SYSTICK_LOAD_VAL	168
/*
 * For 168MHz	in one microsecond there are 168 ticks
 */
#define SYSTICK_COUNTER_EN	(1U<<0)
#define SYSTICK_CSOURCE		(1U<<2)
#define SYSTICK_COUNTFLAG	(1U<<16)

void systick_delay (int delay);

#endif /* SYSTICK_H_ */
