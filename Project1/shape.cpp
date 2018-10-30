#include <windows.h>
#include "Shape.h"

Shape::Shape(void){
	StartPoint.x = 0;
	StartPoint.y = 0;
	EndPoint.x = 0;
	EndPoint.y = 0;
	LineColor = RGB(0,0,0);
	LineWidth = 1;
}
Shape::~Shape(void){
}
void Shape::SetEndPoint(int x, int y){
	EndPoint.x = x;
	EndPoint.y = y;
}

void Shape::DrawSquare(HDC hdc, POINT point){
    hcurpen = CreatePen(PS_SOLID, 1, RGB(255,0,0));
	hprepen = (HPEN)SelectObject (hdc, hcurpen);
	Rectangle(hdc, point.x-2-LineWidth, point.y-2-LineWidth, point.x+2+LineWidth, point.y+2+LineWidth);
	SelectObject(hdc, hprepen);
	DeleteObject(hcurpen);
}
void Shape::PointMove(POINT oldPoint, POINT newPoint){
	StartPoint.x +=  newPoint.x-oldPoint.x;
	StartPoint.y +=  newPoint.y-oldPoint.y;
	EndPoint.x +=  newPoint.x-oldPoint.x;
	EndPoint.y +=  newPoint.y-oldPoint.y;
	//DrawMove(hdc);
}

void Shape::WriteToFile(HANDLE hf){
	DWORD num_written;
	WriteFile(hf,&LineWidth,sizeof(int),&num_written,NULL);
	WriteFile(hf,&LineColor,sizeof(COLORREF),&num_written,NULL);
	WriteFile(hf,&FillColor,sizeof(COLORREF),&num_written,NULL);
	WriteFile(hf,&StartPoint,sizeof(POINT),&num_written,NULL);
	WriteFile(hf,&EndPoint,sizeof(POINT),&num_written,NULL);
}
void Shape::ReadFromFile(HANDLE hf){
	DWORD numRead;
	ReadFile(hf,&LineWidth,sizeof(int),&numRead,NULL);
	ReadFile(hf,&LineColor,sizeof(COLORREF),&numRead,NULL);
	ReadFile(hf,&FillColor,sizeof(COLORREF),&numRead,NULL);
	ReadFile(hf,&StartPoint,sizeof(POINT),&numRead,NULL);
	ReadFile(hf,&EndPoint,sizeof(POINT),&numRead,NULL);
}
//虚函数
void Shape::DrawShape(HDC hdc){
}
void Shape::DrawMove(HDC hdc, int ps){
}
bool Shape::SelectShape(HDC hdc, POINT point){
	return false;
}
void Shape::MarkSelected(HDC hdc){
}


//直线
void LineDraw::DrawShape(HDC hdc){
    //MessageBox(NULL,TEXT("调用函数成功"),"", MB_OK);
    hcurpen = CreatePen(PS_SOLID,LineWidth,LineColor);
    hprepen = (HPEN)SelectObject(hdc,hcurpen);
    //MessageBox(NULL, TEXT("设置画笔完成"),TEXT(""), MB_OK);
    MoveToEx(hdc,StartPoint.x,StartPoint.y,NULL);
    //MessageBox(NULL, TEXT("移动坐标完成"),TEXT(""), MB_OK);
    LineTo(hdc,EndPoint.x,EndPoint.y);
    //MessageBox(NULL, TEXT("绘图完成"),TEXT(""), MB_OK);
    SelectObject(hdc, hprepen);
    DeleteObject(hcurpen);
}

