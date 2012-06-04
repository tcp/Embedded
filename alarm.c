/**
 * Alarm routines
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "stm32f10x_gpio.h"
#include "glcd.h"

#include "global.h"
#include "alarm.h"
#include "screen.h"
#include "led_display.h"

//alarmActive 	= 0 -> don't care about sensor signals
//				= 1 -> waiting for an eventual alarm
//				= 2 -> alarm activated, within grace time
//				= 3 -> alarm signalling
//				= 4 -> in beginning of deactivation
u8	alarmActive;
u8	allowedTime;


/**
 * Check queue for alarm status
 */
static void alarmTask(void *params) {
   int queueData = 0;
   int i;

   u8 alarms = 0;
   u8	maxAlarms = 8;

   for (;;) {
  		xQueueReceive(alarmEventQueue, &queueData, portMAX_DELAY);
		//screenOutputTextLine(0, 192, "Alarm message");

		if (alarmActive == 0 ) {
			//activate alarm?
			if (queueData == ACTIVATE_ALARM ) alarmActive = 1;
		} else {
			switch(queueData) {
				case DOOR_ALARM1:
					//alarm from a door alarm
					screenOutputText(8, 13, Red, "Alarm1\0");
					alarms |= 1;

					if (alarmActive == 1) alarmActive = 2;

					break;
				case DOOR_ALARM2:
					//alarm from a door alarm
					screenOutputText(8, 13, Red, "Alarm2\0");
					alarms |= 2;

					if (alarmActive == 1) alarmActive = 2;

					break;
				case TEMPERATURE_LOW_ALARM:
					//alarm regarding low temperature
					if (alarmActive == 1) alarmActive = 2;

					break;
				case TEMPERATURE_HIGH_ALARM:
					//alarm regarding high temperature
					if (alarmActive == 1) alarmActive = 2;
			
					break;
				//case RESET_ALARM:
					//reset the alarm
				//	alarmActive = 1;
					//screenOutputTextLine(0, 216, "Alarm Reset     ");

				//	break;

				case DEACTIVATE_ALARM:
					//deactivate the alarm
					screenClear ();
					for (i = 0; i < maxAlarms; i++ ) {
						if(alarms & 1<<i) screenTextOk("Alarm!!!");
					}
					alarms = 0;

					screenOutputTextLine(0, 0, "Alarm Deactivated");
					vTaskDelay(500 / portTICK_RATE_MS);

					alarmActive = 0;

					break;

				default:	
					break;

			}  	//switch statment ends here

		}		//else statement ends here
	
		
   }   //forever loop ends here
}


/**
 * Give a signal as output when actually having an alarm
 */
static void giveAlarmTask(void *params) {

	//Use GPIO pins for output
	//GPIO pins are initialized in setup.c
	//
	//alarmActive == 2 -> check changes within grace time
	//alarmActive == 3 -> just repeat set bit on GPIOA pin 12
	//alarmActive == 1 -> reset pin and LED
	//alarmActive == 0 -> reset pin and LED

	u8	i;

	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	for (;;) {
		//xLastWakeTime = xTaskGetTickCount();
		if(alarmActive == 2) {

			//seem like an alarm sensor signals
			//although wait allowedTime seconds first, maybe it will be deactivated

			for (i = 0; i <= allowedTime && alarmActive == 2; i++) {
				//tick once a second in total for this loop
				vTaskDelayUntil(&xLastWakeTime, 500 / portTICK_RATE_MS);
				//give an output signal on GPIOA, pin 12
				//that is,  on CN8 (the left connector),
				//pin 13. Connect this pin to pin 1 on the
				//same connector
				//start by giving intermittent signals
				GPIO_WriteBit(GPIOA, GPIO_Pin_12, Bit_SET);
				toUseLED = RED_LED;
				vTaskDelayUntil(&xLastWakeTime, 500 / portTICK_RATE_MS);
				if(i < allowedTime)	{
					GPIO_WriteBit(GPIOA, GPIO_Pin_12, Bit_RESET);
				}
			}
			if (alarmActive == 2) {
				//still an alarm
				//steady light on LED
				blinkingLED = 0;
				alarmActive = 3;
			}
		} else if (alarmActive == 3) {
			//maybe not required, but repeat write bit
			GPIO_WriteBit(GPIOA, GPIO_Pin_12, Bit_SET);
		} else if (alarmActive == 1) {
			//normal status, waiting for signal from sensor
			GPIO_WriteBit(GPIOA, GPIO_Pin_12, Bit_RESET);
			blinkingLED = 1;
			toUseLED = YELLOW_LED; 
		} else if (alarmActive == 0) {
			//alarm not active at all - reset pin and LED
			GPIO_WriteBit(GPIOA, GPIO_Pin_12, Bit_RESET);
			blinkingLED = 1;
			toUseLED = BLUE_LED;
		}


		//wait for 1.0 sec before checking again
		//vTaskDelay(1000 / portTICK_RATE_MS);
		vTaskDelayUntil(&xLastWakeTime, 1000 / portTICK_RATE_MS);

	}   //forever loop ends here

}


/**
 * Start two tasks that care abot the alarm situation
 */
void setupAlarm(unsigned portBASE_TYPE uxPriority) {
	//no alarm possible yet -> set alarmActive to 0
	alarmActive = 0;
	xTaskCreate(alarmTask, "alarm", 100, NULL, uxPriority, NULL);
	xTaskCreate(giveAlarmTask, "giveAlarm", 100, NULL, uxPriority, NULL);
}

