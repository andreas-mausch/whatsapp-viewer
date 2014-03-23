#pragma once

#include <string>
#include <windows.h>

std::wstring strtowstr(const std::string &text);
WCHAR *buildWcharString(const std::wstring &text);
WCHAR *buildWcharString(const std::string &text);

std::string wstrtostr(const WCHAR *text);
