#include <os_define.h>

#if HeapSize !=0

unsigned char SysHeap[HeapSize];

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
        if( ( buffer % 4 ) == 0 )
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
    
    if(ByteSize<=4)
    {
        buffer=4;
    }
    else
    {
        if( (ByteSize % 4 ) == 0)
        {
            buffer=( ByteSize / 4 ) * 4;
        }
        else
        {
            buffer=( ByteSize / 4 ) * 4 + 4;
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
