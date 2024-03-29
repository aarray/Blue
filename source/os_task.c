#include <os_define.h>


TaskHandle_t RunTaskHandle=NoTask;

tcb_t TaskList[TotalTask];

UBase_t TaskNum=0;

extern void TickEnabled(void);

#if cfg_use_idletask!=0
	extern void IdleTask(void);
#endif

TaskHandle_t OS_TaskCreate(function_t TaskName,UBase_t prio)
{
	TaskHandle_t buffer=TaskNum;
	int index;
	if(TaskNum<TotalTask)
	{
		if(prio <= 0 || prio > UBase_max)
		{
			return NoTask;
		}

        for(index=0;index<TaskNum;index++)
		{			  
			if(TaskList[index].priority==prio)
			{
				return NoTask;
			}			  
		}
	
	    TaskList[TaskNum].task=TaskName;
		TaskList[TaskNum].state=ready;
		TaskList[TaskNum].SubState=NObject;
		TaskList[TaskNum].priority=prio;
		TaskList[TaskNum].context=0;
		TaskList[TaskNum].yield=Nonstop;		
		TaskList[TaskNum].time=0;
		TaskList[TaskNum].pointer=(void *)0;

		#if cfg_use_event!=0
			TaskList[TaskNum].buffer[0]=0;
			TaskList[TaskNum].buffer[1]=0;
			TaskList[TaskNum].buffer[2]=0;
		#endif /* cfg_use_event */

		TaskNum++;
	}
	else
	{
		
		return NoTask;
	}
	
	return buffer;	
}



void OS_TaskDelete(TaskHandle_t TaskID)
{
	TaskList[TaskID].state=deleted;
	TaskList[TaskID].SubState=NObject;
	TaskList[TaskID].yield=Nonstop;
	TaskList[TaskID].time=0;
	TaskList[TaskID].pointer=(void *)0;
}



void OS_TaskSuspend(TaskHandle_t TaskID)
{
	TaskList[TaskID].CurrentState=TaskList[TaskID].state;
	TaskList[TaskID].state=suspend;
}


void OS_TaskResume(TaskHandle_t TaskID)
{
	TaskList[TaskID].state=TaskList[TaskID].CurrentState;	
}


void OS_FindTask(void)
{
	TaskHandle_t index;
	UBase_t prio=0;
	
	for(index=0;index<TotalTask;index++)
	{
		if(TaskList[index].state==ready)
		{
			if(TaskList[index].priority>prio)
			{			
				RunTaskHandle=index;
				prio=TaskList[index].priority;
			}
		}
	}		
}


void TaskScheduler(void)
{
	TickEnabled();
	while(1)
	{
		OS_FindTask();

		if(RunTaskHandle!=NoTask)
		{
			TaskList[RunTaskHandle].task();
		}	
		else
		{
			#if cfg_use_idletask!=0	
				IdleTask();
			#endif /* cfg_use_idletask */
		}		 
	}
}



