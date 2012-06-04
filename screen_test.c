/**
 * Screen test routines
 */

#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
//#include "semphr.h"
//#include "queue.h"
#include "setup.h"
#include "assert.h"
#include "GLCD.h"
//#include "stm3210c_eval_ioe.h"

#include "global.h"
#include "screen.h"

/**
 *task for testing screen
 */
void lcdTestTask(void *params) {

  int response;
  int i;
  unsigned int lineNo = Line0;
  unsigned int textCol = Black;
  unsigned char menuText1[16] = "Activate\0";
  unsigned char menuText2[16] = "Config\0";
  unsigned char menuText3[16] = "Exit\0";
  unsigned char testText[16] = "Hejhopp  Line 0\0";
  unsigned char enterCode[20] = "Enter code:\0";
  char touched[32];
  unsigned char uTouched[32];
  //start with a delay, to make the original diplay appear
  //vTaskDelay(3000 / portTICK_RATE_MS);


  for (;;) {  
  	//clear display
	screenClear();
	response = screenMenu2(menuText1, menuText2);
	sprintf(touched, "Response menu: %d\0", response);
	i = 0;
	while (touched[i] != 0 && i < 32) {
		uTouched[i] = touched[i];
		i++;
	}
	uTouched[i] = 0;
	screenOutputTextLine(Navy, Line9, uTouched);

  	vTaskDelay(2000 / portTICK_RATE_MS);

  
  	//clear display
  	screenClear ();
  	//display lines with text
  	textCol = Black;
  	lineNo = Line0;
  	testText[0] = 'H';
  	testText[14] = '0';
  	//output one line
  	screenOutputTextLine(textCol, lineNo, testText);
  	//change attributes of line
  	textCol = Black;
  	lineNo = Line1;
  	testText[0] = 'h';
  	testText[14] = '1';
  	//output second line
  	screenOutputTextLine(textCol, lineNo, testText);
  	//a third line
  	testText[14] = '2';
  	screenOutputTextLine(Green, Line2, testText);
  	//a fourth line
  	testText[14] = '3';
  	screenOutputTextLine(Yellow, Line3, testText);
  	//the ninth line
  	testText[14] = '4';
  	screenOutputTextLine(Red, Line4, testText);
  	//the ninth line
  	testText[14] = '5';
  	screenOutputTextLine(Olive, Line5, testText);
  	//the ninth line
  	testText[14] = '6';
  	screenOutputTextLine(White, Line6, testText);
  	//the ninth line
  	testText[14] = '7';
  	screenOutputTextLine(Blue, Line7, testText);
  	//the ninth line
  	testText[14] = '8';
  	screenOutputTextLine(Cyan, Line8, testText);
  	//the ninth line
  	testText[14] = '9';
  	screenOutputTextLine(Magenta, Line9, testText);

  	vTaskDelay(5000 / portTICK_RATE_MS);


  	//display text and numbers
  	response = screenQueryNumber(Blue, enterCode, 4);
	sprintf(touched, "%d\0", response);
	i = 0;
	while (touched[i] != 0 && i < 32) {
		uTouched[i] = touched[i];
		i++;
	}
	uTouched[i] = 0;
	screenOutputTextLine(Navy, Line9, uTouched);

	vTaskDelay(3000 / portTICK_RATE_MS);
  


	//diplay text and characters
	screenQueryChars(Blue, enterCode, 20, uTouched);
	screenOutputTextLine(Navy, Line9, uTouched);

	vTaskDelay(3000 / portTICK_RATE_MS);


  	//clear display
	screenClear();
	response = screenMenu3(menuText1, menuText2, menuText3);
	sprintf(touched, "Response menu: %d\0", response);
	i = 0;
	while (touched[i] != 0 && i < 32) {
		uTouched[i] = touched[i];
		i++;
	}
	uTouched[i] = 0;
	screenOutputTextLine(Navy, Line9, uTouched);

  	vTaskDelay(2000 / portTICK_RATE_MS);

  }
}


/**
 *create task for testing screen
 */
void lcdTest() {

  //start a separate task for display test
  xTaskCreate(lcdTestTask, "lcdTestTask", 100, NULL, 1, NULL);

}


/*-----------------------------------------------------------*/

