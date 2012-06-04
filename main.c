/**
 * Alarm program for the course "Programming embedded systems"
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
//#include "semphr.h"
#include "queue.h"
#include "setup.h"
#include "assert.h"
//#include "GLCD.h"
#include "stm3210c_eval_ioe.h"
#include "stm32f10x_gpio.h"

#include "global.h"
#include "screen.h"
#include "screen_test.h"
#include "alarm.h"
#include "alarm_test.h"
#include "master_alarm.h"
#include "led_display.h"
#include "door1sensor.h"
#include "door2sensor.h"
#include "i2c_ee.h"

xQueueHandle alarmEventQueue;

/*-----------------------------------------------------------*/

/*
 * Entry point of program execution
 */
int main( void )
{

	prvSetupHardware();

	/* Initialize the I2C EEPROM driver ----------------------------------------*/
	I2C_EE_Init();  

	//IOE_Config required for touch screen
	IOE_Config();


	initDisplay();

	setupLED(1);

	//test routine start - remove later	  	**********
	//lcdTest();
	//test routine done - remove later		***********

	alarmEventQueue = xQueueCreate(32, sizeof(AlarmEvent));

	setupAlarm(1);

	//test routine start - remove later	  	**********
	//setupAlarmTest();
	//test routine done - remove later		***********

	setupDoor1Sensor(1);
	setupDoor2Sensor(1);

	setupMasterAlarm(1);

	vTaskStartScheduler();
	assert(0);
	return 0;                 // not reachable
}
