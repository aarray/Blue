#include <os_define.h>
#include <string.h>
#include <os_list.h>

#if cfg_use_queue!=0

extern TaskHandle_t RunTaskHandle;
extern tcb_t TaskList[TotalTask];

void QueueCreate(QueueHandle_t *QueueID,Queue_t *Queue,unsigned char *QueuePointer,UBase_t ItemNum,UBase_t ItemSize)
{
		UBase_t index;
		QueueID->value=0;
		QueueID->CurrentValue=0;

		for(index=0;index<TotalTask;index++)
		{
			QueueID->list[index]=NoTask;
		}
		
		QueueID->QPointer=Queue;
	
		QueueID->QPointer->ItemNum=0;
		QueueID->QPointer->ItemSize=ItemSize;
		QueueID->QPointer->ItemLength=(ItemNum+1)*ItemSize;
		QueueID->QPointer->pointer=QueuePointer;
 } 
 

UBase_t OS_QueuePush(QueueHandle_t *QueueID,void *source)
{
	if( ( (QueueID->CurrentValue+QueueID->QPointer->ItemSize) % QueueID->QPointer->ItemLength ) == QueueID->value )
	{
		return FAIL;
	}
	else
	{
		memcpy((void *)&QueueID->QPointer->pointer[QueueID->CurrentValue],source,QueueID->QPointer->ItemSize); 
		
		QueueID->CurrentValue=(QueueID->CurrentValue+QueueID->QPointer->ItemSize)%QueueID->QPointer->ItemLength;
		
		QueueID->QPointer->ItemNum++; 
		
		return SUCCEED;
	}
	
}


UBase_t OS_QueuePop(QueueHandle_t *QueueID,void *target)
{
	if( QueueID->CurrentValue == QueueID->value)
	{
		return FAIL;
	}
	else
	{
		memcpy(target,(void *)&QueueID->QPointer->pointer[QueueID->value],QueueID->QPointer->ItemSize);

		QueueID->value=(QueueID->value+QueueID->QPointer->ItemSize)%QueueID->QPointer->ItemLength;

		QueueID->QPointer->ItemNum--;

		return SUCCEED;
	} 
 } 


void  OS_SetState_WaitingQueue(QueueHandle_t *QueueID,TaskHandle_t TaskID,SubState_t substate,UBase_t TimeOut)
{
	TaskList[TaskID].state=waiting_queue;
	TaskList[TaskID].SubState=substate;
	TaskList[TaskID].time=TimeOut;
	TaskList[TaskID].pointer=QueueID;
	TaskList[TaskID].yield=Again;

	OS_ListEnter(QueueID,TaskID);
}


void  OS_ResetState_WaitingQueue(QueueHandle_t *QueueID,TaskHandle_t TaskID)
{
	TaskList[TaskID].state=ready;
	TaskList[TaskID].SubState=NObject;
	TaskList[TaskID].pointer=(void *)0;
	TaskList[TaskID].time=0;

	OS_ListExti(QueueID,TaskID);
}


UBase_t OS_QueueAvailableItem(QueueHandle_t *QueueID)
{
	return QueueID->QPointer->ItemNum;  
}



void OS_QueueSend(QueueHandle_t *QueueID,void *source,UBase_t TimeOut)
{
	UBase_t boo;
	UBase_t index;
	UBase_t prio=0;
	UBase_t buffer=NoTask;

	boo=OS_QueuePush(QueueID,source);

	if(boo==FAIL)
	{
		if(TimeOut!=0)
		{
			OS_SetState_WaitingQueue(QueueID,RunTaskHandle,QSender,TimeOut);						
		}
		else
		{
			TaskList[RunTaskHandle].yield=Nonstop;
		}
				
	}
	else
	{
		TaskList[RunTaskHandle].yield=Nonstop;

		for(index=0;index<TotalTask;index++)
		{
			if(QueueID->list[index]!=NoTask)
			{				
				if(TaskList[QueueID->list[index]].SubState==QReceiver)
				{	
					if(TaskList[QueueID->list[index]].state==waiting_queue)
					{
						if(TaskList[QueueID->list[index]].priority>prio)
						{
							prio=TaskList[QueueID->list[index]].priority;
							buffer=QueueID->list[index];
						}
					}
				}					
			}						
		}

		if(buffer!=NoTask)
		{
			OS_ResetState_WaitingQueue(QueueID,buffer);			
		}		
	}
		  
} 
	

