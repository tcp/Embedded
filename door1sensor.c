/**
 * Alarm routines
 * This sensor sends an alarm if circuit broken	(normally closed)
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "stm32f10x_gpio.h"

#include "global.h"
#include "alarm.h"
//#include "led_display.h"
#include "door1sensor.h"


/**
 * Task used to signal if door1 opens
 */
static void door1SensorTask(void *params) {

	uint8_t response;

	int alarmEventSend = DOOR_ALARM1;


	for (;;) {
		if (alarmActive > 0 ) {
			GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_SET);		

			vTaskDelay(1000 / portTICK_RATE_MS);

			response = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5);	
			if(response != 1)
				xQueueSend(alarmEventQueue, &alarmEventSend, portMAX_DELAY);

			GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_RESET);

		}
		//check every two seconds
		vTaskDelay(2000 / portTICK_RATE_MS);
								
   }    //forever loop ends here

}



/**
 * Start a task that check door1 situation (open -> alarm)
 */
void setupDoor1Sensor(unsigned portBASE_TYPE uxPriority) {
	//start task for door1 sensor
	xTaskCreate(door1SensorTask, "door1SensorTask", 100, NULL, uxPriority, NULL);
}

