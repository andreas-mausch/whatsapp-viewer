#pragma once

#include <string>

#include "Dialog.h"
#include "../Objects/Brush.h"

struct OpenDatabaseStruct
{
	std::string filename;
	std::string accountName;
	std::string keyFilename;
	std::string wadbFilename;
};

class OpenDatabaseDialog : public Dialog
{
private:
	OpenDatabaseStruct &openDatabaseStruct;
	Brush tooltipBrush;
	HWND currentTooltip;

	void selectFile();
	void selectWadbFile();
	void clickOk(WPARAM wParam);
	void hideTooltips();
	void showTooltip(int id);

protected:
	virtual INT_PTR callback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);

public:
	OpenDatabaseDialog(HWND parent, OpenDatabaseStruct &openDatabaseStruct);
	~OpenDatabaseDialog();

	const std::string &getFilename();
	const std::string &getAccountName();

};
