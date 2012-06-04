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
#include "led_display.h"



u8 toUseLED;
u8 blinkingLED;

/*-----------------------------------------------------------*/

/**
 * Light up one or more of the LEDs, to show that we are alive
 */

static void LEDTask(void *params) {

  	u32	rv;

	//cycle period = 2000 msec
	int cycleDelay = 2000;
	int blinkingDelay = 900;

  	for (;;) {
		//turn out old colour
		GPIOD->BRR |= 0x0002098;
		if (blinkingLED) vTaskDelay(blinkingDelay / portTICK_RATE_MS);

		//switch on current colour
		rv = 0;
		//yellow
		if (toUseLED & 0x01) rv = GPIO_Pin_13;
		//red
		if (toUseLED & 0x02) rv |= GPIO_Pin_3;
		//blue
		if (toUseLED & 0x04) rv |= GPIO_Pin_4;
		//green - not working
		//if (toUseLED & 0x08) rv |= GPIO_Pin_7;

		GPIOD->BSRR |= rv;

		vTaskDelay((cycleDelay - (blinkingDelay * blinkingLED)) / portTICK_RATE_MS);

	}
}


/**
 * Start the LED task - showing different LED colours for different status
 */
void setupLED(unsigned portBASE_TYPE uxPriority) {

	xTaskCreate(LEDTask, "LEDTask", 100, NULL, uxPriority, NULL);
}
