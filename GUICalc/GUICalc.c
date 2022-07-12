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
int matAsize[] = { 0, 0 }, matBsize[] = { 0, 0 }; // Holds data until its final.
MAT* mat_a_dim, * mat_b_dim, * mat_c_dim; // Final dimention storage.
int** mat_a_val, ** mat_b_val, ** mat_c_val; // Stores matrix actuall values

/*
    Paints the textbox for enterting the matrix values
*/
void printMatInput(HWND this_hwnd, int matSize[], int x, int y, int j)
{
    int xOffset = x, yOffset = y;
    int index = INPUT_BOX_START;         // each row index start with 10n0, n = 0, 1, 2, ...
    for (int i = 0; i < matSize[0]; i++) // mat A col
    {
        CreateWindowW(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_TABSTOP, (50 + xOffset), (100 + yOffset), 25, 18, this_hwnd, (HMENU)(index + j), hInst, 0); // create first box in row (leftmost)
        j++;
        for (int i = 1; i < matSize[1]; i++) // create rest of row
        {
            xOffset += 30;
            CreateWindowW(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_TABSTOP, (50 + xOffset), (100 + yOffset), 25, 18, this_hwnd, (HMENU)(index + j), hInst, 0);
            j++;
        }
        yOffset += 30;
        xOffset = x;
        index += 10;
        j = 0;
    }
}

/*
    Prints the matrix to the screen
*/
void printMat(HWND this_hwnd, int** mat, MAT* matDim)
{
    int xOffset = 0, yOffset = 0, i = 0, k = 0;
    wchar_t buff[256] = { 0 };
    for (i = 0; i < matDim->n; i++)
    {
        swprintf_s(buff, 256, L"%d", mat[i][k]);
        CreateWindowW(L"static", buff, WS_CHILD | WS_VISIBLE, (10 + xOffset), (10 + yOffset), 25, 18, this_hwnd, NULL, hInst, 0); // create first box in row (leftmost)
        j++;
        for (k = 1; k < matDim->m; k++) 
        {
            xOffset += 20;
            swprintf_s(buff, 256, L"%d", mat[i][k]);
            CreateWindowW(L"static", buff, WS_CHILD | WS_VISIBLE, (10 + xOffset), (10 + yOffset), 25, 18, this_hwnd, NULL, hInst, 0);
            j++;
        }
        yOffset += 20;
        xOffset = 0;
        k = 0;
    }
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    flags = (FLAGS*)malloc(sizeof(FLAGS));
    flags->mat_a = false;
    flags->mat_b = false;
    flags->err = false;
    mat_a_dim = (MAT*)malloc(sizeof(MAT));
    mat_b_dim = (MAT*)malloc(sizeof(MAT));
    mat_c_dim = (MAT*)malloc(sizeof(MAT));
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
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
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

/*
* Create secondary window. this function get "type" and "subtype" to determain which kinds of window is requested
*/
void createWin2(WNDCLASSEX w2, HINSTANCE hThisInstance, int nCmdShow, int type, int sub_type)
{
    DATA* pData = malloc(sizeof(DATA)); 
    pData->type = type;
    pData->sub_type = sub_type;

    w2.lpszClassName = Window_2_Class;
    w2.hInstance = hThisInstance;
    w2.lpfnWndProc = Wnd2Proc;
    w2.style = CS_HREDRAW | CS_VREDRAW;
    ; /* Catch double-clicks */
    w2.cbSize = sizeof(WNDCLASSEX);
    w2.hIcon = LoadIcon(w2.hInstance, IDI_APPLICATION);
    w2.hIconSm = LoadIcon(w2.hInstance, IDI_APPLICATION);
    w2.hCursor = LoadCursor(NULL, IDC_ARROW);
    w2.lpszMenuName = NULL;
    w2.cbClsExtra = 0;
    w2.cbWndExtra = 0;
    w2.hbrBackground = (HBRUSH)(COLOR_WINDOW);

    if (!hwnd_win2)
    {
        if (!RegisterClassEx(&w2))
        {
            MessageBox(NULL,
                _T("New Class not registered"),
                _T("Error"),
                NULL);

            return 1;
        }
    }

    hwnd_win2 = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, Window_2_Class, _T("Win2"), WS_OVERLAPPEDWINDOW, 550, 350, 300, 300, NULL, NULL, hInst, pData);
    if (!hwnd_win2)
        MessageBox(NULL, _T("New window handle error"), _T("Error"), NULL);
    
    if (!flags->err) { // if there was an error during CreateWindowEX, dont show window
        ShowWindow(hwnd_win2, nCmdShow);
        return;
    }
    DestroyWindow(hwnd_win2);
    flags->err = false;
}

