#ifndef OS_TIME_H
#define OS_TIME_H

#include <os_define.h>

void OS_TimeDelay(UBaseType_t tim);
#define TimeDelay(tim)  OS_TimeDelay(tim);yield(0)

void OS_TimeTisk(void);

#endif /* OS_TIME_H */
