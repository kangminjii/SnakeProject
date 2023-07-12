#include "framework.h"
#include "SnakeProject.h"
#include "Snake.h"
#include <time.h>
#include <iostream>
#include <string>
using namespace std;
#define MAX_LOADSTRING 100

//WinAPI 사용하면서 콘솔창 동시에 띄우기

//#ifdef UNICODE
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
//#else
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
//#endif

// 두 좌표간의 거리
double LengthPts(POINT pt1, POINT pt2)
{
    return (sqrt((float)(pt2.x - pt1.x) * (pt2.x - pt1.x) + (pt2.y - pt1.y) * (pt2.y - pt1.y)));
}
// 충돌 판정
BOOL InCircle(POINT pt1, POINT pt2)
{
    if (LengthPts(pt1, pt2) <= 10)  return TRUE;

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
    if (!InitInstance(hInstance, nCmdShow))
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

    return (int)msg.wParam;
}


//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKEPROJECT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SNAKEPROJECT);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
        700, 300, 500, 500, nullptr, nullptr, hInstance, nullptr);

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
    HBRUSH hBrush, headBrush, oldBrush;

    static RECT rectView;

    // snake
    static Snake s[100];               // snake의 위치 좌표

    // 아이템
    static int count = 0;                    // 몸통 수
    static int random;                // 랜덤 좌표
    static POINT items[100];      // 아이템 위치 좌표
    static int tempX[100], tempY[100]; // 아이템 위치 좌표 임시 저장배열
    static BOOL isCollided = FALSE;

    // 키 이벤트
    enum { Left, Right, Up, Down, None };
    static int state = None;
    static BOOL isClicked = FALSE;

    // 점수 UI
    static TCHAR str[20];
    static SIZE size;
    string score = std::to_string(count * 100);

    //  시작 화면
    enum { START, GAME, NONE };
    static int screen = START;

    // 종료 이벤트
    static int death = 0;

    switch (message)
    {
    case WM_CREATE:
        GetClientRect(hWnd, &rectView); // 윈도우창 크기값을 rectView에 저장
        SetTimer(hWnd, 1, 200, NULL); // 타이머 설정, 속도 조절

        // 아이템
        srand(time(NULL));
        random = 50 * (rand() % 6 + 1); // 50, 100, 150, 200, 250, 300
        items[0] = { random, random };

        break;

    case WM_TIMER:
    {
        ////////////  ALIVE
        if (death == 0)
        {
            // 이전 위치 저장
            for (int i = 0; i < count; i++)
            {
                tempX[i] = s[i].GetX();
                tempY[i] = s[i].GetY();
            }

            // 머리 업데이트
            s[0].Update(rectView);

            // 새롭게 생긴 몸통의 위치와 속도를 지정
            for (int i = 0; i < count; i++)
            {
                s[i + 1].SetPosition(tempX[i], tempY[i]);
                s[i + 1].SetDirection(s[i].getDirectionX(), s[i].getDirectionY());
            }
        }

        ///////////  DEATH
        // 경계선에 닿았을 때, 초기화
        if (screen == GAME && isClicked == TRUE)
        {
            if (s[0].getDirectionX() == 0 && s[0].getDirectionY() == 0)
            {
                // 죽음 이벤트
                death = 1;
                isClicked = FALSE;

                // UI 초기화
                for (int i = 0; i < count; i++)
                    str[i] = 0;

                // 뱀 길이, 위치 초기화
                count = 0;
                s[0].SetPosition(50, 100);

                // 시작화면으로
                screen = START;
                break;
            }
        }
        // 몸통에 닿았을 때, 초기화
        for (int i = 1; i <= count; i++)
        {
            if (InCircle({ s[0].GetX(), s[0].GetY() }, { s[i].GetX(), s[i].GetY() }))
            {
                // 죽음 이벤트
                death = 1;
                isClicked = FALSE;

                // UI 초기화
                for (int i = 0; i < count; i++)
                    str[i] = 0;

                // 뱀 길이, 위치 초기화
                count = 0;
                s[0].SetPosition(50, 100);
                s[0].SetDirection(0, 0);

                // 시작화면으로
                screen = START;
                break;
            }
        }

        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;

    case WM_KEYDOWN: // 눌리면 발생
    {
        // START
        if (wParam == VK_RETURN)
        {
            screen = GAME;
        }

        // GAME
        if (screen == GAME && wParam != VK_RETURN)
        {
            isClicked = TRUE;

            if (GetAsyncKeyState('A') & 0x8000)
            {
                if (state != Right)
                {
                    s[0].SetDirection(-20, 0); // 좌로
                    state = Left;
                }
            }
            if (GetAsyncKeyState('D') & 0x8000)
            {
                if (state != Left)
                {
                    s[0].SetDirection(20, 0); // 우로
                    state = Right;
                }
            }
            if (GetAsyncKeyState('W') & 0x8000)
            {
                if (state != Down)
                {
                    s[0].SetDirection(0, -20); // 위로
                    state = Up;
                }
            }
            if (GetAsyncKeyState('S') & 0x8000)
            {
                if (state != Up)
                {
                    s[0].SetDirection(0, 20); // 아래로
                    state = Down;
                }
            }
        }

        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;

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

        // GAME OVER
        if (death == 1)
        {
            KillTimer(hWnd, 1);

            int gameOver = MessageBox(hWnd, _T("Restart?"), _T("Game Over"), MB_OKCANCEL);
            if (gameOver == IDOK) // 재시작
            {
                death = 0;
                SetTimer(hWnd, 1, 200, NULL);
            }
            else // 종료
                PostMessage(hWnd, WM_CLOSE, 0, 0);

            break;
        }

        // START
        if (screen == START)
        {
            TextOut(hdc, 200, 150, _T("SNAKE GAME"), _tcslen(_T("SNAKE GAME")));
            TextOut(hdc, 200, 180, _T("Press Enter ..."), _tcslen(_T("Press Enter ...")));
        }

        // GAME
        if (screen == GAME)
        {
            ////////////  SCORE 그리기
            // string을 TCHAR로 변환
            for (int i = 0; i < score.size(); i++)
                str[i] = score[i];

            TextOut(hdc, 150, 10, _T("Score : "), _tcslen(_T("Score : ")));
            TextOut(hdc, 200, 10, str, _tcslen(str));


            ///////////  SNAKE 그리기
            // 머리
            headBrush = CreateSolidBrush(RGB(0, 0, 255));
            oldBrush = (HBRUSH)SelectObject(hdc, headBrush);
            s[0].Draw(hdc);

            // 몸통
            hBrush = CreateSolidBrush(RGB(255, 255, 255));
            oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
            for (int i = 1; i <= count; i++)
                s[i].Draw(hdc);

            // 머리가 항상 위에 있게
            oldBrush = (HBRUSH)SelectObject(hdc, headBrush);
            s[0].Draw(hdc);


            //////////  ITEM 그리기
            // 아이템
            hBrush = CreateSolidBrush(RGB(255, 0, 0));
            oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
            Ellipse(hdc, items[count].x - 10, items[count].y - 10, items[count].x + 10, items[count].y + 10);

            // 아이템 랜덤 좌표 생성
            random = 50 * (rand() % 6 + 1); // 50, 100, 150, 200, 250, 300

            // 아이템을 먹었다
            if (InCircle(items[count], { s[0].GetX(), s[0].GetY() }))
            {
                count++;
                items[count] = { 100 + random, 100 + random };

                // 머리가 항상 위에 있게
                oldBrush = (HBRUSH)SelectObject(hdc, headBrush);
                s[0].Draw(hdc);
            }

            /////////  BRUSH delete
            SelectObject(hdc, oldBrush);
            DeleteObject(hBrush);
            DeleteObject(headBrush);
        }

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