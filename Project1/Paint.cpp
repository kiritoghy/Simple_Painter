#include "Paint.h"
#include "resource.h"

Shape* pshape;
extern POINT startPoint, endPoint;
extern std::list<Shape*>shapelist; //保存每个图形
extern std::list<Shape*>::iterator it, itselect;
extern bool Is_Select;
bool LMouseDown;

Paint::Paint()
{
	pshape = NULL;
}

Paint::Paint(COLORREF color)
{
	ccolor = color;
}


Paint::~Paint()
{
}

void Paint::PaintDown(HWND hwnd, int Type, LPARAM lParam)
{
	startPoint.x = LOWORD(lParam);
	startPoint.y = HIWORD(lParam);
	endPoint = startPoint;
	if (Type == SELECT) {
		SetClassLong(hwnd, GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_HAND));//设置光标
		for (it = shapelist.begin(); it != shapelist.end(); ++it) {
			HDC hdc = GetDC(hwnd);
			if ((*it)->SelectShape(hdc, endPoint)) {
				Is_Select = 1;
				itselect = it;
				InvalidateRect(hwnd, NULL, TRUE);
			}
			ReleaseDC(hwnd, hdc);
		}
		return;
	}
	else {
		LMouseDown = true;
		SetClassLong(hwnd, GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));//设置光标
		switch (Type) {
		case LINE:
			pshape = new LineDraw(startPoint, endPoint, ccolor, 1);
			break;
		case CIRCLE:
			pshape = new EllipseDraw(startPoint, endPoint, ccolor, 1);
			break;
		case RECT:
			pshape = new RectangleDraw(startPoint, endPoint, ccolor, 1);
			break;
		}
	}
}

void Paint::PaintMove(HWND hwnd, int Type, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = GetDC(hwnd);

	if (Type != SELECT) {
		SetClassLong(hwnd, GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));//设置光标
	}
	else SetClassLong(hwnd, GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_HAND));//设置光标

	if (LMouseDown) {//绘画过程
		endPoint.x = LOWORD(lParam), endPoint.y = HIWORD(lParam);
		if (Type != SELECT) {

			pshape->DrawMove(hdc, PS_DOT);
			pshape->SetEndPoint(endPoint.x, endPoint.y);
			pshape->DrawMove(hdc, PS_DOT); //重绘图像

		}
	}
	if (Type == SELECT && Is_Select) { // 选中移动
		if (wParam & MK_LBUTTON) {
			endPoint.x = LOWORD(lParam);
			endPoint.y = HIWORD(lParam);
			if ((*itselect)->SelectShape(hdc, endPoint)) {
				(*itselect)->PointMove(startPoint, endPoint);
				InvalidateRect(hwnd, NULL, TRUE);
				startPoint = endPoint;//更新起点
			}
		}
	}
	ReleaseDC(hwnd, hdc);
}

void Paint::PaintUp(HWND hwnd, int Type)
{
	if (Type == SELECT) {
		SetClassLong(hwnd, GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_HAND));//设置光标
	}
	else {
		HDC hdc = GetDC(hwnd);
		pshape->DrawShape(hdc);
		ReleaseDC(hwnd, hdc);
		shapelist.push_back(pshape);				//将该图形实例存入队列
		LMouseDown = false;
	}
}

void Paint::PaintAll(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);
	for (it = shapelist.begin(); it != shapelist.end(); it++)				//图形重绘
		(*it)->DrawShape(hdc);
	if (Is_Select)
		(*itselect)->MarkSelected(hdc);
	ReleaseDC(hwnd, hdc);
}

