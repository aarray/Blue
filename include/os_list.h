#ifndef OS_LIST_H
#define OS_LIST_H

#include <os_define.h>

UBaseType_t OS_ListEnter(list_t *LISTID,TaskHandle_t TaskID );
UBaseType_t OS_ListExti(list_t *LISTID,TaskHandle_t TaskID);
UBaseType_t OS_ListFind(list_t *LISTID,TaskHandle_t TaskID);

#endif /* OS_LIST_H */
