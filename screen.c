/**
 * Screen routines
 */

#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "setup.h"
#include "assert.h"
#include "GLCD.h"
#include "stm3210c_eval_ioe.h"

#include "global.h"
#include "screen.h"

/*-----------------------------------------------------------*/


#define WIDTH 	320
#define HEIGHT  240

xSemaphoreHandle lcdLock;

/*-----------------------------------------------------------*/
/**
 * Initialize the display
 */
void initDisplay () {

  lcdLock = xSemaphoreCreateMutex();

  GLCD_init();
  GLCD_clear(White);
  GLCD_setTextColor(Blue);
}


/*-----------------------------------------------------------*/
/**
 * help routine to display a grid on the screen
 *
 *	type = 0 if characters
 *	type = 1 if numbers
 *  capsIn = 0 when characters and lower case characters shall be used
 *	capsIn = 1 when characters and capital charaters shall be used
 *
 */
static void screenDisplayGrid(int type, int capsIn) {
	//uint16_t x0 = 150;
	//uint8_t y0 = 28;
	//make longer lines when characters
	uint16_t lengthH = (type) ? 210 : 290;
	uint16_t lengthV = (type) ? 170 : 138;
	//lower case characters are 32 above capital characters
	int		caps = (capsIn) ? 0 : 32;
	int		yD, yC, yE;
	int		xD, xC, xE;
	int		i;
	

 	xSemaphoreTake(lcdLock, portMAX_DELAY);

	if(type == 0) {
		//display characters
		//first two lines with characters
		for (i = 0; i < 19; i++)
			GLCD_displayChar(38 + 50 * (i/10), 302 - 30 * (i % 10), 65 + i + caps);
		//third line with characters
 		for (i = 0; i < 7; i++)
			GLCD_displayChar(138, 302 - 30 * (i % 10), 84 + i + caps);
		//decide where to draw Delete, Cancel and Enter
		yD = yC = yE = 182;
		xD = 302;
		xC = 182;
		xE = 80;

	} else {  
		//display numbers
		GLCD_displayChar(38,  278,'7');
		GLCD_displayChar(38,  208,'8');
		GLCD_displayChar(38,  138,'9');
		GLCD_displayChar(88,  278,'4');
		GLCD_displayChar(88,  208,'5');
		GLCD_displayChar(88,  138,'6');
		GLCD_displayChar(138, 278,'1');
		GLCD_displayChar(138, 208,'2');
		GLCD_displayChar(138, 138,'3');
		GLCD_displayChar(188, 208,'0');

		//decide where to draw Delete, Cancel and Enter
		yD = 38;
		yC = 88;
		yE = 138;
		xD = xC = xE = 74;
	}
	//draw Delete, starting in position yD, xD
	GLCD_displayChar(yD,   xD,     'D');
	GLCD_displayChar(yD,   xD - 14,'e');
	GLCD_displayChar(yD,   xD - 28,'l');
	GLCD_displayChar(yD,   xD - 42,'e');
	GLCD_displayChar(yD,   xD - 56,'t');
	GLCD_displayChar(yD,   xD - 70,'e');
	//draw Cancel, starting in position yC, xC
	GLCD_displayChar(yC,   xC,     'C');
	GLCD_displayChar(yC,   xC - 14,'a');
	GLCD_displayChar(yC,   xC - 28,'n');
	GLCD_displayChar(yC,   xC - 42,'c');
	GLCD_displayChar(yC,   xC - 56,'e');
	GLCD_displayChar(yC,   xC - 70,'l');
	//draw Enter, starting in posinio yE, xE
	GLCD_displayChar(yE,   xE,     'E');
	GLCD_displayChar(yE,   xE - 14,'n');
	GLCD_displayChar(yE,   xE - 28,'t');
	GLCD_displayChar(yE,   xE - 42,'e');
	GLCD_displayChar(yE,   xE - 56,'r');
			
	//draw help lines
	GLCD_drawLine(68,  315 - lengthH, lengthH, Horizontal);
	GLCD_drawLine(118, 315 - lengthH, lengthH, Horizontal);
	GLCD_drawLine(168, 315 - lengthH, lengthH, Horizontal);
	if (type == 0) {
		//add text: Caps
		GLCD_displayChar(138,   80,     'S');
		GLCD_displayChar(138,   66,     'p');
		GLCD_displayChar(138,   52,     'a');
		GLCD_displayChar(138,   38,     'c');
		GLCD_displayChar(138,   24,		'e');

		// add an filled arrow, down if caps now, up if lower case now
		GLCD_displayChar( 88,   24,		133 + 2 * capsIn);
		
		//draw verical lines for displayed characters
		for (i = 0; i < 9; i++)
			GLCD_drawLine(30, 295 - i * 30, lengthV - (i / 7) * 50, Vertical);

		//special lines around Delete, Cancel and Enter
		GLCD_drawLine(170,   315 - lengthH, lengthH, Horizontal);
		GLCD_drawLine(210,   315 - lengthH, lengthH, Horizontal);
		GLCD_drawLine(212,   315 - lengthH, lengthH, Horizontal);

		for (i = 0; i < 2; i++) {
			GLCD_drawLine(171,  108 + i * 108, 38, Vertical);
			GLCD_drawLine(171,  110 + i * 108, 38, Vertical);
		}

	} else {
		//draw vertical lines for displayed numbers
		GLCD_drawLine(30,  245, lengthV, Vertical);
		GLCD_drawLine(30,  175, lengthV, Vertical);

		//special lines around Delete, Cancel and Enter
		GLCD_drawLine(26,   0, 94, Horizontal);
		GLCD_drawLine(28,   0, 92, Horizontal);
		GLCD_drawLine(68,   0, 92, Horizontal);
		GLCD_drawLine(70,   0, 92, Horizontal);
		GLCD_drawLine(118,  0, 92, Horizontal);
		GLCD_drawLine(120,  0, 92, Horizontal);
		GLCD_drawLine(168,  0, 92, Horizontal);
		GLCD_drawLine(170,  0, 94, Horizontal);

		GLCD_drawLine(28,  92, 142, Vertical);
		GLCD_drawLine(26,  94, 144, Vertical);
	}

	xSemaphoreGive(lcdLock);
}


