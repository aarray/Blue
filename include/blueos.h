#ifndef _BLUEOS_H
#define _BLUEOS_H

#include <os_define.h>

#include <os_task.h>
#include <os_time.h>

#if cfg_use_event!=0
    #include <os_event.h>
#endif /* cfg_use_event */

#if cfg_use_queue!=0
    #include <os_queue.h>
#endif /* cfg_use_queue  */

#if cfg_use_semaphore!=0
    #include <os_semaphore.h>
#endif /* cfg_use_semaphore */

#if HeapSize!=0
    #include <os_heap.h>
#endif /* HeapSize */

extern TaskHandle_t RunTaskHandle;
extern tcb_t TaskList[TotalTask];

#endif /* _BLUEOS_H */
