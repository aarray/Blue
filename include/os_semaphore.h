#ifndef OS_SEMAPHORE_H
#define OS_SEMAPHORE_H

#include <os_define.h>

void SemaphoreCreateBinary(SemaphoreHandle_t *SemaphoreID);
void SemaphoreCreateCounting(SemaphoreHandle_t *SemaphoreID,UBase_t InitValue,UBase_t MaxValue);

void OS_SemaphoreTake(SemaphoreHandle_t *SemaphoreID,UBase_t TimeOut);
#define SemaphoreTake(SemaphoreID,TimeOut) do{\
                                                OS_SemaphoreTake(SemaphoreID,TimeOut);\
                                                yield(0);\
                                            }while(TaskList[RunTaskHandle].yield==Again)

void OS_SemaphoreGive(SemaphoreHandle_t *Semaphore);
#define SemaphoreGive(Semaphore)    OS_SemaphoreGive(Semaphore);yield(0)
#define SemaphoreGiveISR(Semaphore) OS_SemaphoreGive(Semaphore)

#endif /* OS_SEMAPHORE_H */
