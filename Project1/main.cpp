#include <windows.h>
#include <commdlg.h>
#include <cstdio>
#include <list>
#include "shape.h"
#include "resource.h"

int numShape;
HANDLE hf;
TCHAR szfilename[100];
static bool LMouseDown;
bool Is_Select = 0;
std::list<Shape*>shapelist; //保存每个图形
std::list<Shape*>::iterator it, itselect;
static TCHAR szClassName[] = TEXT("Simple_Painter");
static int shapetype;
OPENFILENAME ofn;
POINT startPoint, endPoint;
COLORREF ccolor;


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
	LMouseDown = false;
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
    HDC hdc;
	PAINTSTRUCT ps;
	static Shape *pshape;
	ccolor = RGB(0,0,0);


	switch(uMsg){
        case WM_COMMAND:
            switch(LOWORD(wParam)){
            case ID_NEW:
            case ID_DELETE_ALL:
                if(IDYES == MessageBox(hwnd, TEXT("是否新建？"), "", MB_YESNO)){
                    Is_Select = 0;
                    itselect = shapelist.end();
                    InvalidateRect(hwnd, NULL, TRUE);
                    shapelist.clear();
                }
                return 0;
            case ID_OPEN:
                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lpstrTitle = TEXT("打开");
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hwnd;
                ofn.lpstrFile = szfilename;
                ofn.lpstrFile[0] = '\0';
                ofn.nMaxFile = sizeof(szfilename);
                ofn.lpstrFilter = "All\0*.*\0Text\0*.txt\0";
                ofn.nFilterIndex = 1;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = NULL;
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST |OFN_HIDEREADONLY;
                if(GetOpenFileName (&ofn))
                {
                    hf = CreateFile(ofn.lpstrFile, GENERIC_READ,
                        0, (LPSECURITY_ATTRIBUTES) NULL,
                        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
                        (HANDLE) NULL);

                DWORD numRead;
                ReadFile(hf,&numShape,sizeof(int),&numRead,NULL);
                char classname[20];
                int classnamelen;

                shapelist.clear();
                for(int i=0;i<numShape;i++)
                {
                     ReadFile(hf,&classnamelen,sizeof(int),&numRead,NULL);
                     ReadFile(hf,&classname[0],classnamelen,&numRead,NULL);
                     classname[classnamelen] = '\0';
                     if(strcmp(classname,"Rectangle") == 0 )
                         pshape = new RectangleDraw();
                     else if(strcmp(classname,"Line")== 0 )
                         pshape = new LineDraw();
                     else if(strcmp(classname,"Ellipse")== 0)
                         pshape = new EllipseDraw();
                     else
                         MessageBox(NULL,TEXT("错误"),TEXT("打开发生错误"),MB_OK);

                     pshape->ReadFromFile(hf);
                     shapelist.push_back(pshape);
                }

                InvalidateRect(hwnd,NULL,TRUE);
                CloseHandle(hf);
                }
                return 0;
            case ID_SAVE:
                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hwnd;
                ofn.lpstrFile = szfilename;
                ofn.lpstrTitle = TEXT("保存到");
                ofn.lpstrFile[0] = '\0';
                ofn.nMaxFile = sizeof(szfilename);
                ofn.lpstrFilter = "All\0*.*\0TEXT\0*.txt\0";
                ofn.nFilterIndex = 1;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = NULL;
                ofn.lpstrDefExt = "txt";
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;

                if(GetSaveFileName(&ofn))
                {
                    hf = CreateFile(ofn.lpstrFile, GENERIC_WRITE,
                        0, (LPSECURITY_ATTRIBUTES) NULL,
                        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
                        (HANDLE) NULL);

                DWORD numWitten;
                numShape = shapelist.size();
                WriteFile(hf,&numShape,sizeof(int),&numWitten,NULL);//先写入图形对象的个数
                for(it = shapelist.begin(); it != shapelist.end(); it++)
                {
                    (*it) -> WriteToFile(hf);
                }
                CloseHandle(hf);
                }
                return 0;
            case ID_LINE:
                shapetype = LINE;
                //MessageBox(hwnd, TEXT("选择直线"),TEXT(""), MB_OK);
                return 0;
            case ID_CIRCLE:
                shapetype = CIRCLE;
                return 0;
            case ID_RECT:
                shapetype = RECT;
                return 0;
            case ID_SELECT:
                shapetype = SELECT;
                return 0;
            case ID_ABOUT:
                MessageBox(hwnd, TEXT("2017080106021-高弘毅"), TEXT("简易画图程序"), MB_OK);
                return 0;
            default:
                return 0;

            }
            return 0;
		case WM_CREATE:
		    OnCreateMenu(hwnd);
            return 0;
		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
            for(it = shapelist.begin();it!=shapelist.end();it++)				//图形重绘
                (*it)->DrawShape(hdc);
            if(Is_Select)
                (*itselect)->MarkSelected(hdc);

            EndPaint(hwnd, &ps);
            return 0;
		case WM_LBUTTONDOWN:
		    startPoint.x = LOWORD(lParam);
            startPoint.y = HIWORD(lParam);
            endPoint = startPoint;
            if(shapetype == SELECT){
                SetClassLong(hwnd, GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_HAND));//设置光标
                for(it = shapelist.begin(); it != shapelist.end(); ++it){
                    hdc = GetDC(hwnd);
                    if((*it) -> SelectShape(hdc,endPoint)){
                        Is_Select = 1;
                        itselect = it;
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    ReleaseDC(hwnd,hdc);
                }
                return 0;
            }
            if(shapetype == LINE || shapetype == CIRCLE || shapetype == RECT){
                LMouseDown = true;
                SetClassLong(hwnd, GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));//设置光标
                switch(shapetype){
                case LINE:
                    pshape = new LineDraw(startPoint, endPoint, ccolor, 1);
                    //if(pshape == NULL)
                    //MessageBox(hwnd, TEXT("创建直线对象"),TEXT(""), MB_OK);
                    //else MessageBox(hwnd, TEXT("创建直线对象成功"),TEXT(""), MB_OK);*/
                    break;
                case CIRCLE:
                    pshape = new EllipseDraw(startPoint, endPoint, ccolor, 1);
                    //MessageBox(hwnd, TEXT("创建直线对象"),TEXT(""), MB_OK);
                    break;
                case RECT:
                    pshape = new RectangleDraw(startPoint, endPoint, ccolor, 1);
                    //MessageBox(hwnd, TEXT("创建直线对象"),TEXT(""), MB_OK);
                    break;
                }
            }
			break;
		case WM_LBUTTONUP:
		    //MessageBox(hwnd, TEXT("鼠标弹起"),TEXT(""), MB_OK);
			if(shapetype == SELECT){
                SetClassLong(hwnd, GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_HAND));//设置光标
            }
            if(shapetype == LINE || shapetype == RECT || shapetype == CIRCLE){
                hdc = GetDC(hwnd);
                pshape -> DrawShape(hdc);
                ReleaseDC(hwnd,hdc);
                shapelist.push_back(pshape);				//将该图形实例存入队列
                LMouseDown = false;
            }
			break;
		case WM_MOUSEMOVE:
		    hdc = GetDC(hwnd);

			if(shapetype == LINE || shapetype == CIRCLE || shapetype == RECT){
                SetClassLong(hwnd, GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));//设置光标
            }
            else SetClassLong(hwnd, GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_HAND));//设置光标

            if(LMouseDown){//绘画过程
                endPoint.x = LOWORD(lParam), endPoint.y = HIWORD(lParam);
                if(shapetype == LINE || shapetype == CIRCLE || shapetype == RECT){

                    pshape -> DrawMove(hdc, PS_DOT);
                    pshape -> SetEndPoint(endPoint.x, endPoint.y);
                    pshape -> DrawMove(hdc, PS_DOT); //重绘图像

                }
            }
            if(shapetype == SELECT && Is_Select){ // 选中移动
                if(wParam & MK_LBUTTON){
                    endPoint.x = LOWORD(lParam);
                    endPoint.y = HIWORD(lParam);
                    if((*itselect) -> SelectShape(hdc, endPoint)){
                        /*(*itselect) -> DrawMove(hdc, PS_SOLID);
                        (*itselect) -> PointMove(startPoint, endPoint);
                        (*itselect) -> DrawMove(hdc, PS_SOLID);
                        (*itselect) -> MarkSelected(hdc);*/
                        (*itselect) -> PointMove(startPoint, endPoint);
                        InvalidateRect(hwnd, NULL, TRUE);
                        startPoint = endPoint;//更新起点
                    }
                }
            }
            ReleaseDC(hwnd, hdc);
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

