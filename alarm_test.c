/**
 * Alarm routines
 */

#include "FreeRTos.h"
#include "task.h"

#include "alarm.h"
#include "queue.h"
#include "global.h"
#include "task.h"




static void alarmTaskTest(void *params) {
	int alarmEventSend;


	for (;;) {
		vTaskDelay(5000 / portTICK_RATE_MS);
	
 		alarmEventSend = DOOR_ALARM1;
		xQueueSend(alarmEventQueue, &alarmEventSend, portMAX_DELAY);

		vTaskDelay(10000 / portTICK_RATE_MS);
 		alarmEventSend = DEACTIVATE_ALARM;
		xQueueSend(alarmEventQueue, &alarmEventSend, portMAX_DELAY);		
		
				
   }    //forever loop ends here

}




void setupAlarmTest(unsigned portBASE_TYPE uxPriority) {
	//create task that waits for eventual alarms
	xTaskCreate(alarmTaskTest,
	"alarmTest",
	100,
	NULL,
	uxPriority,
	NULL);
}
