/*
 * task.h
 *
 * Created: 3/31/2019 12:13:09 PM
 *  Author: Yann
 */ 


#ifndef TASK_H_
#define TASK_H_

#include <stdint.h>

// states
#define READY 0
#define EXECUTING 1
#define BLOCKING 2


 struct Task
{
	void (*funptr)();
	uint8_t period;
	uint8_t expire_time;
	uint8_t priority;
	uint16_t missed_deadline;
	uint16_t ID;
	char state;
	
	struct Task * next;
};

void _Task_setTickPeriod(uint8_t period);

void _Task_releaseTasks();

void _Task_stopTasks();

void _Task_newTask(int _ID, uint8_t _period, int _priority, void (*_funptr)());

void _Task_scheduleTask();

void _Task_updateState();

void _Task_orderTask(struct Task * newTask);


#endif /* TASK_H_ */