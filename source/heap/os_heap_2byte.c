#include <os_define.h>

#if HeapSize !=0

uint8_t SysHeap[HeapSize];

typedef struct{
    UBase_t num;
    UBase_t next;
}heap_t;

heap_t Heap={0,0};

void OS_HeapAlign(void)
{
    UBase_t index;
    UBase_t buffer;

    for(index=0;index<HeapSize;index++)
    {
        buffer=(uint32_t)&SysHeap[index];
        if( ( buffer % 2 ) ==0 )
        {
            Heap.next=index;
            Heap.num=HeapSize-index;
        }
    }   
}

void * os_malloc(UBase_t ByteSize)
{
    void *Rpointer=(void *)0;
    UBase_t buffer;

    if(Heap.next==0&&Heap.num==0)
    {
        OS_HeapAlign();
    }
    
    if(ByteSize<=2)
    {
        buffer=2;
    }
    else
    {
        if( (ByteSize % 2 ) == 0)
        {
            buffer=( ByteSize / 2 ) * 2;
        }
        else
        {
            buffer=( ByteSize / 2 ) * 2 + 2;
        }        
    }
       
    if(buffer<=Heap.num)
    {
        Rpointer=&SysHeap[Heap.next];
        Heap.next+=buffer;
        Heap.num-=buffer;
    }

    return Rpointer;
} 

#endif /* HeapSize!=0 */
