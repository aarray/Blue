#ifndef _MYOS_H
#define _MYOS_H

#include <os_define.h>

extern TaskHandle_t RunTaskHandle;
extern tcb_t TaskList[TotalTask];
extern UBase_t TaskNum;

#endif /* _MYOS_H */
