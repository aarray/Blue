#include <os_define.h>


UBase_t OS_ListEnter(list_t *LISTID,TaskHandle_t TaskID )
{
	UBase_t index;

	for (index=0;index<TotalTask;index++)
	{
		if(LISTID->list[index]==NoTask)
		{
			LISTID->list[index]=TaskID;
			return SUCCEED;
		}		
	}	
	
	return FAIL;	
}


UBase_t OS_ListExti(list_t *LISTID,TaskHandle_t TaskID)
{
	UBase_t index;

	for(index=0;index<TotalTask;index++)
	{
		if(LISTID->list[index]==TaskID && TaskID!=NoTask)
		{
			LISTID->list[index]=NoTask;	
			return SUCCEED;
		}				
	}
	
	return FAIL;
}


UBase_t OS_ListFind(list_t *LISTID,TaskHandle_t TaskID)
{
	UBase_t index;
	for(index=0;index<TotalTask;index++)
	{
		if(LISTID->list[index]==TaskID)
		{
			return index;
		}
	}

	return NoTask;
}
