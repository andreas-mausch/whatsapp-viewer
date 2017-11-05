#include <windows.h>
#include <algorithm>

#include "FileDialog.h"
#include "../StringHelper.h"

bool saveFileDialog(HWND window, const std::string &suggestion, const std::string &filter, std::string &filename)
{
	std::string suggestionFixed = suggestion;
	std::replace(suggestionFixed.begin(), suggestionFixed.end(), '/', '-');

	WCHAR filenameW[MAX_PATH];
	memset(filenameW, 0, sizeof(WCHAR) * MAX_PATH);
	wcsncpy_s(filenameW, strtowstr(suggestionFixed).c_str(), suggestionFixed.length());

	WCHAR filterTextW[MAX_PATH];
	memset(filterTextW, 0, sizeof(WCHAR) * MAX_PATH);
	std::wstring filterW = strtowstr(filter);
	wcsncpy_s(filterTextW, MAX_PATH, filterW.c_str(), filterW.length());
	int position = filterW.length() + 1;
	wcsncpy_s(&filterTextW[position], MAX_PATH - position, filterW.c_str(), filterW.length());
	filterTextW[position * 2] = '\0';

	OPENFILENAME openFilename;
	memset(&openFilename, 0, sizeof(OPENFILENAME));
	openFilename.lStructSize = sizeof(OPENFILENAME);
	openFilename.hwndOwner = window;
	openFilename.lpstrFile = filenameW;
	openFilename.nMaxFile = MAX_PATH;
	openFilename.lpstrFilter = filterTextW;
	openFilename.lpstrInitialDir = NULL;
	openFilename.lpstrDefExt = L"";
	openFilename.Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	openFilename.lpstrTitle = L"Save as..";

	if (GetSaveFileName(&openFilename))
	{
		filename = wstrtostr(filenameW);
		return true;
	}

	return false;
}
