// GUICalc.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "GUICalc.h"

#define MAX_LOADSTRING 100
#define INPUT_BOX_START 1000

// Global Variables:
HINSTANCE hInst; // current instance
static TCHAR szTitle[] = _T("Matrix Calulator - Ido Aloni");
static TCHAR szWindowClass[] = _T("DesktopApp");
TCHAR Window_2_Class[] = _T("Win2");

HWND hwnd, hwnd_win2, hwndDialog;

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
void createWin2(WNDCLASSEX, HINSTANCE, int, int, int);
LRESULT CALLBACK Wnd2Proc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Dlgproc(HWND, UINT, WPARAM, LPARAM);


// Calculator variables
FLAGS* flags;
MAT* mat_a_dim, * mat_b_dim;
int** mat_a_val, ** mat_b_val, ** mat_c_val;
int matAsize[] = { 0, 0 }, matBsize[] = { 0, 0 };

void printMat(HWND this_hwnd, int matSize[], int x, int y, int j)
{
    int xOffset = x, yOffset = y;
    int index = INPUT_BOX_START;         // each row index start with 10n0, n = 0, 1, 2, ...
    for (int i = 0; i < matSize[0]; i++) // mat A col
    {
        CreateWindowW(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_TABSTOP, (50 + xOffset), (100 + yOffset), 25, 18, this_hwnd, (HMENU)(index + j), hInst, 0);
        j++;
        for (int i = 0; i < matSize[1]; i++) // mat A col
        {
            CreateWindowW(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_TABSTOP, (50 + xOffset), (100 + yOffset), 25, 18, this_hwnd, (HMENU)(index + j), hInst, 0);
            xOffset += 30;
            j++;
        }
        yOffset += 30;
        xOffset = x;
        index += 10;
        j = 0;
    }
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    flags = (FLAGS*)malloc(sizeof(FLAGS));
    flags->mat_a = false;
    flags->mat_b = false;
    flags->detErr = false;
    mat_a_dim = (MAT*)malloc(sizeof(MAT));
    mat_b_dim = (MAT*)malloc(sizeof(MAT));
    mat_a_dim->n = 0;
    mat_a_dim->m = 0;
    mat_b_dim->n = 0;
    mat_b_dim->m = 0;
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    RegisterClassEx(&wcex);
    hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 500, 200, 500, 500, NULL, NULL, hInst, NULL);

    if (!hwnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindowEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!IsDialogMessage(hwndDialog, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

    }

    return (int)msg.wParam;
}
void createWin2(WNDCLASSEX w2, HINSTANCE hThisInstance, int nCmdShow, int type, int sub_type)
{
    DATA* pData = malloc(sizeof(DATA));
    pData->type = type;
    pData->sub_type = sub_type;

    w2.lpszClassName = Window_2_Class;
    w2.hInstance = hThisInstance;
    w2.lpfnWndProc = Wnd2Proc; /* This function is called by windows */
    w2.style = CS_HREDRAW | CS_VREDRAW;
    ; /* Catch double-clicks */
    w2.cbSize = sizeof(WNDCLASSEX);
    w2.hIcon = LoadIcon(w2.hInstance, IDI_APPLICATION);
    w2.hIconSm = LoadIcon(w2.hInstance, IDI_APPLICATION);
    w2.hCursor = LoadCursor(NULL, IDC_ARROW);
    w2.lpszMenuName = NULL;                        /* No menu */
    w2.cbClsExtra = 0;                             /* No extra bytes after the window class */
    w2.cbWndExtra = 0;                             /* structure or the window instance */
    w2.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); /* Use Windows's default colour as the background of the window */

    if (!hwnd_win2)
    {
        if (!RegisterClassEx(&w2))
        {
            MessageBox(NULL,
                _T("Child window not registered"),
                _T("Error"),
                NULL);

            return 1;
        }
    }

    hwnd_win2 = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, Window_2_Class, _T("Win2"), WS_OVERLAPPEDWINDOW, 550, 350, 200, 200, NULL, NULL, hInst, pData);
    // CreateWindowW(WS_EX_OVERLAPPEDWINDOW, Window_2_Class, _T("Win2"), WS_CHILD | WS_OVERLAPPEDWINDOW, 550, 350, 200, 200, hwnd_win2, flag, hInst, NULL);
    if (!hwnd_win2)
    {
        MessageBox(NULL,
            _T("win2 handle error"),
            _T("Windows Desktop Guided Tour"),
            NULL);
    }
    ShowWindow(hwnd_win2, nCmdShow);
}
LRESULT CALLBACK WndProc(_In_ HWND hwnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{

    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        CreateWindowW(L"Button", L"Matrix A", WS_CHILD | WS_VISIBLE, 100, 50, 100, 50, hwnd, MAT_IN_A, hInst, NULL);
        CreateWindowW(L"Button", L"Matrix B", WS_CHILD | WS_VISIBLE, 300, 50, 100, 50, hwnd, MAT_IN_B, hInst, NULL);
        CreateWindowW(L"Button", L"AXB", WS_CHILD | WS_VISIBLE, 50, 200, 50, 50, hwnd, MAT_MULT, hInst, NULL);
        CreateWindowW(L"Button", L"det(A)", WS_CHILD | WS_VISIBLE, 150, 200, 50, 50, hwnd, DET_A, hInst, NULL);
        CreateWindowW(L"Button", L"det(B)", WS_CHILD | WS_VISIBLE, 250, 200, 50, 50, hwnd, DET_B, hInst, NULL);
        CreateWindowW(L"Button", L"Common Matrix", WS_CHILD | WS_VISIBLE, 320, 200, 150, 50, hwnd, COMM_MAT, hInst, NULL);

        EndPaint(hwnd, &ps);
        break;

    case WM_COMMAND:
    {
        WNDCLASSEX w2 = { 0 };
        if (HIWORD(wParam) == BN_CLICKED)
        {
            switch (LOWORD(wParam))
            {
                // Create window to input matrix size
            case MAT_IN_A:
                flags->mat_a = true;
                createWin2(w2, &hInst, SW_SHOW, MAT_INPUT, MAT_IN_A);
                break;
            case MAT_IN_B:
                flags->mat_b = true;
                createWin2(w2, &hInst, SW_SHOW, MAT_INPUT, MAT_IN_B);
                break;
            case MAT_MULT:
                // handle AXB
                createWin2(w2, &hInst, SW_SHOW, MAT_MULT, NULL);
                break;
            case DET_A:
                // handle detA
                createWin2(w2, &hInst, SW_SHOW, DET_A, NULL);
                break;
            case DET_B:
                // handle detB
                createWin2(w2, &hInst, SW_SHOW, DET_B, NULL);

                break;
            case COMM_MAT:
                // handle common matrices
                break;
            default:
                break;
            }
        }
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}
LRESULT CALLBACK Wnd2Proc(HWND hwndWin2, UINT msg, WPARAM wParam, LPARAM lParam)
{

    PAINTSTRUCT ps;
    HDC hdc;

    switch (msg)
    {

    case WM_CREATE:
    {
        DATA* pData = ((LPCREATESTRUCT)lParam)->lpCreateParams;
        wchar_t buff[256] = { 0 };
        HWND hwndComboN = NULL, hwndComboM = NULL;

        if (pData->type == MAT_INPUT)
        { // Enter Matrix size N*M
            CreateWindowW(L"static", L"# Of Rows", WS_CHILD | WS_VISIBLE, 10, 10, 100, 50, hwndWin2, NULL, hInst, NULL);
            CreateWindowW(L"static", L"# Of Col", WS_CHILD | WS_VISIBLE, 10, 40, 100, 50, hwndWin2, NULL, hInst, NULL);

            switch (pData->sub_type)
            {
            case MAT_IN_A:

                hwndComboN = CreateWindowW(L"Combobox", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 100, 10, 50, 110, hwndWin2, A_ROW_SIZE, hInst, NULL);
                hwndComboM = CreateWindowW(L"Combobox", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 100, 40, 50, 110, hwndWin2, A_COL_SIZE, hInst, NULL);
                CreateWindowW(L"Button", L"Enter", WS_CHILD | WS_VISIBLE, 50, 100, 100, 50, hwndWin2, ENTER_VAL_A, hInst, NULL);

                break;
            case MAT_IN_B:
                hwndComboN = CreateWindowW(L"Combobox", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 100, 10, 50, 110, hwndWin2, B_ROW_SIZE, hInst, NULL);
                hwndComboM = CreateWindowW(L"Combobox", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 100, 40, 50, 110, hwndWin2, B_COL_SIZE, hInst, NULL);
                CreateWindowW(L"Button", L"Enter", WS_CHILD | WS_VISIBLE, 50, 100, 100, 50, hwndWin2, ENTER_VAL_B, hInst, NULL);

                break;
            default:
                break;
            }
            for (int i = 0; i <= 4; i++)
            {
                swprintf_s(buff, 256, L"%d", i);
                SendMessageW(hwndComboM, CB_ADDSTRING, 0, (LPCTSTR)buff);
                SendMessageW(hwndComboN, CB_ADDSTRING, 0, (LPCTSTR)buff);
            }
            SendMessage(hwndComboN, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
            SendMessage(hwndComboM, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
        }
        else if (pData->type == MAT_MULT)
        {
        }
        else if (pData->type == DET_A)
        {
            int res = getDet(mat_a_val,mat_a_dim->n, mat_a_dim, flags);
            if (!flags->detErr)
            {
                swprintf_s(buff, 256, L"%d", res);
                CreateWindowW(L"static", L"det(A) = ", WS_CHILD | WS_VISIBLE, 10, 10, 100, 50, hwndWin2, NULL, hInst, NULL);
                CreateWindowW(L"static", buff, WS_CHILD | WS_VISIBLE, 50, 10, 100, 50, hwndWin2, NULL, hInst, NULL);
            }
        }
        else if (pData->type == DET_B)
        {
            // int res = getDet(mat_b_val, mat_b_dim);
        }
        else if (pData->type == COMM_MAT)
        {
        }
        break;
    }

    case WM_COMMAND:
        if (HIWORD(wParam) == CBN_SELCHANGE) // listen for change in combo box
        {
            LRESULT sz = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
            if (flags->mat_a)
            {
                if (LOWORD(wParam) == A_ROW_SIZE)
                {
                    matAsize[0] = sz;
                }
                else if (LOWORD(wParam) == A_COL_SIZE)
                {
                    matAsize[1] = sz;
                }
                if (matAsize[0] && matAsize[1])
                {
                    mat_a_val = initMat(matAsize);
                    mat_a_dim->n = matAsize[0];
                    mat_a_dim->m = matAsize[1];

                    // flags->mat_a = false;
                }
            }
            if (flags->mat_b)
            {
                if (LOWORD(wParam) == B_ROW_SIZE)
                {
                    matBsize[0] = sz;
                }
                else if (LOWORD(wParam) == B_COL_SIZE)
                {
                    matBsize[1] = sz;
                }
                if (matBsize[0] && matBsize[1])
                {
                    mat_b_dim = initMat(matBsize);
                    flags->mat_b = false;
                    mat_b_dim->n = matBsize[0];
                    mat_b_dim->m = matBsize[1];
                }
            }
        }
        if (HIWORD(wParam) == BN_CLICKED)
        {
            // Create window to input matrix
            hwndDialog = CreateDialogA(hInst, MAKEINTRESOURCEA(IDD_DIALOG1), hwndWin2, (DLGPROC)Dlgproc);
            ShowWindow(hwndDialog, SW_SHOW);
            if (LOWORD(wParam) == ENTER_VAL_A)
            {
                printMat(hwndDialog, matAsize, 0, 0, 0);
                //matAsize[0] = 0;
                //matAsize[1] = 0;
            }
            else if (LOWORD(wParam) == ENTER_VAL_B)
            {
                printMat(hwndDialog, matBsize, 0, 0, 0);
             /*   matBsize[0] = 0;
                matBsize[1] = 0;*/
            }
        }
        break;

    case WM_DESTROY:
        DestroyWindow(hwnd_win2);
        flags->mat_a = false;
        flags->mat_b = false;
        break;

    default:
        return DefWindowProc(hwndWin2, msg, wParam, lParam);
    }
    return 0;
}
INT_PTR CALLBACK Dlgproc(HWND thisHwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_INITDIALOG:
        return true;
        break;
    case WM_COMMAND:
        if (HIWORD(wParam) == EN_CHANGE)
        {
            wchar_t buffer[256] = { 0 };
            WORD cellVal = LOWORD(wParam);
            char cellValStr[256];
            _itoa_s(cellVal, cellValStr, 256, 10);
            GetWindowTextA((HWND)lParam, (LPTSTR)buffer, sizeof(buffer));
            if (flags->mat_a)
            {
                // Save input values in **mat_a_dim
                int val = atoi(buffer);
                int n = cellValStr[2] - 48, m = cellValStr[3] - 48; // 48 is ansi for 0
                mat_a_val[n][m] = val;
            }
            else if (flags->mat_b)
            {
                // Save input values in **mat_b_dim
            }
            break;
        }
        switch (LOWORD(wParam))
        {

        case IDOK:
            DestroyWindow(thisHwnd);
            DestroyWindow(hwnd_win2);
            break;
        case IDCANCEL:
            DestroyWindow(thisHwnd);
            break;
        }
        break;
    case WM_DESTROY:
        DestroyWindow(thisHwnd);
        break;
    default:
        break;
    }
    return false;
}