#ifndef OS_TASK_H
#define OS_TASK_H

#include <os_define.h> 

#define TaskEnter UBase_t cont=TaskList[RunTaskHandle].context;switch(cont){ case 0:
#define TaskExti  TaskList[RunTaskHandle].state=deleted;RunTaskHandle=NoTask;return;}
#define yield(num)  do{TaskList[RunTaskHandle].context=__LINE__+num;RunTaskHandle=NoTask;return;case (__LINE__+num):;}while(0)

TaskHandle_t OS_TaskCreate(function_t TaskName,UBase_t prio);
#define TaskCreate(TaskName,prio)       OS_TaskCreate(TaskName,prio)

void OS_TaskDelete(TaskHandle_t TaskID);
#define TaskDelete(TaskID)  OS_TaskDelete(TaskID);yield(0)

void TaskScheduler(void);

void OS_TaskSuspend(TaskHandle_t TaskID);
#define TaskSuspend(TaskID) OS_TaskSuspend(TaskID);yield(0)

void OS_TaskResume(TaskHandle_t TaskID);
#define TaskResume(TaskID)  OS_TaskResume(TaskID);yield(0)

#endif /* 0S_TASK_H */
