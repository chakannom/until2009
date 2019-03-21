#include <windows.h>
#include "SearchName.h"
#include "resource.h"

#define SUBWAY_NAME_MAX 20

extern char Search_Name_Start[];
extern char Search_Name_End[];

BOOL CALLBACK SearchNameProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch(wParam) {

		case IDOK:
			GetDlgItemText(hDlg, IDC_START, Search_Name_Start, SUBWAY_NAME_MAX);
			GetDlgItemText(hDlg, IDC_END, Search_Name_End, SUBWAY_NAME_MAX);
			EndDialog(hDlg,IDOK);
			return TRUE;

		case IDCANCEL:
			EndDialog(hDlg,IDCANCEL);
			return TRUE;
		}
	}

	return FALSE;
}