void LineDraw::DrawMove(HDC hdc, int ps){
	SetROP2(hdc,R2_NOTXORPEN);
	hcurpen = CreatePen(ps,LineWidth,LineColor);
    hprepen = (HPEN)SelectObject(hdc,hcurpen);
	MoveToEx(hdc,StartPoint.x,StartPoint.y,NULL);
	LineTo(hdc,EndPoint.x,EndPoint.y);
	SelectObject(hdc, hprepen);
	DeleteObject(hprepen);
}
bool LineDraw::SelectShape(HDC hdc, POINT point){			//检测点point是否在已画图形上
	POINT xArea[4] = {{StartPoint.x-5,StartPoint.y}, {StartPoint.x+5,StartPoint.y}, {EndPoint.x+5,EndPoint.y}, {EndPoint.x-5,EndPoint.y}};
	POINT yArea[4] = {{StartPoint.x,StartPoint.y+5}, {StartPoint.x,StartPoint.y-5}, {EndPoint.x,EndPoint.y-5}, {EndPoint.x,EndPoint.y+5}};
    HRGN hrgnx= CreatePolygonRgn(xArea,4,ALTERNATE);
	HRGN hrgny= CreatePolygonRgn(yArea,4,ALTERNATE);
	if(PtInRegion(hrgnx, point.x, point.y) || PtInRegion(hrgny, point.x, point.y))
	{
		DeleteObject(hrgnx);
		DeleteObject(hrgny);
		return true;
	}
	DeleteObject(hrgnx);
	DeleteObject(hrgny);
	return false;
}
void LineDraw::MarkSelected(HDC hdc){								//标记被选中的图形
    DrawSquare(hdc,StartPoint);
	DrawSquare(hdc,EndPoint);
}

void LineDraw::WriteToFile(HANDLE hf){
	DWORD num_written;
	char classname[20] = "Line";
	int classnamelen = strlen(classname);
	WriteFile(hf,&classnamelen,sizeof(int),&num_written,NULL);
	WriteFile(hf,&classname[0],classnamelen,&num_written,NULL);
	Shape::WriteToFile(hf);
}


//矩形
void RectangleDraw::DrawShape(HDC hdc){
    hcurpen = CreatePen(PS_SOLID,LineWidth,LineColor);
    hprepen = (HPEN)SelectObject(hdc,hcurpen);
	SelectObject (hdc, GetStockObject (NULL_BRUSH) );
	Rectangle(hdc,StartPoint.x,StartPoint.y,EndPoint.x,EndPoint.y);
	SelectObject(hdc, hprepen);
	DeleteObject(hcurpen);
}

void RectangleDraw::DrawMove(HDC hdc, int ps){
	SetROP2(hdc,R2_NOTXORPEN);
	hcurpen = CreatePen(ps,LineWidth,LineColor);
    hprepen = (HPEN)SelectObject(hdc,hcurpen);
	//SelectObject(hdc,CreatePen(ps,LineWidth,LineColor));
	Rectangle(hdc,StartPoint.x,StartPoint.y,EndPoint.x,EndPoint.y);
	SelectObject(hdc, hprepen);
	DeleteObject(hcurpen);
}
bool RectangleDraw::SelectShape(HDC hdc, POINT point){			//检测点point是否在已画图形上
	RECT rectbig,rectsmall;
	POINT tempPoint;
	if(StartPoint.x>EndPoint.x)
	{
	    tempPoint.x = StartPoint.x;		tempPoint.y = StartPoint.y;
		StartPoint.x = EndPoint.x;		StartPoint.y = EndPoint.y;
		EndPoint.x = tempPoint.x;		EndPoint.y = tempPoint.y;
	}
	SetRect(&rectbig,StartPoint.x-5,StartPoint.y-5,EndPoint.x+5,EndPoint.y+5);
	SetRect(&rectsmall,StartPoint.x+5,StartPoint.y+5,EndPoint.x-5,EndPoint.y-5);
    if(PtInRect(&rectbig,point) && !PtInRect(&rectsmall,point))
		return true;

	return false;
}
void RectangleDraw::MarkSelected(HDC hdc){								//标记被选中的图形
	POINT point;
	DrawSquare(hdc,StartPoint);
	DrawSquare(hdc,EndPoint);
	point.x = EndPoint.x;   point.y = StartPoint.y;
	DrawSquare(hdc,point);
	point.x = StartPoint.x;  point.y = EndPoint.y;
	DrawSquare(hdc,point);
}

void RectangleDraw::WriteToFile(HANDLE hf){
	DWORD num_written;
	char classname[20] = "Rectangle";
	int classnamelen = strlen(classname);
	WriteFile(hf,&classnamelen,sizeof(int),&num_written,NULL);
	WriteFile(hf,&classname[0],classnamelen,&num_written,NULL);
	Shape::WriteToFile(hf);
}

