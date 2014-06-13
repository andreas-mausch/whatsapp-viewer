#include "SettingsRegistry.h"
#include "StringHelper.h"
#include "../../Exceptions/Exception.h"
#include "../../Exceptions/KeyNotFoundException.h"

#include <windows.h>

SettingsRegistry::SettingsRegistry()
{
}

SettingsRegistry::~SettingsRegistry()
{
}

std::string SettingsRegistry::read(const std::string &key)
{
	HKEY keyHandle;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\WhatsApp Viewer", 0, KEY_READ, &keyHandle) != ERROR_SUCCESS)
	{
		throw Exception("Could not open registry key");
	}

	DWORD type;
	DWORD size;
	std::wstring keyW = strtowstr(key);
	if (RegQueryValueEx(keyHandle, keyW.c_str(), NULL, &type, NULL, &size) != ERROR_SUCCESS)
	{
		RegCloseKey(keyHandle);
		throw KeyNotFoundException("Could not read registry value");
	}

	if (type != REG_SZ)
	{
		RegCloseKey(keyHandle);
		throw Exception("Incorrect registry value type");
	}

	std::wstring value(size / sizeof(wchar_t), L'\0');
	if (RegQueryValueEx(keyHandle, keyW.c_str(), NULL, NULL, reinterpret_cast<BYTE *>(&value[0]), &size) != ERROR_SUCCESS)
	{
		RegCloseKey(keyHandle);
		throw Exception("Could not read registry value");
	}

	RegCloseKey(keyHandle);

	return wstrtostr(value.c_str());
}

void SettingsRegistry::write(const std::string &key, const std::string &value)
{
	HKEY keyHandle;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\WhatsApp Viewer", 0, NULL, 0, KEY_SET_VALUE, NULL, &keyHandle, NULL) != ERROR_SUCCESS)
	{
		throw Exception("Could not create registry key");
	}

	std::wstring keyW = strtowstr(key);
	std::wstring valueW = strtowstr(value);
	if (RegSetValueEx(keyHandle, keyW.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE *>(valueW.c_str()), valueW.size() * sizeof(WCHAR)) != ERROR_SUCCESS)
	{
		RegCloseKey(keyHandle);
		throw Exception("Could not write registry value");
	}

	RegCloseKey(keyHandle);
}
