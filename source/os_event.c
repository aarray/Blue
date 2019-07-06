#include <os_define.h>
#include <os_list.h>

extern TaskHandle_t RunTaskHandle;
extern tcb_t TaskList[TotalTask];

void EventCreate(EventHandle_t *EventID)
{    
    #if CFG_USE_event==0
        #error "Set cfg_use_queue to 1 in the os_config.h file"
    #endif /*CFG_USE_tcbbuffer */
    
    UBaseType_t index;

    for(index=0;index<TotalTask;index++)
    {
        EventID->list[index]=NoTask;
    }
}


void OS_SetState_WaitingEvent(EventHandle_t *EventID,UBaseType_t TaskID,SubState_t substate,UBaseType_t TimeOut)
{
    TaskList[TaskID].state=waiting_event;
    TaskList[TaskID].SubState=substate;
    TaskList[TaskID].yield=Again;
    TaskList[TaskID].time=TimeOut;
    TaskList[TaskID].pointer=EventID;
    
    OS_ListEnter(EventID,TaskID);
}


void OS_ResetState_WaitingEvent(EventHandle_t *EventID,UBaseType_t TaskID)
{
    TaskList[TaskID].state=ready;
    TaskList[TaskID].SubState=NObject;
    TaskList[TaskID].yield=Nonstop;
    TaskList[TaskID].time=0;
    TaskList[TaskID].pointer=(void *)0;
}


void OS_EventWait(EventHandle_t *EventID,UBaseType_t EventBit,UBaseType_t WaitBitAll,UBaseType_t TimeOut)
{
    TaskList[RunTaskHandle].buffer[0]=EventBit;
    if(WaitBitAll==TRUE)
    {
        OS_SetState_WaitingEvent(EventID,RunTaskHandle,EAll,TimeOut);
    }
    else
    {
        OS_SetState_WaitingEvent(EventID,RunTaskHandle,EPart,TimeOut);
    }   
}

void OS_EventSet(EventHandle_t *EventID,UBaseType_t EventBit)
{
    UBaseType_t index;
    UBaseType_t buffer[2];
    int EventWidth=sizeof(UBaseType_t)*8;

    for(index=0;index<TotalTask;index++)
    {
        if(EventID->list[index]!=NoTask && TaskList[EventID->list[index]].state==waiting_event )
        {          
            TaskList[EventID->list[index]].buffer[1]|=EventBit;
            
            TaskList[EventID->list[index]].buffer[1]&=TaskList[EventID->list[index]].buffer[0];

            if(TaskList[EventID->list[index]].SubState==EAll)
            {
                if (TaskList[EventID->list[index]].buffer[1]==TaskList[EventID->list[index]].buffer[0])
                {
                  OS_ResetState_WaitingEvent(EventID,EventID->list[index]);

                  TaskList[EventID->list[index]].buffer[2]=TaskList[EventID->list[index]].buffer[1];
                  TaskList[EventID->list[index]].buffer[1]=0;                    
                }                                
            }
            else
            {                
                for(int i=0;i<EventWidth;i++)
                {
                    buffer[0]=TaskList[EventID->list[index]].buffer[0];
                    buffer[1]=TaskList[EventID->list[index]].buffer[1];

                    if( ( (buffer[0]>>i) & 1 ) == ( (buffer[1]>>i) & 1 ) )
                    {
                        OS_ResetState_WaitingEvent(EventID,EventID->list[index]);
                        
                        TaskList[EventID->list[index]].buffer[2]=TaskList[EventID->list[index]].buffer[1];
                        TaskList[EventID->list[index]].buffer[1]=0; 
                    }
                    
                }
            }            
        }
    }
}


UBaseType_t EventGetBit(EventHandle_t *QueueID,TaskHandle_t TaskID)
{
    return TaskList[TaskID].buffer[2];
}