void OS_QueueReceive(QueueHandle_t *QueueID,void *target,UBase_t TimeOut)
{
	UBase_t boo;
	UBase_t index;
	UBase_t prio=0;
	UBase_t buffer=NoTask;

	boo=OS_QueuePop(QueueID,target);

	if(boo==FAIL)
	{
		if(TimeOut!=0)
		{			
			OS_SetState_WaitingQueue(QueueID,RunTaskHandle,QReceiver,TimeOut);			
		}
		else
		{
			TaskList[RunTaskHandle].yield=Nonstop;
		}
	}
	else
	{
		TaskList[RunTaskHandle].yield=Nonstop;

		for(index=0;index<TotalTask;index++)
		{
			if(QueueID->list[index]!=NoTask)
			{								
				if(TaskList[QueueID->list[index]].SubState==QSender)
				{				
					if(TaskList[QueueID->list[index]].state==waiting_queue)
					{
						if(TaskList[QueueID->list[index]].priority>prio)
						{
							prio=TaskList[QueueID->list[index]].priority;
							buffer=QueueID->list[index];
						}
					}
				}				
			}						
		}

		if(buffer!=NoTask)
		{		
			OS_ResetState_WaitingQueue(QueueID,buffer);			
		}		
	}
	
}

UBase_t QueueSendISR(QueueHandle_t *QueueID,void *source)
{
	UBase_t boo;
	UBase_t index;
	UBase_t prio=0;
	UBase_t buffer=NoTask;

	boo=OS_QueuePush(QueueID,source);

	if (boo==FAIL)
	{
		return FAIL;
	}
	else
	{
		for (index=0;index<TotalTask;index++)
		{
			if(QueueID->list[index]!=NoTask)
			{
				if(TaskList[QueueID->list[index]].state==waiting_queue)
				{
					if(TaskList[QueueID->list[index]].SubState==QReceiver)
					{
						if(TaskList[QueueID->list[index]].priority>prio)
						{
							prio=TaskList[QueueID->list[index]].priority;
							buffer=QueueID->list[index];
						}
					}
				}
			}
		}

		if(buffer!=NoTask)
		{
			OS_ResetState_WaitingQueue(QueueID,QueueID->list[index]);
		}
		
		return SUCCEED;
	}		
}


UBase_t QueueReceiveISR(QueueHandle_t *QueueID,void *target)
{
	UBase_t boo;
	UBase_t index;
	UBase_t prio=0;
	UBase_t buffer=TotalTask;

	boo=OS_QueuePop(QueueID,target);
	
	if (boo==FAIL)
	{
		return FAIL;
	}
	else
	{
		for (index=0;index<TotalTask;index++)
		{
			if(QueueID->list[index]!=NoTask)
			{
				if(TaskList[QueueID->list[index]].state==waiting_queue)
				{
					if(TaskList[QueueID->list[index]].SubState==QSender)
					{
						if(TaskList[QueueID->list[index]].priority>prio)
						{
							prio=TaskList[QueueID->list[index]].priority;
							buffer=QueueID->list[index];
						}
					}
				}
			}
		}

		if(buffer!=NoTask)
		{
			OS_ResetState_WaitingQueue(QueueID,QueueID->list[index]);
		}
		
		return SUCCEED;
	}		
}

#endif /* cfg_use_queue!=0 */