/**
 * read from touch screen
 *
 *	dataX - output x
 *	dataY - output y
 */
static void screenTouched(int *dataX, int *dataY) {
	int temp;
 	u8 notReady = 1;
	u8 pressed = 0;
	TS_STATE *ts_state;

	while(notReady) {
    	xSemaphoreTake(lcdLock, portMAX_DELAY);
		//delay required, alternative was to have the displaySting below
		//without delay the system will hang!

		vTaskDelay(10 / portTICK_RATE_MS);  		
		//GLCD_displayStringLn(Line9, "semaphore taken");
		ts_state = IOE_TS_GetState();
		if (pressed) {
			//wait until no touch
			if (!ts_state->TouchDetected) {
				notReady = 0;
			}
			xSemaphoreGive(lcdLock);
		} else if(ts_state->TouchDetected) {
			temp = ts_state->X;
			*dataX = temp;
			temp = ts_state->Y;
			xSemaphoreGive(lcdLock);
			*dataY = temp;
			//check to see valid numbers entered
			//number 0,0 not concidered as valid
			//(a few times the response has been 0,0)
			if (dataX > 0 && dataY > 0 )pressed = 1;
		} else {
			xSemaphoreGive(lcdLock);
			//no touch yet, wait 0.1 s before checking again
			vTaskDelay(100 / portTICK_RATE_MS);
		}
	}
 }


/**
 * find out wich square in screenDisplayGrid was touched
 * - unfortunately not the same coordinate systemj!!!
 * for text and lines: 	x = 0 at right border of screen, pos to the left
 *						y = 0 at top of screen, pos downwards
 * for touch			x = 0 at left border of screen, pos to the right
 *						y = 0 at top of screen, pos downwards
 *
 *	returns input number
 *	returns	200 if Delete pressed (delete latest character)
 *	returns 201 if Cancel pressed (finished, don't care about number)
 *	returns 202 if Enter pressed (finished)
 */
static int screenGridTouchedDigit(int dataX, int dataY) {
 	int response = -1;
	int y0 = 68;
	int i;
	
	//exclude values with y < 10
	if (dataY > 10) {
		//check the first three lines first
		//start with upper line
		for (i = 0; i < 3 && response < 0; i++) {
	 		if( dataY < y0 + i * 50 ) {
				//on a row
				if (dataX < 75 ) {
					//leftmost, number 7, 4 or 1
					response = 7 - i * 3;
				} else if (dataX < 145 ) {
					//middle, number 8, 5, 2
					response = 8 - i * 3;
				} else if (dataX < 215 ) {
					//number 9, 6, 3
					response = 9 - i * 3;
				} else if (dataX > 215 ) {
					//i = 0 -> delete -> response = 200
					//i = 1 -> cancel -> response = 201
					//i = 2 -> enter -> response = 202
					response = 200 + i;
				}
			}
		}
	}

	if (response < 0 ) {
		//check if 0
		if(dataY > 168 && dataY < 218 &&
			dataX > 75 && dataX < 145 ) {
			//number 0
			response = 0;			
		}
	}

	return response;
 }


