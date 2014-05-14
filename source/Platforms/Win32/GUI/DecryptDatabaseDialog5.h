#pragma once

#include "OpenDatabaseDialog.h"

class DecryptDatabaseDialog5 : public OpenDatabaseDialog
{
protected:
	virtual INT_PTR callback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);

public:
	DecryptDatabaseDialog5(HWND parent, OpenDatabaseStruct &openDatabaseStruct);
	~DecryptDatabaseDialog5();
};
