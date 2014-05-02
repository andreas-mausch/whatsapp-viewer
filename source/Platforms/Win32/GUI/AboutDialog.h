#pragma once

#include "Dialog.h"
#include "../Objects/Font.h"
#include "../Objects/Icon.h"

class AboutDialog : public Dialog
{
private:
	Font headingFont;
	Icon icon;

	void onPaint();
	void sendCloseMessage(int code);

protected:
	INT_PTR callback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);

public:
	AboutDialog(HWND parent);
};
