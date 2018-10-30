#pragma once
#include <windows.h>
#include <list>

class Shape
{
    protected:
        POINT StartPoint;
        POINT EndPoint;

        int LineWidth;			//线宽
        COLORREF LineColor;		//线的颜色
        COLORREF FillColor;     //线的填充
        //预防增加改色，改线宽，改填充的需求
        HPEN hcurpen, hprepen; //用于创建和删除画笔,防止GDI对象创建过多卡死
    public:
        Shape(void);
        ~Shape(void);
        void SetEndPoint(int x, int y);				    //设定终点坐标
        virtual void DrawShape(HDC hdc);						//画出完整图形
        virtual void DrawMove(HDC hdc, int ps);					//鼠标移动过程中显示图形
        virtual void PointMove(POINT oldPoint, POINT newPoint);				//拖动图形
//选中与删除
        virtual bool SelectShape(HDC hdc, POINT point);			//检测点point是否在已画图形上
        void DrawSquare(HDC hdc,POINT point);										//标记被选中的图形
        virtual void MarkSelected(HDC hdc);
//保存和打开
        virtual void WriteToFile(HANDLE hf);
        virtual void ReadFromFile(HANDLE hf);
};


//画直线
class LineDraw: public Shape{
    public:
        LineDraw(void)	{ }
        LineDraw(POINT start,POINT end,COLORREF LineColor,int LineWidth = 2){
            StartPoint= start;
            EndPoint = end;
            LineColor = LineColor;
            LineWidth = LineWidth;
         }
        ~LineDraw(void);
    //画图函数
        virtual void DrawShape(HDC hdc);
        virtual void DrawMove(HDC hdc, int ps);
    //选中与删除
        virtual bool SelectShape(HDC hdc, POINT point);			//检测点point是否在已画图形上
        virtual void MarkSelected(HDC hdc);									//标记被选中的图形
//保存和打开
        virtual void WriteToFile(HANDLE hf);
};

//画矩形
class RectangleDraw: public Shape
{
    public:
        RectangleDraw(void)	{}
        RectangleDraw(POINT start,POINT end,COLORREF LineColor,int LineWidth = 2){
            StartPoint= start;
            EndPoint = end;
            LineColor = LineColor;
            LineWidth = LineWidth;
         }
        ~RectangleDraw(void);
        virtual void DrawShape(HDC hdc);
        virtual void DrawMove(HDC hdc, int ps);
        virtual bool SelectShape(HDC hdc, POINT point);			//检测点point是否在已画图形上
        virtual void MarkSelected(HDC hdc);									//标记被选中的图形
//保存和打开
        virtual void WriteToFile(HANDLE hf);
};

//画圆
class EllipseDraw: public Shape{
    public:
        EllipseDraw(void){}
        EllipseDraw(POINT start,POINT end,COLORREF LineColor,int LineWidth = 2){
            StartPoint= start;
            EndPoint = end;
            LineColor = LineColor;
            LineWidth = LineWidth;
         }
        ~EllipseDraw(void);
        virtual void DrawShape(HDC hdc);
        virtual void DrawMove(HDC hdc, int ps);
        virtual bool SelectShape(HDC hdc, POINT point);			//检测点point是否在已画图形上
        virtual void MarkSelected(HDC hdc);									//标记被选中的图形
//保存和打开
        virtual void WriteToFile(HANDLE hf);
};

