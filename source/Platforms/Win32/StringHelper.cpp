#include "StringHelper.h"

std::string wstrtostr(const WCHAR *text)
{
	std::string textString;
	int size = WideCharToMultiByte(CP_UTF8, 0, text, -1, NULL, 0, NULL, NULL);
	char *szText = new char[size];
	memset(szText, 0, size);
	WideCharToMultiByte(CP_UTF8, 0, text, -1, szText, size, NULL, NULL);
	textString = szText;
	delete[] szText;
	return textString;
}

std::wstring strtowstr(const std::string &text)
{
	std::wstring wText;
	WCHAR *szWText = new WCHAR[text.length() + 1];
	szWText[text.size()] = '\0';
	MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, szWText, static_cast<int>(text.length()));
	wText = szWText;
	delete[] szWText;
	return wText;
}

WCHAR *buildWcharString(const std::wstring &text)
{
	WCHAR *wcharString = new WCHAR[text.length() + 1];
	wcscpy_s(wcharString, text.length() + 1, text.c_str());
	return wcharString;
}

WCHAR *buildWcharString(const std::string &text)
{
	return buildWcharString(strtowstr(text));
}
