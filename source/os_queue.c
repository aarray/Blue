#include <os_define.h>
#include <string.h>
#include <os_list.h>

extern TaskHandle_t RunTaskHandle;
extern tcb_t TaskList[TotalTask];


void QueueCreate(QueueHandle_t *QueueID,Queue_t *Queue,uint8_t *QueuePointer,UBaseType_t ItemNum,UBaseType_t ItemSize)
{
	UBaseType_t index;
	#if cfg_use_queue==1
		QueueID->value=0;
		QueueID->CurrentValue=0;
		for(index=0;index<TotalTask;index++)
		{
			QueueID->list[index]=NoTask;
		}
		QueueID->QPointer=Queue;/* QueeueID变量的Qpointer指针成员指向Queue变量 */

		/* Queue_t数据类型的Queue变量的初始化 */	
		QueueID->QPointer->ItemNum=0;
		QueueID->QPointer->ItemSize=ItemSize;
		QueueID->QPointer->ItemLength=(ItemNum+1)*ItemSize;
		QueueID->QPointer->pointer=QueuePointer;
	#else
		#error "Set cfg_use_queue to 1 in the os_config.h file"
	#endif /* cfg_use_queue */
 } 
 

UBaseType_t OS_QueuePush(QueueHandle_t *QueueID,void *source)
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


UBaseType_t OS_QueuePop(QueueHandle_t *QueueID,void *target)
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


void  OS_SetState_WaitingQueue(QueueHandle_t *QueueID,TaskHandle_t TaskID,SubState_t substate,UBaseType_t TimeOut)
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


UBaseType_t OS_QueueAvailableItem(QueueHandle_t *QueueID)
{
	return QueueID->QPointer->ItemNum;  
}



void OS_QueueSend(QueueHandle_t *QueueID,void *source,UBaseType_t TimeOut)
{
	UBaseType_t boo;
	UBaseType_t index;
	UBaseType_t prio=0;
	UBaseType_t buffer=NoTask;

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
	

void OS_QueueReceive(QueueHandle_t *QueueID,void *target,UBaseType_t TimeOut)
{
	UBaseType_t boo;
	UBaseType_t index;
	UBaseType_t prio=0;
	UBaseType_t buffer=NoTask;

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

UBaseType_t QueueSendISR(QueueHandle_t *QueueID,void *source)
{
	UBaseType_t boo;
	UBaseType_t index;
	UBaseType_t prio=0;
	UBaseType_t buffer=NoTask;

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


UBaseType_t QueueReceiveISR(QueueHandle_t *QueueID,void *target)
{
	UBaseType_t boo;
	UBaseType_t index;
	UBaseType_t prio=0;
	UBaseType_t buffer=TotalTask;

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

