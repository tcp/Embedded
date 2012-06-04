/**
 * Alarm routines
 * This sensor sends an alarm if circuit closed (normally open)
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "stm32f10x_gpio.h"

#include "global.h"
#include "alarm.h"
//#include "led_display.h"
#include "door2sensor.h"


/**
 * Task used to signal if door1 opens
 */
static void door2SensorTask(void *params) {

	uint8_t response;

	int alarmEventSend = DOOR_ALARM2;


	for (;;) {
		if (alarmActive > 0 ) {
			GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_SET);
					
			vTaskDelay(1000 / portTICK_RATE_MS);

			response = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);	
			if(response != 0)
				xQueueSend(alarmEventQueue, &alarmEventSend, portMAX_DELAY);

			GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_RESET);

		}
		//check every two seconds
		vTaskDelay(2000 / portTICK_RATE_MS);
				
   }    //forever loop ends here

}



/**
 * Start a task that check door2 situation (closed circuit -> alarm)
 */
void setupDoor2Sensor(unsigned portBASE_TYPE uxPriority) {
	//start task for door1 sensor
	xTaskCreate(door2SensorTask, "door2SensorTask", 100, NULL, uxPriority, NULL);
}

