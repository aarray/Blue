#ifndef OS_QUEUE_H
#define OS_QUEUE_H

#include <os_define.h>

void QueueCreate(QueueHandle_t *QueueID,Queue_t *Queue,uint8_t *QueuePointer,UBase_t ItemNum,UBase_t ItemSize);

UBase_t OS_QueueAvailableItem(QueueHandle_t *QueueID);
#define QueueAvailableItem(QueueID) OS_QueueAvailableItem(QueueHandle_t *QueueID)

UBase_t OS_QueueSend(QueueHandle_t *QueueID,void *source,UBase_t TimeOut);
UBase_t QueueSendISR(QueueHandle_t *QueueID,void *source);
#define QueueSend(QueueID,source,TimeOut)   do{\
                                                OS_QueueSend(QueueID,source,TimeOut);\
                                                yield(0);\
                                             }while (TaskList[RunTaskHandle].yield==Again)


UBase_t OS_QueueReceive(QueueHandle_t *QueueID,void *target,UBase_t TimeOut);
UBase_t QueueReceiveISR(QueueHandle_t *QueueID,void *target);
#define QueueReceive(QueueID,target,TimeOut) do{\
                                                OS_QueueReceive(QueueID,target,TimeOut);\
                                                yield(0);\
                                            }while (TaskList[RunTaskHandle].yield==Again)
                                            
#endif /* OS_QUEUE_H */