//画圆
void EllipseDraw::DrawShape(HDC hdc){
    hcurpen = CreatePen(PS_SOLID,LineWidth,LineColor);
    hprepen = (HPEN)SelectObject(hdc,hcurpen);
	SelectObject (hdc, GetStockObject (NULL_BRUSH) );
	Ellipse(hdc,StartPoint.x,StartPoint.y,EndPoint.x,EndPoint.y);
	SelectObject(hdc, hprepen);
	DeleteObject(hcurpen);
}
void EllipseDraw::DrawMove(HDC hdc, int ps){
	SetROP2(hdc,R2_NOTXORPEN);
	hcurpen = CreatePen(ps,LineWidth,LineColor);
    hprepen = (HPEN)SelectObject(hdc,hcurpen);
	//SelectObject(hdc,CreatePen(ps,LineWidth,LineColor));
	Ellipse(hdc,StartPoint.x,StartPoint.y,EndPoint.x,EndPoint.y);
	SelectObject(hdc, hprepen);
	DeleteObject(hcurpen);
}
bool EllipseDraw::SelectShape(HDC hdc, POINT point){			//检测点point是否在已画图形上
	POINT tempPoint;
	if(StartPoint.x>EndPoint.x)
	{
		tempPoint.x = StartPoint.x; tempPoint.y = StartPoint.y;
		StartPoint.x = EndPoint.x; StartPoint.y = EndPoint.y;
		EndPoint.x = tempPoint.x; EndPoint.y = tempPoint.y;
	}
	POINT  middleTop, middleDown, middleLeft, middleRight;
    middleTop.x = (StartPoint.x+EndPoint.x)/2;		 middleTop.y = StartPoint.y;
	middleDown.x = (StartPoint.x+EndPoint.x)/2;		 middleDown.y = EndPoint.y;
	middleLeft.x = StartPoint.x;		 middleLeft.y = (StartPoint.y+EndPoint.y)/2;
	middleRight.x = EndPoint.x;		middleRight.y = (StartPoint.y+EndPoint.y)/2;
	POINT lArray[8] = {{middleLeft.x-10,middleLeft.y},  {StartPoint.x,StartPoint.y},  {middleTop.x,middleTop.y-10},
									{EndPoint.x,StartPoint.y}, {middleRight.x+10,middleRight.y}, {EndPoint.x,EndPoint.y},
									{middleDown.x,middleDown.y+10}, {StartPoint.x,EndPoint.y} };
	POINT sArray[4] = {{middleLeft.x+5,middleLeft.y},{middleTop.x,middleTop.y+5},
										{middleRight.x-5,middleRight.y},{middleDown.x,middleDown.y-5}};
	HRGN hrgnl = CreatePolygonRgn(lArray,8,ALTERNATE);
	HRGN hrgns = CreatePolygonRgn(sArray,4,ALTERNATE);
	if(PtInRegion(hrgnl,point.x,point.y)&&!PtInRegion(hrgns,point.x,point.y))
	{
		DeleteObject(hrgnl);
		DeleteObject(hrgns);
		return true;
	}
	DeleteObject(hrgnl);
	DeleteObject(hrgns);
	return false;
}
void EllipseDraw::MarkSelected(HDC hdc){								//标记被选中的图形
	POINT pointTop, pointDown, pointLeft, pointRight;
	pointTop.x = (StartPoint.x+EndPoint.x)/2;   pointTop.y = StartPoint.y;
	DrawSquare(hdc,pointTop);
	pointDown.x = (StartPoint.x+EndPoint.x)/2;   pointDown.y = EndPoint.y;
	DrawSquare(hdc,pointDown);
	pointLeft.x = StartPoint.x;   pointLeft.y = (StartPoint.y+EndPoint.y)/2;
	DrawSquare(hdc,pointLeft);
	pointRight.x = EndPoint.x;   pointRight.y = (StartPoint.y+EndPoint.y)/2;
	DrawSquare(hdc,pointRight);
}

void EllipseDraw::WriteToFile(HANDLE hf){
	DWORD num_written;
	char classname[20] = "Ellipse";
	int classnamelen = strlen(classname);
	WriteFile(hf,&classnamelen,sizeof(int),&num_written,NULL);
	WriteFile(hf,&classname[0],classnamelen,&num_written,NULL);
	Shape::WriteToFile(hf);
}


