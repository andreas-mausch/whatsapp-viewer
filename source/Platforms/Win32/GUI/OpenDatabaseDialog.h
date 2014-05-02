#pragma once

struct OpenDatabaseStruct
{
	std::string filename;
	std::string accountName;
};

INT_PTR CALLBACK openDatabaseDialogCallback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK decryptDatabaseDialogCallback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);
