#ifndef OS_CONFIG_H 
#define OS_CONFIG_H

#include <stdint.h>

#define UBaseType_t   uint32_t
#define UBaseType_max UINT32_MAX
#define MAXTIME UBaseType_max

#define BaseType_t    int32_t
#define BaseType_max  INT32_MAX
#define BaseType_min  INT32_MIN


#define TotalTask   0
#if TotalTask==0
    #error "ToTalTask cannot be defined as 0"
#endif /* TotalTask=0 */


#define cfg_use_idletask 0
#define cfg_use_event 0
#define cfg_use_queue 0
#define cfg_use_semaphore 0

#endif /* OS_CONFIG_H */