/**
 * find out wich square in screenDisplayGrid was touched
 * - unfortunately not the same coordinate systemj!!!
 * for text and lines: 	x = 0 at right border of screen, pos to the left
 *						y = 0 at top of screen, pos downwards
 * for touch			x = 0 at left border of screen, pos to the right
 *						y = 0 at top of screen, pos downwards
 *
 *	returns input character number (uppercase number)
 *	returns 199 if Caps pressed (changes between upper and lower characters)
 *	returns	200 if Delete pressed (delete latest character)
 *	returns 201 if Cancel pressed (finished, don't care about number)
 *	returns 202 if Enter pressed (finished)
 */
static int screenGridTouchedChar(int dataX, int dataY) {
 	int response = -1;
	int y0 = 68;
	int i, j;
	
	//exclude values with y < 10
	if (dataY > 10) {
		//start by checking the first three lines
		for (i = 0; i < 3 && response < 0; i++) {			
	 		if( dataY < y0 + i * 50 ) {
				//on a row
				for (j = 0; j < 10 && response < 0; j++) {
					if (dataX < 25 + j * 30 ) response = 65 + i * 10 + j;
				}									
			}

		}
		//two exceptions because of a caps up or down at the right on line 2
		if (response > 84)
			response--;
		else if (response == 84)
			response = 199;


		//maybe space pressed
		if (response == 91 || response == 92 || response == 93) response = 32;

		//check for Delete, Cancel and Enter
		if (dataY > 170 && dataY < 210 ) {
			//Delete, Cancel or Enter
			if (dataX < 102 ) {
				// Delete
				response = 200;
			} else if (dataX < 210 ) {
				// Cancel
				response = 201;
			} else {
				// Enter
				response = 202;
			}		
		}
	}

	return response;
 }


/**
 * Display one character, in pos y, x
 * with color according to textCol (= text color)
 * with size of y = line number
 * with size of x = number of characters from left border
 * (max 20 characters on one line)
 *
 * y = 0 -> first line, y = 2 -> second line....
 *
 * in this case input x=0 means the left border
 * A small distance to the right will be added to x
 */
static void screenDisplayChar(int y, int x, unsigned short textCol, char c) {
	unsigned char cOut;

	cOut = c;

    xSemaphoreTake(lcdLock, portMAX_DELAY);
  	GLCD_setTextColor(textCol);		
	GLCD_displayChar(y * 24, 304 - 16 * x, cOut);
	xSemaphoreGive(lcdLock);
}


/**
 * Clear diplay - make white
 */
void screenClear () {

  xSemaphoreTake(lcdLock, portMAX_DELAY);
  GLCD_clear(White);
  xSemaphoreGive(lcdLock);
}


/**
 * Diplay one line of text, max 20 chars
 *
 * lineNo = selected Line1 to Line9 (or y-coordinates)
 * textCol = text color, see file GLCD.h (not all colors work)
 * text = text to print
 */
void screenOutputTextLine(unsigned short textCol, unsigned int lineNo, unsigned char text[]) {

  xSemaphoreTake(lcdLock, portMAX_DELAY);
  GLCD_setTextColor(textCol);
  GLCD_displayStringLn(lineNo, text);
  xSemaphoreGive(lcdLock);
}

/**
 * Diplay some chars somewhere on the screen (horisontal)
 *
 * with size of y = line number
 * with size of x = number of characters from left border
 * (max 20 characters on one line)
 *
 * y = 0 -> first line, y = 2 -> second line....
 *
 * In this case input x=0 means the left border
 * A small distance to the right will be added to x
 *
 * textCol = text color, see file GLCD.h (not all colors work), 0 = black
 * text = text to print
 */
void screenOutputText(int y, int x, unsigned short textCol, unsigned char text[]) {
	int i = 0; 		//counter

  	xSemaphoreTake(lcdLock, portMAX_DELAY);
  	GLCD_setTextColor(textCol);
	xSemaphoreGive(lcdLock);
	
	while (text[i] > 0 ) {
		screenDisplayChar(y, x + i, textCol, text[i]);
		i++;
	}
}


/**
 * Display one line of text plus a grid with numbers
 * from 0 to 9. Asking for example to "Enter Code
 *
 * textCol - text color, see file GLCD.h (not all colors work)
 * text - a number of characters forming a text string
 * numberOfDigits - maximum number of digits in response
 *
 * returns entered number, which can be 0
 */
