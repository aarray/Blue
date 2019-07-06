#include <os_define.h>


TaskHandle_t RunTaskHandle=NoTask;

tcb_t TaskList[TotalTask];

UBaseType_t TaskNum=0;

extern void TickEnabled(void);

#if CFG_USE_idletask==1
	extern void IdleTask(void);
#endif

TaskHandle_t OS_TaskCreate(function_t TaskName,UBaseType_t prio)
{
	TaskHandle_t buffer=TaskNum;
	
	if(TaskNum<TotalTask)
	{
		if(prio <= 0 || prio > UBaseType_max)
		{
			return NoTask;
		}

        for(int index=0;index<TaskNum;index++)
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

		#if CFG_USE_event==1
			TaskList[TaskNum].buffer[0]=0;
			TaskList[TaskNum].buffer[1]=0;
			TaskList[TaskNum].buffer[2]=0;
		#endif /* CFG_USE_event */

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
	UBaseType_t prio=0;
	
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
			#if CFG_USE_idletask==1	
				IdleTask();
			#endif /* CFG_USE_idletask */
		}		 
	}
}



