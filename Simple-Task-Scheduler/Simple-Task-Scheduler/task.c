/*
 * task.c
 *
 * Created: 3/31/2019 12:13:49 PM
 *  Author: Yann
 *
 * Brief: This is a simple task scheduler implementation. 
 * 
 * 
 * Features: - 1ms-10ms selectable tick period
 *			 - non-preemptive
 *			 - priority based
 *			 - hardware abstraction
 */ 
    
#include <stdint.h>
#include <stdlib.h>

#include "task.h"
#include "timer.h"


struct Task * _Task_headList = NULL; // head of the task list


uint8_t task_count = 0;

uint8_t ms_count = 0;

extern void (*TMR_fun_ptr)(void);

/**************************************************************************
 * _Task_setTickPeriod: 												  *
 *						   		        				                  *
 **************************************************************************/
void _Task_setTickPeriod(uint8_t period) // from 1ms to 10 ms (default is 1ms)
{
	_Timer_funPtr = &_Task_updateState; // get timer IS7R function pointer
	_Timer_setPeriodISR(period); // setup timer for periodic interrupt
}



/**************************************************************************
 * _Task_releaseTasks:													  *
 *						   		        				                  *
 **************************************************************************/
void _Task_releaseTasks()
{
	_Timer_start(); // enable TMR interrupt
}



/**************************************************************************
 * _Task_stopTasks:														  *
 *						   		        				                  *
 **************************************************************************/
void _Task_stopTasks()
{
	_Timer_stop(); // disable TMR interrupt
}



/**************************************************************************
 * _Task_newTask:									 					  *
 *						   		        				                  *
 **************************************************************************/
void _Task_newTask(int _ID, uint8_t _period, int _priority, void (*_funptr)())
{
	struct Task * newTask = malloc(sizeof(*newTask));
	
	newTask->ID = _ID;
	newTask->period = _period;
	newTask->expire_time = _period;
	newTask->priority = _priority;
	newTask->state = READY;
	newTask->funptr = _funptr;

	_Task_orderTask(newTask);
	
	task_count++;
}



/************************************************************************************
 * _Task_orderTask:	- This function insures that the tasks are ordered by			*
 *					   priority (lowest priority at the end of the list				*
 *					- This ordered list allows the scheduler to run the highest		*
 *					   priority task first,							     			*
 *					- IF two tasks have the same priority, the oldest task has		*
 *					  higher priority (newest task is inserted after the older		*
 *					  one.	   		        										*
 ************************************************************************************/
void _Task_orderTask(struct Task * newTask)
{
	// temp nodes
	struct Task * prev; 
	struct Task * cur;

	// go through the list until we find a lowest priority task (list should already \
	   be ordered) or until the end of the list (if the new tasks priority is the lowest
	for (cur = _Task_headList, 
		 prev = NULL; newTask->priority > cur->priority || cur != NULL; 
		 prev = cur, cur = cur->next); 

	// the case where the new task has the highest priority (becomes the first task \
	   in the list), so the header must point to it											  
	if (cur == _Task_headList) 							     
	{
		newTask->next = _Task_headList;
		
		_Task_headList = newTask;
	}
	else // otherwise, insert the task in the list
	{
		prev->next = newTask;
		
		newTask->next = cur;
	}
	
}



/**************************************************************************
 * _Task_scheduleTask: 													  *
 *						   		        				                  *
 **************************************************************************/
void _Task_scheduleTask()
{
	struct Task * cur;

	// go through the list until we find a task that is in the ready state
	for (cur = _Task_headList; cur != NULL; cur = cur->next) 
	{
		if (cur->state == READY)
		{
			cur->state = EXECUTING; // update state to executing
			cur->funptr(); // call task callback function
			cur->state = BLOCKING; // task is now waiting for the next event
		}
	}
}



/**************************************************************************
 * _Task_updateState:													  *
 *						   		        				                  *
 **************************************************************************/
void _Task_updateState()
{
	ms_count++; // increment ms count
		
	struct Task * cur;	
	
	// go through the list until we find a task that is in the ready state
	for (cur = _Task_headList; cur != NULL; cur = cur->next) 
	{
		if (ms_count == cur->expire_time) // if the task need to be executed,
		{
			// if the task is EXECUTING or still in the READY state, the \
			   deadline has been missed
			if ( (cur->state == EXECUTING) || (cur->state == READY) ) 
			{
				cur->missed_deadline++;
			}
			else // else update the next expiration time (next deadline)
			{
				cur->expire_time += cur->period; 
				cur->state = READY; // set the task to ready state
			}
		}
	}
}

