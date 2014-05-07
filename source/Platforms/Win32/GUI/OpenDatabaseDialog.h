#pragma once

#include "Dialog.h"
#include "../Objects/Brush.h"

struct OpenDatabaseStruct
{
	std::string filename;
	std::string accountName;
};

class OpenDatabaseDialog : public Dialog
{
private:
	OpenDatabaseStruct &openDatabaseStruct;
	Brush tooltipBrush;
	HWND currentTooltip;

	void selectFile();
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

class DecryptDatabaseDialog : public OpenDatabaseDialog
{
protected:
	virtual INT_PTR callback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);

public:
	DecryptDatabaseDialog(HWND parent, OpenDatabaseStruct &openDatabaseStruct);
	~DecryptDatabaseDialog();
};
