#pragma once

enum ListViewShowArrow
{
	LISTVIEW_SHOW_NO_ARROW,
	LISTVIEW_SHOW_UP_ARROW,
	LISTVIEW_SHOW_DOWN_ARROW
};

BOOL xListViewSetSortArrow(HWND listViewHandle, int columnIndex, ListViewShowArrow showArrow);
