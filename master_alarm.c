/**
 * Alarm program for the course "Programming embedded systems"
 */

#include <stdio.h>


#include "FreeRTOS.h"
#include "task.h"
//#include "semphr.h"
#include "queue.h"
#include "setup.h"
//#include "assert.h"
#include "GLCD.h"
//#include "stm3210c_eval_ioe.h"
//#include "stm32f10x_gpio.h"

#include "global.h"
#include "screen.h"
//#include "screen_test.h"
#include "alarm.h"
//#include "alarm_test.h"
#include "master_alarm.h"
#include "led_display.h"
#include "configure_alarm.h"
#include "i2c_ee.h"

/* Private variables ---------------------------------------------------------*/
uint8_t R_Buffer[BufferSize];
uint8_t Buf[BufferSize];
volatile TestStatus TransStatus = FAILED;

/*-----------------------------------------------------------*/

//u32	secretCode;
//u32 configureCode;


/**
 * Read some initial values
 */
static void startUp(void) {

	//temporary set secret code to something
	//secretCode = 23;

	//maybe also a separate code for configuring?
	//configureCode = 45;
	//allowed time (seconds) between alarm activated and signaling
	allowedTime = 10;


	//start values for the LED's; blue and blinking
	toUseLED = BLUE_LED;
	blinkingLED = 1;

}

/**
 * Main task - taking over after main
 *
 */
static void masterAlarmTask(void *params) {
	unsigned char menuText0[] = " Activate alarm\0";
	unsigned char menuText1[] = " Configure alarm\0";
	unsigned char menuText2[] = "Deactivate code:\0";
	unsigned char messageText0[] = "Wrong code\0";

	int alarmEventSend;

	//mainStatus == 0 -> with  menu to activate or config
	//mainStatus == 1 -> with possibility to deactivate alarm
	int	mainStatus = 0;
	//statusReturn used for temporary response from other routines
	int statusReturn;

	//get start values
	startUp();

	for (;;) {
		switch(mainStatus) {
			case 0:			//initial (first) status
				//ask to activate alarm or to configure alarm
				statusReturn = screenMenu2(menuText0, menuText1);
				//screenOutputTextLine(0, 192, "main statusReturn");

				if(statusReturn == 1) {
					//activate alarm
					alarmEventSend = ACTIVATE_ALARM;
					//screenOutputTextLine(0, 192, "main activate");
					xQueueSend(alarmEventQueue, &alarmEventSend, portMAX_DELAY);
					mainStatus = 1;
					toUseLED = YELLOW_LED;

				} else if(statusReturn == 2) {
					//configure
					//screenOutputTextLine(0, 192, "main configure");
					toUseLED = BLUE_LED;
					configureAlarm();

				}

				break;
			case 1:			//alarm active
				//give a chance to deactivate
				statusReturn = screenQueryNumber(Blue, menuText2, 8);
				
				// integer to buffer
				Int2Buf (statusReturn, Buf);
				
				/* Read from I2C EEPROM from EEPROM_ReadAddress */
				I2C_EE_BufferRead(R_Buffer, EEPROM_ReadAddress, BufferSize);
				
				/* Check if the data written to the memory is read correctly */
				TransStatus = Buffercmp(Buf, R_Buffer, BufferSize);

				if(TransStatus) //if (statusReturn == secretCode) 
				{
					//ok code -> deactivate
					alarmEventSend = DEACTIVATE_ALARM;
					xQueueSend(alarmEventQueue, &alarmEventSend, portMAX_DELAY);

					//wait for the alarmTask to be ready
					while (alarmActive > 0)
						vTaskDelay(100 / portTICK_RATE_MS);

					mainStatus = 0;
					blinkingLED = 1;
					toUseLED = BLUE_LED;
				} else {
					//wrong code
					screenClear();
					screenOutputTextLine(Red, Line3, messageText0);
		
					vTaskDelay(500 / portTICK_RATE_MS);	
				}

				break;
		}			//end switch statements

		//while testing with debugger, have a delay here - remove when screen available
		//vTaskDelay(1000 / portTICK_RATE_MS);

	}		//end forever loop


}




/**
 * Start a task that care about the alarm and configuration
 */
void setupMasterAlarm(unsigned portBASE_TYPE uxPriority) {

	xTaskCreate(masterAlarmTask, "masterAlarmTask", 100, NULL, uxPriority, NULL);

}
/*-----------------------------------------------------------*/


