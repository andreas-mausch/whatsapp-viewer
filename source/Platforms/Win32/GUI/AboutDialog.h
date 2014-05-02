#pragma once

#include "Dialog.h"

class AboutDialog : public Dialog
{
protected:
	INT_PTR callback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);
public:
	AboutDialog(HWND parent);
};
