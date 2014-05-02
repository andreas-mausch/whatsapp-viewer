#include <windows.h>

#include "AboutDialog.h"
#include "../../../Exceptions/Exception.h"
#include "../../../../resources/resource.h"

AboutDialog::AboutDialog(HWND parent)
	: Dialog(parent, IDD_ABOUT),
	headingFont(CreateFont(30, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Verdana")),
	icon(MAKEINTRESOURCE(IDI_ICON), 128, 128)
{
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

INT_PTR AboutDialog::callback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
    {
		case WM_INITDIALOG:
		{
			SendDlgItemMessage(dialog, IDC_ABOUT_APPLICATION_NAME, WM_SETFONT, reinterpret_cast<WPARAM>(headingFont.get()), TRUE);
			ShowWindow(dialog, SW_SHOW);
		} break;
        case WM_COMMAND:
		{
            switch (LOWORD(wParam))
            {
                case IDOK:
				{
					SendMessage(parent, WM_DIALOG, MAKEWPARAM(DIALOG_CLOSED, LOWORD(wParam)), reinterpret_cast<LPARAM>(this));
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
