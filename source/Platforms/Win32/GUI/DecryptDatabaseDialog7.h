#pragma once

#include "OpenDatabaseDialog.h"

class DecryptDatabaseDialog7 : public Dialog
{
private:
	OpenDatabaseStruct &openDatabaseStruct;

	void selectDatabaseFile();
	void selectKeyFile();
	void clickOk(WPARAM wParam);

protected:
	virtual INT_PTR callback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);

public:
	DecryptDatabaseDialog7(HWND parent, OpenDatabaseStruct &openDatabaseStruct);
	~DecryptDatabaseDialog7();

	const std::string &getFilename();
	const std::string &getKeyFilename();

};
