/*
 * Simple-Task-Scheduler.c
 *
 * Created: 3/31/2019 12:11:54 PM
 * Author : Yann
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "task.h"


//&&&&&&&&&&&&&&&&& MACROS &&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
/* LED */
#define LED1_DDR DDRE
#define LED1_PORT PORTE
#define LED1_PIN 4
#define LED1_ON LED1_PORT |= (1<<LED1_PIN);
#define LED1_OFF LED1_PORT &= ~(1<<LED1_PIN);
#define TOGGLE_LED1 LED1_PORT ^= (1<<LED1_PIN);

#define LED2_DDR DDRB
#define LED2_PORT PORTB
#define LED2_PIN 5
#define LED2_ON LED2_PORT |= (1<<LED2_PIN);
#define LED2_OFF LED2_PORT &= ~(1<<LED2_PIN);
#define TOGGLE_LED2 LED2_PORT ^= (1<<LED2_PIN);



// TASK 1
#define TASK1_ID 1
#define TASK1_PERIOD 100
#define TASK1_PRIORITY 0
void task1_callback();

// TASK 2
#define TASK2_ID 2
#define TASK2_PERIOD 200
#define TASK2_PRIORITY 0
void task2_callback();

int main(void)
{
	
	_Task_setTickPeriod(1); // set the scheduler period to 1ms
	
	_Task_newTask(TASK1_ID, TASK1_PERIOD, TASK1_PRIORITY, &task1_callback); 
	
	_Task_newTask(TASK2_ID, TASK2_PERIOD, TASK2_PRIORITY, &task2_callback); 
	
	_Task_releaseTasks();
	
    /* Replace with your application code */
    while (1) 
    {
		_Task_scheduleTask();
    }
}

void task1_callback()
{
	TOGGLE_LED1;
}

void task2_callback()
{
	TOGGLE_LED2;
}


