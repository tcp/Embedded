/**
 * Alarm program for the course "Programming embedded systems"
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
//#include "semphr.h"
//#include "queue.h"
#include "setup.h"
//#include "assert.h"
#include "GLCD.h"
//#include "stm3210c_eval_ioe.h"
//#include "stm32f10x_gpio.h"

//#include "global.h"
#include "screen.h"
//#include "screen_test.h"
//#include "alarm.h"
//#include "alarm_test.h"
//#include "master_alarm.h"
#include "configure_alarm.h"
#include "i2c_ee.h"

/* Private variables ---------------------------------------------------------*/
uint8_t Tx1_Buffer[BufferSize+1];
uint8_t Rx1_Buffer[BufferSize];
uint8_t Buffer[BufferSize];
uint8_t flag[1];
volatile TestStatus TransferStatus = FAILED;

/*-----------------------------------------------------------*/

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *   FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;  
}

void AssignBuffer(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    *pBuffer1 = *pBuffer2;    
    pBuffer1++;
    pBuffer2++;
  }	
}

void Int2Buf(int number, uint8_t* pBuffer)
{
	pBuffer[3]=number/1000;
	pBuffer[2]=(number/100)%10;
	pBuffer[1]=(number/10)%10;
	pBuffer[0]=number%10;
}

/*-----------------------------------------------------------*/

/**
 * Main configuration routine - called from master_alarm
 *
 */
void configureAlarm(void) {

	//configure - and store alarm values, like secret code

	//statusReturn used for temporary response from other routines
	int statusReturn;

	unsigned char menuText0[] = "Configuration code:\0";
	unsigned char menuText1[] = "Secret code:\0";
	unsigned char menuText2[] = "Signal delay:\0";
	unsigned char message0[] = "Wrong code!\0";
	unsigned char message1[] = "Changes not allowed\0";

 /* Read from I2C EEPROM from 0x49 */
  I2C_EE_BufferRead(flag, 0x49, 1);
	
	/* No flag at the beginning */
	if(flag[0]!=0 && flag[0]!=1)
	{
		/* No secret code at the beginning */
		flag[0]=0;
		/* Write flag number on I2C EEPROM */
		I2C_EE_BufferWrite(flag, 0x49, 1);
	}
	
	/* If there is no secret code in EEPROM */
	if(flag[0]==0)
	{
		//ask for secret code
		statusReturn = screenQueryNumber(Blue, menuText1, 8);
		
		// integer to buffer
		Int2Buf (statusReturn, Buffer);
		
		AssignBuffer(Tx1_Buffer, Buffer, BufferSize);
		
		/* Write on I2C EEPROM from EEPROM_WriteAddress */
		I2C_EE_BufferWrite(Tx1_Buffer, EEPROM_WriteAddress, BufferSize); 

		//ask about allowed waiting time
		statusReturn = screenQueryNumber(Blue, menuText2, 3);
		//check, max 256 seconds (240 - 4 minutes should be enough)

		/* secret code has been set */
		flag[0]=1;
		I2C_EE_BufferWrite(flag, 0x49, 1);
	}
	/* If secret code has been set */
	else if(flag[0]==1)
	{		
		//check to see the user is allowed to change things
		statusReturn = screenQueryNumber(Blue, menuText0, 4);
		//code hardcoded now, better if stored
		
		// integer to buffer
		Int2Buf (statusReturn, Buffer);
		
		/* Read from I2C EEPROM from EEPROM_ReadAddress */
		I2C_EE_BufferRead(Rx1_Buffer, EEPROM_ReadAddress, BufferSize);
		
		/* Check if the data written to the memory is read correctly */
		TransferStatus = Buffercmp(Buffer, Rx1_Buffer, BufferSize);

		if(TransferStatus) 
		{
			//ok, allowed
			//ask for secret code
			statusReturn = screenQueryNumber(Blue, menuText1, 8);
			
			// integer to buffer
			Int2Buf (statusReturn, Buffer);
			
			AssignBuffer(Tx1_Buffer, Buffer, BufferSize);
			
			/* Write on I2C EEPROM from EEPROM_WriteAddress */
			I2C_EE_BufferWrite(Tx1_Buffer, EEPROM_WriteAddress, BufferSize); 

			//ask about allowed waiting time
			statusReturn = screenQueryNumber(Blue, menuText2, 3);
			//check, max 256 seconds (240 - 4 minutes should be enough)
					
			flag[0]=1;
			I2C_EE_BufferWrite(flag, 0x49, 1);
		} 
		else 
		{
			//wrong code
			screenClear();
			screenOutputTextLine(Red, Line3, message0);
			screenOutputTextLine(Red, Line4, message1);

			vTaskDelay(900 / portTICK_RATE_MS);			
		}
	}
	
}
/*-----------------------------------------------------------*/


