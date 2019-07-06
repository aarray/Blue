#include <os_define.h>
#include <os_list.h>

extern TaskHandle_t RunTaskHandle;
extern tcb_t TaskList[TotalTask];


void SemaphoreCreateBinary(SemaphoreHandle_t *SemaphoreID)
{
    UBaseType_t index;

    #if cfg_use_semaphore==1
        SemaphoreID->value=1;
        SemaphoreID->CurrentValue=0;
    #else
        #error "Set cfg_use_semaphore to 1 in the os_config.h file"
    #endif/* cfg_use_semaphore */

    for(index=0;index<TotalTask;index++)
    {
        SemaphoreID->list[index]=NoTask;
    }
}



void SemaphoreCreateCounting(SemaphoreHandle_t *SemaphoreID,UBaseType_t MaxValue,UBaseType_t InitValue)
{
    UBaseType_t index;

    #if cfg_use_semaphore==1
        SemaphoreID->value=MaxValue;
        SemaphoreID->CurrentValue=InitValue;
    #else
        #error "Set cfg_use_semaphore to 1 in the os_config.h file"
    #endif/* cfg_use_semaphore */

    for(index=0;index<TotalTask;index++)
    {
        SemaphoreID->list[index]=NoTask;
    }
}



void OS_SetState_WaitingSemaphore(SemaphoreHandle_t *SemaphoreID,UBaseType_t TaskID,SubState_t substate,UBaseType_t TimeOut)
{
    TaskList[TaskID].state=waiting_semaphore;
	TaskList[TaskID].SubState=substate;
	TaskList[TaskID].time=TimeOut;
	TaskList[TaskID].pointer=SemaphoreID;
	TaskList[TaskID].yield=Again;

    OS_ListEnter(SemaphoreID,TaskID);
}



void OS_ResetState_WaitingSemaphore(SemaphoreHandle_t *SemaphoreID,UBaseType_t TaskID)
{
    TaskList[TaskID].state=ready;
	TaskList[TaskID].SubState=NObject;
	TaskList[TaskID].pointer=(void *)0;
	TaskList[TaskID].time=0;

    OS_ListExti(SemaphoreID,TaskID);
}


void OS_SemaphoreTake(SemaphoreHandle_t *SemaphoreID,UBaseType_t TimeOut)
{
    if(SemaphoreID->CurrentValue!=0)
    {
        SemaphoreID->CurrentValue--;
        TaskList[RunTaskHandle].yield=Nonstop;
    }
    else
    {
        OS_SetState_WaitingSemaphore(SemaphoreID,RunTaskHandle,SEmpty,TimeOut);
    }    
}


void OS_SemaphoreGive(SemaphoreHandle_t *SemaphoreID)
{
		UBaseType_t index;
		UBaseType_t waken=NoTask;
		UBaseType_t prio=0;
    if(SemaphoreID->CurrentValue!=SemaphoreID->value)
    {
        SemaphoreID->CurrentValue++;
        TaskList[RunTaskHandle].yield=Nonstop;

        for(index=0;index<TotalTask;index++)
        {
            if(SemaphoreID->list[index]!=NoTask)
            {
                if(TaskList[SemaphoreID->list[index]].state==waiting_semaphore)
                {
                    if (TaskList[SemaphoreID->list[index]].priority>prio)
                    {
                        prio=TaskList[SemaphoreID->list[index]].priority;
                        waken=SemaphoreID->list[index];
                    }                    
                }
            }
        }

        if(waken!=NoTask)
        {
            OS_ResetState_WaitingSemaphore(SemaphoreID,waken);
        }
    }
}
