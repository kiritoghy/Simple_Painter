#pragma once
#include "shape.h"
#include <list>
#include <Windows.h>
class Paint
{
protected:
	COLORREF ccolor;
	int width;
	//可以利用Paint完成颜色线宽修改
public:
	Paint();
	Paint(COLORREF color);
	~Paint();
	void PaintDown(HWND hwnd, int Type, LPARAM lParam);
	void PaintMove(HWND hwnd, int Type, WPARAM wParam, LPARAM lParam);
	void PaintUp(HWND hwnd, int Type);
	void PaintAll(HWND hwnd);
	
};

