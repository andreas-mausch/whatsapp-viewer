#include <windows.h>

#include "AboutDialog.h"
#include "../StringHelper.h"
#include "../../../Exceptions/Exception.h"
#include "../../../../resources/resource.h"

AboutDialog::AboutDialog(HWND parent)
	: Dialog(parent, IDD_ABOUT),
	headingFont(CreateFont(30, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Verdana")),
	icon(MAKEINTRESOURCE(IDI_ICON), 128, 128)
{
	readVersionInformation();
}

void AboutDialog::readVersionInformation()
{
	HRSRC resource = FindResource(NULL, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
	DWORD size = SizeofResource(NULL, resource);
	HGLOBAL memoryHandle = LoadResource(NULL, resource);
	void *data = LockResource(memoryHandle);

	void *versionString = NULL;
	unsigned int versionStringLength = 0;
	if (VerQueryValue(data, L"\\StringFileInfo\\000004b0\\ProductVersion", &versionString, &versionStringLength))
	{
		WCHAR *versionStringW = static_cast<WCHAR *>(versionString);
		version = wstrtostr(versionStringW);
	}
}

void AboutDialog::onPaint()
{
	RECT clientRect;
	GetClientRect(dialog, &clientRect);

	PAINTSTRUCT paint;
	HDC deviceContext = BeginPaint(dialog, &paint);
	DrawIconEx(deviceContext, 10, 20, icon.get(), 0, 0, 0, NULL, DI_NORMAL);
	EndPaint(dialog, &paint);
}

void AboutDialog::sendCloseMessage(int code)
{
	SendMessage(parent, WM_DIALOG, MAKEWPARAM(DIALOG_CLOSED, code), reinterpret_cast<LPARAM>(this));
}

INT_PTR AboutDialog::callback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
		{
			SendDlgItemMessage(dialog, IDC_ABOUT_APPLICATION_NAME, WM_SETFONT, reinterpret_cast<WPARAM>(headingFont.get()), TRUE);

			std::wstring versionW = strtowstr(version);
			SetDlgItemText(dialog, IDC_ABOUT_APPLICATION_VERSION, versionW.c_str());

			ShowWindow(dialog, SW_SHOW);
		} break;
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDOK:
				case IDCANCEL:
				{
					sendCloseMessage(LOWORD(wParam));
					return TRUE;
				} break;
			}
		} break;
		case WM_PAINT:
		{
			onPaint();
		} break;
	}

	return 0;
}
