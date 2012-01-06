/*
Elise Messages Plugin for Miranda IM

*/

#include "options.h"
#include "resource.h"


static INT_PTR CALLBACK DlgProcOptions(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_INITDIALOG:
            {
                return TRUE;
            }
            break;
        case WM_COMMAND:
            {

            }
            break;
        case WM_NOTIFY:
            {

            }
            break;
        case WM_DESTROY:
            break;
    }
    return FALSE;
}

// Options init
int EliseOptInit(WPARAM wParam, LPARAM lParam)
{
    /* проблем
        OPTIONSDIALOGPAGE odp = { 0 };

        odp.cbSize = sizeof(odp);
        odp.position = 0;
        odp.hInstance = hEliseInst;
        odp.pszTemplate = MAKEINTRESOURCEA(IDD_GENERAL_OPT);
        odp.ptszGroup = TranslateT("Message Sessions");
        odp.ptszTab = TranslateT("eliseTab");
        odp.ptszTitle = TranslateT("Elise Messages");
        odp.pfnDlgProc = DlgProcOptions;
        odp.flags = ODPF_BOLDGROUPS | ODPF_TCHAR;
        CallService(MS_OPT_ADDPAGE, wParam, (LPARAM) & odp);

        */

    return 0;
}