int screenQueryNumber(unsigned short textCol, unsigned char text[], int numberOfDigits) {
	int dataX;
	int dataY;
	int digit = 0;
	char cDigit;
	int pos = 0;
	int totalInput = 0;

	screenClear ();

 	xSemaphoreTake(lcdLock, portMAX_DELAY);
	//clear display
	GLCD_clear(White);
	//set color
	GLCD_setTextColor(textCol);
	//write text on bottom line
  	GLCD_displayStringLn(Line9, text);
	xSemaphoreGive(lcdLock);

	//show grid with numbers
	screenDisplayGrid(1, 0);

	//loop over touched numbers
	while (pos < numberOfDigits && digit < 201) {
		//find out where screen touched
		screenTouched(&dataX, &dataY);

		//find out the meaning of where screen touched
		digit = screenGridTouchedDigit(dataX, dataY);
		if(digit >= 0 && digit <= 9) {
			//regular input of digit
			pos++;
			cDigit = digit + 48;
			totalInput = totalInput*10 + digit;
			screenDisplayChar(0, pos, textCol, cDigit);
		} else if (digit == 200 && pos > 0 ) {
			//delete previous character
			cDigit = ' ';
			totalInput = totalInput/10;		
			screenDisplayChar(0, pos, textCol, cDigit);
			pos--;
		}
	}
	if(digit == 201) {
		//cancel pressed
		totalInput = 0;
	}

	return totalInput;
}


/**
 * Display one line of text plus a grid with characters (qwerty...)
 * asking for example to "Enter Text"
 *
 * textCol - text color, see file GLCD.h (not all colors work)
 * text - a number of characters forming a text string
 * numberOfChars - maximum number of characters in response
 * (maximum characters in one line is 20)
 *
 * returns entered characters, which can be ' ', ended by a \0
 */
void screenQueryChars(unsigned short textCol, unsigned char text[], int numberOfChars, unsigned char textEntered[]) {
	int dataX;
	int dataY;
	int charInt = 0;
	int pos = 0;
	int	caps = 1;
	//let capsOld be different from caps,
	//so grid with characters will be drawn first time
	int capsOld = 1 - caps;

	screenClear ();

 	xSemaphoreTake(lcdLock, portMAX_DELAY);
	//clear display
	GLCD_clear(White);
	//set color
	GLCD_setTextColor(textCol);
	//write text on bottom line
  	GLCD_displayStringLn(Line9, text);
	xSemaphoreGive(lcdLock);

	//loop over touched numbers
	while (pos < numberOfChars && charInt < 201) {
		//if caps changed, redraw grid with characters
		if (caps != capsOld) {
			screenDisplayGrid(0,caps);
			capsOld = caps;
		}
		//find out where screen touched
		screenTouched(&dataX, &dataY);
		//and which character this means
		charInt = screenGridTouchedChar(dataX, dataY);

		if (charInt == 32 || (charInt > 64  && charInt < 91)) {
			//spece or regular printable character
			//eventually change to lower case
			if (!caps && charInt != 32) charInt = charInt + 32;
			textEntered[pos] = charInt;
			//if lower case change character (except for spaces)
			screenDisplayChar(0, pos, textCol, charInt);
			pos++;
		} else if (charInt == 199) {
			//change between caps and lower case
			caps = 1 - caps;
		} else if (charInt == 200) {
			//Delete one character
			if (pos > 0) pos--;
			//write a space (clear next position)
			screenDisplayChar(0, pos, textCol, 32);
		} else if (charInt == 201 ) {
			//Cancel pressed
			pos = 0;
			//add a char 0 to end of string
			textEntered[pos] = 0;
		} else if (charInt == 202) {
			//Enter pressed
			textEntered[pos] = 0;			
		}
	}
}


/**
 * 	Display a menu and select menu item number
 *	With 2 menu items
 *
 *	text1 - text on line 1 (max 18 characters)
 *	text2 - text on line 2 (max 18 characters)
 *
 *	returns selected menu alternative, 1 or 2
 */
