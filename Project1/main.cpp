#include <windows.h>
#include "Menu.h"
#include <commdlg.h>
#include <cstdio>
#include <list>
#include "shape.h"
#include "Paint.h"
#include "resource.h"

std::list<Shape*>shapelist; //保存每个图形
std::list<Shape*>::iterator it, itselect;
Paint *paint = new Paint(RGB(0, 0, 0));
Menu *menu = new Menu;
bool Is_Select = 0;
static TCHAR szClassName[] = TEXT("Simple_Painter");
int shapetype;
POINT startPoint, endPoint;


LRESULT CALLBACK DefaultWndProc(
								HWND hwnd,
								UINT uMsg,
								WPARAM wParam,
								LPARAM lParam
								);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	MSG msg;
	HWND hwnd;
	WNDCLASS wcx;
	shapetype = LINE;
;
	ZeroMemory(&wcx, sizeof(wcx));

	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.lpfnWndProc = DefaultWndProc;
	wcx.hInstance = hInstance;
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = szClassName;
	wcx.style = CS_HREDRAW | CS_VREDRAW;

	if(!RegisterClass(&wcx)){
		MessageBox(NULL, TEXT("Program requires Windows NT"), szClassName, MB_ICONERROR);
	}

	hwnd = CreateWindow(
		szClassName,
		TEXT("Simple_Painter"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	while(GetMessage(&msg, hwnd, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}


LRESULT CALLBACK DefaultWndProc(
								HWND hwnd,
								UINT uMsg,
								WPARAM wParam,
								LPARAM lParam
								)
{
	switch(uMsg){
        case WM_COMMAND:
			menu->Command(hwnd, LOWORD(wParam));
            return 0;
		case WM_CREATE:
			menu->CreateWinMenu(hwnd);
            return 0;
		case WM_PAINT:
			paint->PaintAll(hwnd);
            return 0;
		case WM_LBUTTONDOWN:
			paint->PaintDown(hwnd, shapetype, lParam);
			break;
		case WM_LBUTTONUP:
			paint->PaintUp(hwnd, shapetype);
			break;
		case WM_MOUSEMOVE:
			paint->PaintMove(hwnd, shapetype, wParam, lParam);
			break;
		case WM_CLOSE:
		    DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
			PostQuitMessage(0);
			break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

