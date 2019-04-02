/*
 * timer.c
 *
 * Created: 3/31/2019 12:18:09 PM
 *  Author: Yann
 *
 * Helper function to setup timers for the ATmega324PB
 *
 */ 

#include <avr/interrupt.h>
#include <stdint.h>

#include "timer.h"

/************************************************************
 * _Timer_setupTaskTimer: - setups TMR1 for "_ms" ms ISR  *
 *						   		        				    *
 ************************************************************/
void _Timer_setPeriodISR(uint8_t _ms)
{
	// COMPARE OUTPUT MODE
	TCCR1A |= (1<<COM1A1) | (1<<COM1A0); // clear OCRA on compare
	
	// WAVEFORM GENERATION MODE
	TCCR1B |= (1<<WGM12); // CTC mode
	
	// PRESCALER
	if (_ms < 5 )
	{
		TCCR1B |= (1<<CS10); // no-prescaling, allows better accuracy
	}
	else
	{
		TCCR1B |= (1<<CS11); // prescaler of 8 to avoid overflow
	}
	
	// PERIOD
	switch (_ms)
	{
		case 1:
			OCR1A = 15999; 
			break;
		case 2:
			OCR1A = 31999; 
			break;
		case 3:
			OCR1A = 47999; 
			break;
		case 4:
			OCR1A = 63999; 
			break;
		case 5:
			OCR1A = 9999; 
			break;
		case 6:
			OCR1A = 11999; 
			break;
		case 7:
			OCR1A = 13999; 
			break;
		case 8:
			OCR1A = 15999; 
			break;
		case 9:
			OCR1A = 17999; 
			break;
		case 10:
			OCR1A = 19999;
			break;
		default:
			TCCR1B |= (1<<CS10); 
			OCR1A = OCR1A = 15999; // 1ms default
			break;
	}

}

/************************************************************
 * _Timer_start: - enables interrupts on TMR1			    *
 *						   		        				    *
 ************************************************************/
void _Timer_start()
{
	// INTERRUPT
	TIMSK1 |= (1<<OCIE1A); // set interrupt on OCA compare
}

/************************************************************
 * _Timer_stop: - disables interrupts on TMR1			    *
 *						   		        				    *
 ************************************************************/
void _Timer_stop()
{
	// INTERRUPT
	TIMSK1 &= ~(1<<OCIE1A); // clear interrupt on OCA compare
}

/*************************************************************
 * TIMER1 ISR: - calls the function pointed by _TMR_funPtr   *
 *										        	         *
 *************************************************************/
ISR(TIMER1_COMPA_vect)
{
	(*_Timer_funPtr)(); // call TMR1 function
}