/*
* Main window proc
*/
LRESULT CALLBACK WndProc(_In_ HWND hwnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{

    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {

    case WM_PAINT:
        // Paint main window
        hdc = BeginPaint(hwnd, &ps);
        CreateWindowW(L"Button", L"Matrix A", WS_CHILD | WS_VISIBLE, 100, 50, 100, 50, hwnd, MAT_IN_A, hInst, NULL);
        CreateWindowW(L"Button", L"Matrix B", WS_CHILD | WS_VISIBLE, 300, 50, 100, 50, hwnd, MAT_IN_B, hInst, NULL);
        CreateWindowW(L"Button", L"AXB", WS_CHILD | WS_VISIBLE, 50, 200, 50, 50, hwnd, MAT_MULT, hInst, NULL);
        CreateWindowW(L"Button", L"det(A)", WS_CHILD | WS_VISIBLE, 150, 200, 50, 50, hwnd, DET_A, hInst, NULL);
        CreateWindowW(L"Button", L"det(B)", WS_CHILD | WS_VISIBLE, 250, 200, 50, 50, hwnd, DET_B, hInst, NULL);
        CreateWindowW(L"Button", L"Common Matrix", WS_CHILD | WS_VISIBLE, 320, 200, 150, 50, hwnd, COMM_MAT, hInst, NULL);
        CreateWindowW(L"Button", L"Print A", WS_CHILD | WS_VISIBLE, 150, 300, 50, 50, hwnd, PRINT_A, hInst, NULL);
        CreateWindowW(L"Button", L"Print B", WS_CHILD | WS_VISIBLE, 250, 300, 50, 50, hwnd, PRINT_B, hInst, NULL);

        EndPaint(hwnd, &ps);
        break;
        
    case WM_COMMAND:
    {
        WNDCLASSEX w2 = { 0 }; // window class placeholder
        if (HIWORD(wParam) == BN_CLICKED)
        {
            switch (LOWORD(wParam))
            {
            case MAT_IN_A: // Create window to input matrix size
                matAsize[0] = 0;
                matAsize[1] = 0;
                mat_a_dim->n = 0;
                mat_a_dim->m = 0;
                flags->mat_a = true;
                createWin2(w2, &hInst, SW_SHOW, MAT_INPUT, A);
                break;
            case MAT_IN_B: // Create window to input matrix size
                matBsize[0] = 0;
                matBsize[1] = 0;
                mat_b_dim->n = 0;
                mat_b_dim->m = 0;
                flags->mat_b = true;
                createWin2(w2, &hInst, SW_SHOW, MAT_INPUT, B);
                break;

            case MAT_MULT:
                createWin2(w2, &hInst, SW_SHOW, MAT_MULT, NULL);
                break;
            case DET_A:
                createWin2(w2, &hInst, SW_SHOW, DET, A);
                break;
            case DET_B:
                createWin2(w2, &hInst, SW_SHOW, DET, B);
                break;
            case COMM_MAT:
                // TODO handle common matrices
                break;
            case PRINT_A:
                createWin2(w2, &hInst, SW_SHOW, PRINT_MAT, A);
                break;
            case PRINT_B:
                createWin2(w2, &hInst, SW_SHOW, PRINT_MAT, B);
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

/*
* Secondary window proc. lParam hold data about type of window that is requested
*/
LRESULT CALLBACK Wnd2Proc(HWND hwndWin2, UINT msg, WPARAM wParam, LPARAM lParam)
{

    switch (msg)
    {

    case WM_CREATE:
    {
        // Get lParam data
        DATA* pData = ((LPCREATESTRUCT)lParam)->lpCreateParams;
        wchar_t buff[256] = { 0 };
        HWND hwndComboN = NULL, hwndComboM = NULL;

        if (pData->type == MAT_INPUT)
        {
            CreateWindowW(L"static", L"# Of Rows", WS_CHILD | WS_VISIBLE, 10, 10, 100, 50, hwndWin2, NULL, hInst, NULL);
            CreateWindowW(L"static", L"# Of Col", WS_CHILD | WS_VISIBLE, 10, 40, 100, 50, hwndWin2, NULL, hInst, NULL);

            switch (pData->sub_type)
            {
                // Create comboboxes to select matrix dimentions
            case A:
                hwndComboN = CreateWindowW(L"Combobox", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 100, 10, 50, 110, hwndWin2, A_ROW_SIZE, hInst, NULL);
                hwndComboM = CreateWindowW(L"Combobox", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 100, 40, 50, 110, hwndWin2, A_COL_SIZE, hInst, NULL);
                CreateWindowW(L"Button", L"Enter", WS_CHILD | WS_VISIBLE, 50, 100, 100, 50, hwndWin2, ENTER_VAL_A, hInst, NULL);
                break;
            case B:
                hwndComboN = CreateWindowW(L"Combobox", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 100, 10, 50, 110, hwndWin2, B_ROW_SIZE, hInst, NULL);
                hwndComboM = CreateWindowW(L"Combobox", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 100, 40, 50, 110, hwndWin2, B_COL_SIZE, hInst, NULL);
                CreateWindowW(L"Button", L"Enter", WS_CHILD | WS_VISIBLE, 50, 100, 100, 50, hwndWin2, ENTER_VAL_B, hInst, NULL);
                break;
            default:
                break;
            }
            // Fill combobox with options
            for (int i = 0; i <= 4; i++)
            {
                swprintf_s(buff, 256, L"%d", i);
                SendMessageW(hwndComboM, CB_ADDSTRING, 0, (LPCTSTR)buff);
                SendMessageW(hwndComboN, CB_ADDSTRING, 0, (LPCTSTR)buff);
            }
            SendMessage(hwndComboN, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
            SendMessage(hwndComboM, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
        }
        else if (pData->type == DET) // Calculate determinant
        {
            int res;
            switch (pData->sub_type)
            {
            case A:
                res = getDet(mat_a_val, mat_a_dim->n, mat_a_dim, flags);
                if (!flags->err)
                {
                    swprintf_s(buff, 256, L"%d", res);
                    CreateWindowW(L"static", L"det(A) = ", WS_CHILD | WS_VISIBLE, 10, 10, 100, 50, hwndWin2, NULL, hInst, NULL);
                    CreateWindowW(L"static", buff, WS_CHILD | WS_VISIBLE, 50, 10, 100, 50, hwndWin2, NULL, hInst, NULL);
                }
                break;
            case B:
                res = getDet(mat_b_val, mat_b_dim->n, mat_b_dim, flags);
                if (!flags->err)
                {
                    swprintf_s(buff, 256, L"%d", res);
                    CreateWindowW(L"static", L"det(A) = ", WS_CHILD | WS_VISIBLE, 10, 10, 100, 50, hwndWin2, NULL, hInst, NULL);
                    CreateWindowW(L"static", buff, WS_CHILD | WS_VISIBLE, 50, 10, 100, 50, hwndWin2, NULL, hInst, NULL);
                }
                break;
            default:
                break;
            }
        }
        else if (pData->type == MAT_MULT) // Multiply A and B. product stored in C.
        {
            mat_c_val = matMultiply(mat_a_val, mat_b_val, mat_a_dim, mat_b_dim, flags);
            if (flags->err)
                break;
            mat_c_dim->n = mat_a_dim->n;
            mat_c_dim->m = mat_b_dim->m;
            printMat(hwndWin2, mat_c_val, mat_c_dim);
        }
        else if (pData->type == COMM_MAT)
        {
        }
        else if (pData->type == PRINT_MAT) // Print matrix to screen
        {
            switch (pData->sub_type)
            {
            case A:
                if (!mat_a_dim->n || !mat_a_dim->m) {
                    MessageBox(NULL, _T("Matrix not initialized"), _T("Error"), NULL);
                    flags->err = true;
                    break;
                }
                printMat(hwndWin2, mat_a_val, mat_a_dim);
                break;
            case B:
                if (!(mat_a_dim->n) || !(mat_b_dim->m)) {
                    MessageBox(NULL, _T("Matrix not initialized"), _T("Error"), NULL);
                    flags->err = true;
                    break;
                }
                hwndDialog = CreateDialogA(hInst, MAKEINTRESOURCEA(IDD_DIALOG1), hwndWin2, (DLGPROC)Dlgproc);
                printMat(hwndWin2, mat_b_val, mat_b_dim);
                break;
            default:
                break;
            }
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
                    mat_a_val = initMat(matAsize); // once both dimentions are set, store it in MAT struct and initialize matrix
                    mat_a_dim->n = matAsize[0];
                    mat_a_dim->m = matAsize[1];
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
                if (matBsize[0] && matBsize[1]) // once both dimentions are set, store it in MAT struct and initialize matrix
                {
                    mat_b_val = initMat(matBsize);
                    mat_b_dim->n = matBsize[0];
                    mat_b_dim->m = matBsize[1];
                }
            }
        }
        if (HIWORD(wParam) == BN_CLICKED)
        {
            // paint textbox for matrix input
            hwndDialog = CreateDialogA(hInst, MAKEINTRESOURCEA(IDD_DIALOG1), hwndWin2, (DLGPROC)Dlgproc);
            ShowWindow(hwndDialog, SW_SHOW);
            if (LOWORD(wParam) == ENTER_VAL_A)
            {
                printMatInput(hwndDialog, matAsize, 0, 0, 0);
            }
            else if (LOWORD(wParam) == ENTER_VAL_B)
            {
                printMatInput(hwndDialog, matBsize, 0, 0, 0);
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


}/*
 * Dialog proc for input proccesing
 */
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
                int val = atoi(buffer);
                int n = cellValStr[2] - 48, m = cellValStr[3] - 48; // 48 is ansi for 0
                mat_b_val[n][m] = val;
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