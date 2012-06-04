/**
 * Include file for screen routines
 *
 */


#ifndef SCREEN_H
#define SCREEN_H


/*-----------------------------------------------------------*/


/**
 * Initialize the display
 */
void initDisplay (void);


/**
 * Clear diplay - make white
 */
void screenClear (void);


/**
 * Diplay one line of text, max 20 chars
 *
 * lineNo = selected Line1 to Line9 (or y-coordinates)
 * textCol = text color, see file GLCD.h (not all colors work)
 * text = text to print
 */
void screenOutputTextLine(unsigned short textCol, unsigned int lineNo, unsigned char text[]);

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
void screenOutputText(int y, int x, unsigned short textCol, unsigned char text[]);

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
int screenQueryNumber(unsigned short textCol, unsigned char text[], int numberOfDigits);


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
void screenQueryChars(unsigned short textCol, unsigned char text[], int numberOfChars, unsigned char textEntered[]);


/**
 * Display a menu and select menu item number
 * With 2 menu items
 *
 *	text1 - text on line 1 (max 18 characters)
 *	text2 - text on line 2 (max 18 characters)
 *
 *	returns selected menu alternative, 1 or 2
 */
int screenMenu2(unsigned char text1[], unsigned char text2[]);


/**
 * Display a menu and select menu item number
 * With 3 menu items
 *
 *	text1 - text on line 1 (max 18 characters)
 *	text2 - text on line 2 (max 18 characters)
 *	text3 - text on line 3 (max 18 characters)

 *
 *	returns selected menu alternative, 1, 2 or 3
 */
int screenMenu3(unsigned char text1[], unsigned char text2[], unsigned char text3[]);


/**
 * 	Display a line with text and let the user confirm
 *
 *	text - text on line, within rectangle max 18 characters)
 *
 */
int screenTextOk(unsigned char text[] );


#endif
