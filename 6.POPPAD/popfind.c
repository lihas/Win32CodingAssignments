#include <Windows.h>
#include<commdlg.h>
#include<tchar.h>

#define MAX_STRING_LEN 256

static TCHAR szFindText[MAX_STRING_LEN];
static TCHAR szReplText[MAX_STRING_LEN];

HWND PopFindFindDlg(HWND hwnd)
{
    static FINDREPLACE fr;
    fr.lStructSize = sizeof(FINDREPLACE);
    fr.hwndOwner = hwnd;
    fr.hInstance = NULL;
    fr.Flags = FR_HIDEUPDOWN | FR_HIDEMATCHCASE | FR_HIDEWHOLEWORD;
    fr.lpstrFindWhat = szFindText;
    fr.lpstrReplaceWith = NULL;
    fr.wFindWhatLen = MAX_STRING_LEN;
    fr.wReplaceWithLen = 0;
    fr.lCustData = 0;
    fr.lpfnHook = NULL;
    fr.lpTemplateName = NULL;

    return FindText(&fr);
}

HWND PopFindReplaceDlg(HWND hwnd)
{
    static FINDREPLACE fr;
    fr.lStructSize = sizeof(FINDREPLACE);
    fr.hwndOwner = hwnd;
    fr.hInstance = NULL;
    fr.Flags = FR_HIDEUPDOWN | FR_HIDEMATCHCASE | FR_HIDEWHOLEWORD;
    fr.lpstrFindWhat = szFindText;
    fr.lpstrReplaceWith = szReplText;
    fr.wFindWhatLen = MAX_STRING_LEN;
    fr.wReplaceWithLen = MAX_STRING_LEN;
    fr.lCustData = 0;
    fr.lpfnHook = NULL;
    fr.lpTemplateName = NULL;

    return ReplaceText(&fr);
}

BOOL PopFindFindText(HWND hwndEdit, int * piSearchOffset, LPFINDREPLACE pfr)
{
    int iLengh, iPos;
    PTSTR pstrDoc, pstrPos;

    //Read in the edit document
    iLengh = GetWindowTextLength(hwndEdit);
    if (NULL == (pstrDoc = (PTSTR)malloc((iLengh + 1) * sizeof(TCHAR))))
        return FALSE;

    GetWindowText(hwndEdit, pstrDoc, iLengh + 1);

    //Search the document for the find string

    pstrPos = _tcsstr(pstrDoc + *piSearchOffset, pfr->lpstrFindWhat);
    free(pstrDoc);

    //Return an error code if the string cannot be found

    if (pstrPos == NULL)
    {
        return FALSE;
    }

    iPos = pstrPos - pstrDoc;
    *piSearchOffset = iPos + lstrlen(pfr->lpstrFindWhat);

    //select the found text

    SendMessage(hwndEdit, EM_SETSEL, iPos, *piSearchOffset);
    SendMessage(hwndEdit, EM_SCROLLCARET, 0, 0);
    return TRUE;
}

BOOL PopFindNextText(HWND hwndEdit, int* piSearchOffset)
{
    FINDREPLACE fr;
    fr.lpstrFindWhat = szFindText;
    return PopFindFindText(hwndEdit, piSearchOffset, &fr);
}

BOOL PopFindReplaceText(HWND hwndEdit, int* piSearchOffset, LPFINDREPLACE pfr)
{
    //Find the text
    if (!PopFindFindText(hwndEdit, piSearchOffset, pfr))
        return FALSE;

    //Replace it
    SendMessage(hwndEdit, EM_REPLACESEL, 0, (LPARAM)pfr->lpstrReplaceWith);
    return TRUE;
}

BOOL PopFindValidFind(void)
{
    return *szFindText != '\0';
}
