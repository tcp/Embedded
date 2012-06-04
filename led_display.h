/**
 * Include file for global communication
 *
 */


#ifndef LED_DISPLAY_H
#define LED_DISPLAY_H



/**
 * LED colors that can occur
 */
typedef enum {
	//no LED active
	NO_LED = 0,

	//yellow 	-> pin 13
	YELLOW_LED = 1,

	//red		-> pin 3
	RED_LED = 2,

	//blue		-> pin 4
	BLUE_LED = 4,

	//green not working! (pin 7)
	GREEN_LED = 8

} LEDalternatives;


//the currently used LED
extern u8 toUseLED;
extern u8 blinkingLED;




//prototype, for starting LED task
void setupLED(unsigned portBASE_TYPE uxPriority);

#endif
