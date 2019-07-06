#include <os_define.h>


UBaseType_t OS_ListEnter(list_t *LISTID,TaskHandle_t TaskID )
{
	UBaseType_t index;

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


UBaseType_t OS_ListExti(list_t *LISTID,TaskHandle_t TaskID)
{
	UBaseType_t index;

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


UBaseType_t OS_ListFind(list_t *LISTID,TaskHandle_t TaskID)
{
	UBaseType_t index;
	for(index=0;index<TotalTask;index++)
	{
		if(LISTID->list[index]==TaskID)
		{
			return index;
		}
	}

	return NoTask;
}
