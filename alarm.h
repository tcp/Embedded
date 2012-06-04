/**
 * Include file for the alarm task
 */

#ifndef ALARM_H
#define ALARM_H

//#include "FreeRTOS.h"
//#include "task.h"
#include <stm32f10x.h>

//allowed time (seconds) between alarm activated and signaling
extern u8	allowedTime;

//alarmActive 	= 0 -> don't care about sensor signals
//				= 1 -> waiting for an eventual alarm
//				= 2 -> alarm activated, within grace time
//				= 3 -> alarm signalling
//				= 4 -> in beginning of deactivation
extern u8	alarmActive;

/**
 * Start two tasks that care abot the alarm situation
 */
void setupAlarm(unsigned portBASE_TYPE uxPriority);


#endif

