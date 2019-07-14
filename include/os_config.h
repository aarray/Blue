#ifndef OS_CONFIG_H 
#define OS_CONFIG_H

/***************************************************/

#include <stdint.h>

#define UBase_t   uint32_t
#define UBase_max UINT32_MAX
#define MAXTIME   UBase_max

#define Base_t    int32_t
#define Base_max  INT32_MAX
#define Base_min  INT32_MIN

/***************************************************/

#define TotalTask   0
#if TotalTask==0
    #error "ToTalTask cannot be defined as 0"
#endif /* TotalTask */

/* 0:disable   1:enable */
#define cfg_use_idletask 0
#define cfg_use_event 0
#define cfg_use_queue 0
#define cfg_use_semaphore 0

/* bit */
#define HeapSize   0

#endif /* OS_CONFIG_H */
