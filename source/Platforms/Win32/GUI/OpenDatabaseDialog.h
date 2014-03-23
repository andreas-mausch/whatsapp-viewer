#pragma once

struct OpenFileDialogStruct
{
	std::string filename;
	std::string accountName;
};

INT_PTR CALLBACK openFileCallback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);
