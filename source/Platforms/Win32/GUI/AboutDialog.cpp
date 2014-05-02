#include <windows.h>

#include "AboutDialog.h"

INT_PTR CALLBACK aboutDialogCallback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
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
					EndDialog(dialog, LOWORD(wParam));
					return TRUE;
				} break;
            }
		} break;
    }

	return 0;
}
