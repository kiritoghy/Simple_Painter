#pragma once
#include <windows.h>
#include <list>

class Shape
{
    protected:
        POINT StartPoint;
        POINT EndPoint;

        int LineWidth;			//�߿�
        COLORREF LineColor;		//�ߵ���ɫ
        COLORREF FillColor;     //�ߵ����
        //Ԥ�����Ӹ�ɫ�����߿�����������
        HPEN hcurpen, hprepen; //���ڴ�����ɾ������,��ֹGDI���󴴽����࿨��
    public:
        Shape(void);
        ~Shape(void);
        void SetEndPoint(int x, int y);				    //�趨�յ�����
        virtual void DrawShape(HDC hdc);						//��������ͼ��
        virtual void DrawMove(HDC hdc, int ps);					//����ƶ���������ʾͼ��
        virtual void PointMove(POINT oldPoint, POINT newPoint);				//�϶�ͼ��
//ѡ����ɾ��
        virtual bool SelectShape(HDC hdc, POINT point);			//����point�Ƿ����ѻ�ͼ����
        void DrawSquare(HDC hdc,POINT point);										//��Ǳ�ѡ�е�ͼ��
        virtual void MarkSelected(HDC hdc);
//����ʹ�
        virtual void WriteToFile(HANDLE hf);
        virtual void ReadFromFile(HANDLE hf);
};


//��ֱ��
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
    //��ͼ����
        virtual void DrawShape(HDC hdc);
        virtual void DrawMove(HDC hdc, int ps);
    //ѡ����ɾ��
        virtual bool SelectShape(HDC hdc, POINT point);			//����point�Ƿ����ѻ�ͼ����
        virtual void MarkSelected(HDC hdc);									//��Ǳ�ѡ�е�ͼ��
//����ʹ�
        virtual void WriteToFile(HANDLE hf);
};

//������
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
        virtual bool SelectShape(HDC hdc, POINT point);			//����point�Ƿ����ѻ�ͼ����
        virtual void MarkSelected(HDC hdc);									//��Ǳ�ѡ�е�ͼ��
//����ʹ�
        virtual void WriteToFile(HANDLE hf);
};

//��Բ
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
        virtual bool SelectShape(HDC hdc, POINT point);			//����point�Ƿ����ѻ�ͼ����
        virtual void MarkSelected(HDC hdc);									//��Ǳ�ѡ�е�ͼ��
//����ʹ�
        virtual void WriteToFile(HANDLE hf);
};

