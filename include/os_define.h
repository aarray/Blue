#ifndef OS_DEFINE_H 
#define OS_DEFINE_H 

#include <os_config.h>

#define TaskHandle_t       UBase_t
#define EventHandle_t      list_t
#define SemaphoreHandle_t  list_t
#define QueueHandle_t      list_t
#define Queue_t			   QList_t

#define NoTask             TotalTask

typedef void (*function_t)(void);

typedef enum {	
	FALSE=0,
	TRUE=1,	
	FAIL=0,
	SUCCEED=1,
}logic_t;

typedef enum{
	Again,
	HihterTask,
	Nonstop
}yield_t;

typedef enum{
	NObject,
	QSender,
	QReceiver,
	EAll,
	EPart,
	SFull,
	SEmpty
}SubState_t;

typedef enum {
	ready,
	waiting_time,
	waiting_queue,
	waiting_event,
	waiting_semaphore,
	suspend,
	deleted
}TaskState_t;


#if cfg_use_queue!=0
	typedef struct{
		UBase_t ItemNum;
		UBase_t ItemSize;
		UBase_t ItemLength;
		uint8_t *pointer;
	}QList_t;
#endif /* cfg_use_queue */


typedef struct {
		UBase_t list[TotalTask];
	#if cfg_use_semaphore!=0||cfg_use_queue!=0		
		UBase_t value;
		UBase_t CurrentValue;
	#endif /* cfg_use_semaphore||cfg_use_queue */

	#if cfg_use_queue!=0	
		QList_t *QPointer;
	#endif/* cfg_use_queue */
}list_t;


typedef struct{
	function_t  task;
	UBase_t priority;
	UBase_t context;
	TaskState_t state;
	TaskState_t CurrentState;
	SubState_t  SubState;
	UBase_t yield;
	UBase_t time;
	list_t *pointer;

	#if cfg_use_event!=0
		UBase_t buffer[3];
	#endif /* cfg_use_event */

}tcb_t;


#endif /* OS_DEFINE_H */
