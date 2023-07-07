// SnakeProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "SnakeProject.h"
#include "Snake.h"
#include <list>
#include <time.h>

#define MAX_LOADSTRING 100
#define timer_ID_1 11

double LengthPts(POINT pt1, POINT pt2)
{
    return (sqrt((float)(pt2.x - pt1.x) * (pt2.x - pt1.x) + (pt2.y - pt1.y) * (pt2.y - pt1.y)));
}

BOOL InCircle(POINT pt1, POINT pt2)
{
    if (LengthPts(pt1, pt2) < 10)  return TRUE;

    return FALSE;
}

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SNAKEPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SNAKEPROJECT));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//WinAPI 사용하면서 콘솔창 동시에 띄우기

//#ifdef UNICODE
//
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
//
//#else
//
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
//
//#endif


//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKEPROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SNAKEPROJECT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      300, 100, 700, 700, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    static POINT center;
    static POINT ptMousePos;

    static RECT rectView;

    static Snake s[100];
    static Snake items[100];

    static int count;
    static int snakeCount;
    static int random;
   
    static BOOL isClicked = FALSE;
    static BOOL isCollided = FALSE;
    
    //static std::list<Snake> s[100];

    switch (message)
    {
    case WM_CREATE:
        center = { 0,0 };
        GetClientRect(hWnd, &rectView); // 윈도우창 크기값을 rectView에 저장함
        
        SetTimer(hWnd, 1, 100, NULL);

        count = 0;
        snakeCount = 0;
        random = 0;

        //Snake* temp = &s[0];
       
        break;
    case WM_TIMER: // 타이머 이벤트, 타이머는 일이 바쁘지 않을때만 잘 작동됨
    {
        s[0].Update(rectView);
        for (int i = 0; i <= count; i++)
        {
            // 링크드 리스트처럼 연결
            s[i + 1].SetNext(&s[i]);
        }
        for (int i = 0; i <= count; i++)
        {
            s[i + 1].SetPosition(s[i].GetX() - s[0].getDirectionX() * 4, s[i].GetY() - s[0].getDirectionY() * 4);
        }
        InvalidateRect(hWnd, NULL, TRUE);
    }
        break;
    case WM_KEYDOWN: // 눌리면 발생
    {
        // : wm_keydown, wm_keyup을 사용하면 반응이 즉각적이지 않음 > VK_DOWN?
        // : GetKeyState는 "키가 눌렸는가?"와 "키의 토글상태가 무엇인가?"를 알아낼 때 사용한다. 지속적인 상태를 확인할때 사용
        // : GetAsyncKeyState는 "키가 눌렸는가?"와 "언제부터 눌렸는가?"를 알아낼 때 사용한다. 순간적인 상태를 확인할때 사용
        
        count++;
        isClicked = TRUE;

        if (GetAsyncKeyState('A') & 0x8000)
        {
            s[0].SetDirection(-5, 0); // 좌로
        }
        else if (GetAsyncKeyState('D') & 0x8000)
        {
            s[0].SetDirection(5, 0); // 우로
        }
        else if (GetAsyncKeyState('W') & 0x8000)
        {
            s[0].SetDirection(0, -5); // 위로
        }
        else if (GetAsyncKeyState('S') & 0x8000)
        {
            s[0].SetDirection(0, 5); // 아래로
        }

        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;
    case WM_KEYUP: // 눌렀다 떼면 발생
    {
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);

            if (isClicked == TRUE)
            {
                for (int i = 0; i < count; i++)
                    s[i].Draw(hdc);
            }




            
           // // 랜덤아이템
           // srand(time(NULL));
           // random = 100 + 20 * (rand() % 10 + 1);

           // HBRUSH hBrush, oldBrush;
           // hBrush = (HBRUSH)GetStockObject(RGB(0,0,255)); 
           // oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
          
           // if(count == 0)
           //     Ellipse(hdc, random - 10, random - 10, random + 10, random + 10);

           //// 아이템을 먹었다
           // if (InCircle({random, random}, {s[0].GetX(), s[0].GetY()}))
           // {
           //     count++;
           //     Ellipse(hdc, random - 10, random - 10, random + 10, random + 10);
           // }
           //    
           // SelectObject(hdc, oldBrush);
           // DeleteObject(hBrush);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

