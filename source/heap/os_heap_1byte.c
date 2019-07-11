#include <os_define.h>

#if HeapSize !=0

uint8_t SysHeap[HeapSize];

typedef struct{
    UBase_t num;
    UBase_t next;
}heap_t;

heap_t Heap={HeapSize,0};

void * os_malloc(UBase_t ByteSize)
{
    void *Rpointer=(void *)0;
	                 
    if(ByteSize <= Heap.num)
    {
        Rpointer=(void *)&SysHeap[Heap.next];
        Heap.next+=ByteSize;
        Heap.num-=ByteSize;
    }
	   
    return Rpointer;
} 

#endif /* HeapSize !=0 */
