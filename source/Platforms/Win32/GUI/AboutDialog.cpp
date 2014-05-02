#include <windows.h>

#include "AboutDialog.h"
#include "../../../Exceptions/Exception.h"
#include "../../../../resources/resource.h"

AboutDialog::AboutDialog(HWND parent)
	: Dialog(parent, IDD_ABOUT)
{
}

INT_PTR AboutDialog::callback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
    {
		case WM_INITDIALOG:
		{
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
    }

	return 0;
}
