#pragma once

struct OpenDatabaseStruct
{
	std::string filename;
	std::string accountName;
};

INT_PTR CALLBACK openDatabaseCallback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);
