/**
 * Include file for global communication
 *
 */


#ifndef GLOBAL_H
#define GLOBAL_H

#include "queue.h"


/**
 * Alarm events that can occur
 */
typedef enum {
	UNASSIGNED = 0,

	// Door alarm
	DOOR_ALARM1 = 1,
	DOOR_ALARM2 = 2,

	//Temperature alarms
	TEMPERATURE_LOW_ALARM = 6,
	TEMPERATURE_HIGH_ALARM = 7,


	//reset alarms	  - currently not used
	//RESET_ALARM = 100,  
	//activate alarm
	ACTIVATE_ALARM = 101,
	//deactivate alarm
	DEACTIVATE_ALARM = 102

} AlarmEvent;


// extern xQueueHandle printQueue;
extern xQueueHandle alarmEventQueue;


#endif
