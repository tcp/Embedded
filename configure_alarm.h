/**
 * Include file for the main alarm task
 */

#ifndef CONFIGURE_ALARM_H
#define CONFIGURE_ALARM_H

//#include "FreeRTOS.h"
//#include "task.h"
//#include <stm32f10x.h>

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define EEPROM_WriteAddress    0x50
#define EEPROM_ReadAddress     0x50
#define BufferSize             5

TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
void AssignBuffer(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
void Int2Buf(int number, uint8_t* pBuffer);
void configureAlarm(void);


#endif