int screenMenu2(unsigned char text1[], unsigned char text2[] ) {
	int dataX;
	int dataY;
	int i = 0;

	screenClear ();

	screenOutputText(3, 1, Blue, text1);
	screenOutputText(6, 1, Blue, text2);

	//draw rectangles
	xSemaphoreTake(lcdLock, portMAX_DELAY);	
	GLCD_setTextColor(Blue);
	GLCD_drawRect(Line2, 5, 72,310);
	GLCD_drawRect(Line5, 5, 72, 310);
	
  	xSemaphoreGive(lcdLock);

	//wait for user to select alternative
	while(i == 0) {
		//select alternative
		screenTouched(&dataX, &dataY);
		//find out which alternative selected
		if (dataY > Line2 && dataY < Line5) i = 1;
		if (dataY > Line5 && dataY < Line8) i = 2;
		
	}

	//confirm choice
	xSemaphoreTake(lcdLock, portMAX_DELAY);	
	GLCD_setTextColor(Green);
	GLCD_setBackColor (Green);
	if ( i == 1) {
		GLCD_fillRect(Line2, 5, 72, 310);
  		xSemaphoreGive(lcdLock);
		screenOutputText(3, 1, Blue, text1);
	} else {
		GLCD_fillRect(Line5, 5, 72, 310);
	  	xSemaphoreGive(lcdLock);
		screenOutputText(6, 1, Blue, text2);
	}

	xSemaphoreTake(lcdLock, portMAX_DELAY);
	GLCD_setBackColor (White);
  	xSemaphoreGive(lcdLock);

	vTaskDelay(900 / portTICK_RATE_MS);

	return i;
}



/**
 *	Display a menu and select menu item number
 * 	With 3 menu items
 *
 *	text1 - text on line 1 (max 18 characters)
 *	text2 - text on line 2 (max 18 characters)
 *	text3 - text on line 3 (max 18 characters)
 *
 *	returns selected menue alternative, 1, 2 or 3
 */
int screenMenu3(unsigned char text1[], unsigned char text2[], unsigned char text3[]) {
	int dataX;
	int dataY;
	int i = 0;

	screenClear ();

	//write text
	screenOutputText(2, 1, Blue, text1);
	screenOutputText(4, 1, Blue, text2);
	screenOutputText(6, 1, Blue, text3);

	//draw rectangles
	xSemaphoreTake(lcdLock, portMAX_DELAY);	
	GLCD_setTextColor(Blue);
	GLCD_drawRect(Line2 - 12, 5, 48, 310);
	GLCD_drawRect(Line4 - 12, 5, 48, 310);
	GLCD_drawRect(Line6 - 12, 5, 48, 310);
	
  	xSemaphoreGive(lcdLock);

	//wait for user to select alternative
	while(i == 0) {
		//select alternative
		screenTouched(&dataX, &dataY);
		//find out which alternative selected
		if (dataY > Line2 - 12 && dataY < Line4 - 12) i = 1;
		if (dataY > Line4 - 12 && dataY < Line6 - 12) i = 2;
		if (dataY > Line6 - 12 && dataY < Line8 - 12) i = 3;
	}

	xSemaphoreTake(lcdLock, portMAX_DELAY);	
	GLCD_setTextColor(Green);
	GLCD_setBackColor (Green);
	if ( i == 1) {
		GLCD_fillRect(Line2 - 12, 5, 48, 310);
  		xSemaphoreGive(lcdLock);
		screenOutputText(2, 1, Blue, text1);
	} else if (i == 2) {
		GLCD_fillRect(Line4 - 12, 5, 48, 310);
	  	xSemaphoreGive(lcdLock);
		screenOutputText(4, 1, Blue, text2);
	} else {
		GLCD_fillRect(Line6 - 12, 5, 48, 310);
	  	xSemaphoreGive(lcdLock);
		screenOutputText(6, 1, Blue, text2);
	}

	xSemaphoreTake(lcdLock, portMAX_DELAY);
	GLCD_setBackColor (White);
  	xSemaphoreGive(lcdLock);

	vTaskDelay(900 / portTICK_RATE_MS);


	return i;
}


/**
 * 	Display a line with text and let the user confirm
 *
 *	text - text on line, within rectangle max 18 characters)
 *
 */
int screenTextOk(unsigned char text[] ) {
	int dataX;
	int dataY;
	int i = 0;

	screenClear ();

	//write text
	screenOutputText(4, 1, Blue, text);
	screenOutputText(6, 5, Red, "Please confirm");

	//draw rectangle 
	xSemaphoreTake(lcdLock, portMAX_DELAY);	
	GLCD_setTextColor(Blue);
	GLCD_drawRect(Line2, 5, 120,310);
	xSemaphoreGive(lcdLock);

	//wait for the user to confirm
	while(i == 0) {
		//select alternative
		screenTouched(&dataX, &dataY);
		//find out which alternative selected
		if (dataY > Line2 && dataY < Line6) i = 1;		
	}

	return i;

}

