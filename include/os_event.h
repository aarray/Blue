#ifndef OS_EVENT_H
#define OS_EVENT_H

#include <os_define.h>

void EventCreate(EventHandle_t *EventID);

void OS_EventWait(EventHandle_t *EventID,UBase_t EventBit,UBase_t WaitBitAll,UBase_t TimeOut);
#define EventWait(EventID,EventBit,WaitBitAll,TimeOut) OS_EventWait(EventID,EventBit,WaitBitAll,TimeOut);yield(0)

void OS_EventSet(EventHandle_t *EventID,UBase_t EventBit);
#define EventSetISR(EventID,EventBit)   OS_EventSet(EventID,EventBit)
#define EventSet(EventID,EventBit)  OS_EventSet(EventID,EventBit);yield(0)

UBase_t EventGetBit(EventHandle_t *QueueID,TaskHandle_t TaskID);

#endif /* OS_EVENT_H */
