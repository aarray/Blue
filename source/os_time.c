#include <os_define.h>
#include <os_list.h>

extern TaskHandle_t RunTaskHandle;
extern tcb_t TaskList[TotalTask];

 
void OS_TimeDelay(UBase_t TimeOut)
{
	TaskList[RunTaskHandle].state=waiting_time;
	TaskList[RunTaskHandle].SubState=NObject;
	TaskList[RunTaskHandle].yield=Again;
	TaskList[RunTaskHandle].time=TimeOut;
	TaskList[RunTaskHandle].pointer=(void *)0;
}


void OS_TimeTisk(void)
{
	TaskHandle_t index;

	for(index=0;index<TotalTask;index++)
	{
		if( TaskList[index].time!=0 && TaskList[index].time!=MAXTIME)
			if(TaskList[index].state==waiting_time ||TaskList[index].state==waiting_queue ||TaskList[index].state==waiting_event ||TaskList[index].state==waiting_semaphore)
			{
				TaskList[index].time--;

				if(TaskList[index].time==0)
				{									
					if(TaskList[index].state!=waiting_time)
					{
						OS_ListExti(TaskList[index].pointer,index);																					
					}

					TaskList[index].state=ready;
					TaskList[index].SubState=NObject;
					TaskList[index].yield=Nonstop;
					TaskList[index].pointer=(void *)0;															
				}
			}
	}
}


