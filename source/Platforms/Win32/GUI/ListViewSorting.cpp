#include <windows.h>
#include <commctrl.h>

#include "ListViewSorting.h"

BOOL xListViewSetSortArrow(HWND listViewHandle, int columnIndex, ListViewShowArrow showArrow)
{
	HWND headerHandle = NULL;
	HDITEM headerItem = { 0 };

	headerHandle = ListView_GetHeader(listViewHandle);

	if (!headerHandle)
	{
		return FALSE;
	}

	headerItem.mask = HDI_FORMAT;
	if (!Header_GetItem(headerHandle, columnIndex, &headerItem))
	{
		return FALSE;
	}

	if (showArrow == LISTVIEW_SHOW_UP_ARROW)
	{
		headerItem.fmt = (headerItem.fmt & ~HDF_SORTDOWN) | HDF_SORTUP;
	}
	else if (showArrow == LISTVIEW_SHOW_DOWN_ARROW)
	{
		headerItem.fmt = (headerItem.fmt & ~HDF_SORTUP) | HDF_SORTDOWN;
	}
	else 
	{
		headerItem.fmt = headerItem.fmt & ~(HDF_SORTDOWN | HDF_SORTUP);
	}

	return Header_SetItem(headerHandle, columnIndex, &headerItem);
}
