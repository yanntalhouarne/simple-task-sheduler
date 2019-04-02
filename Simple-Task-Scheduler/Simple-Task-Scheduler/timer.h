/*
 * timer.h
 *
 * Created: 3/31/2019 12:24:58 PM
 *  Author: Yann
 */ 

#include <stdint.h>

#ifndef TIMER_H_
#define TIMER_H_

void (*_Timer_funPtr)(void); // function pointer which points to the function to be executed by the TMR1 ISR

void _Timer_setPeriodISR(uint8_t _ms);

void _Timer_start();

void _Timer_stop();

#endif /* TIMER_H_ */