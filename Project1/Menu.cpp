#include "Menu.h"
#include <Windows.h>
#include "shape.h"
#include "Paint.h"
#include <list>
#include "resource.h"

extern std::list<Shape*>shapelist; //保存每个图形
extern std::list<Shape*>::iterator it, itselect;
int numShape;
extern bool Is_Select;
extern int shapetype;
extern Paint* paint;
extern Shape* pshape;
OPENFILENAME ofn;
TCHAR szfilename[100];
HANDLE hf;

Menu::Menu()
{
}


Menu::~Menu()
{
}

void Menu:: CreateWinMenu(HWND hwnd) {
	HMENU hMain = CreateMenu();
	HMENU hFile = CreatePopupMenu();
	HMENU hShape = CreatePopupMenu();
	HMENU hSelect = CreatePopupMenu();
	//HMENU hAbout = CreatePopupMenu();
	//主菜单栏
	AppendMenu(hMain, MF_POPUP, (UINT)hFile, TEXT("文件"));
	AppendMenu(hMain, MF_POPUP, (UINT)hShape, TEXT("图形"));
	AppendMenu(hMain, MF_POPUP, (UINT)hSelect, TEXT("区域"));
	AppendMenu(hMain, MF_STRING, ID_ABOUT, TEXT("关于"));
	//文件菜单
	AppendMenu(hFile, MF_STRING, ID_NEW, TEXT("新建"));
	AppendMenu(hFile, MF_STRING, ID_OPEN, TEXT("打开"));
	AppendMenu(hFile, MF_STRING, ID_SAVE, TEXT("保存"));
	//图形菜单
	AppendMenu(hShape, MF_STRING, ID_LINE, TEXT("直线"));
	AppendMenu(hShape, MF_STRING, ID_CIRCLE, TEXT("圆"));
	AppendMenu(hShape, MF_STRING, ID_RECT, TEXT("矩形"));
	//选取菜单
	AppendMenu(hSelect, MF_STRING, ID_SELECT, TEXT("选取"));
	AppendMenu(hSelect, MF_STRING, ID_DELETE_ALL, TEXT("清空"));

	SetMenu(hwnd, hMain);
}

void Menu::Command(HWND hwnd, UINT Msg) {
	switch (Msg) {
	case ID_NEW:
	case ID_DELETE_ALL:
		Is_Select = 0;
		itselect = shapelist.end();
		InvalidateRect(hwnd, NULL, TRUE);
		shapelist.clear();
		return;
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
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		if (GetOpenFileName(&ofn))
		{
			hf = CreateFile(ofn.lpstrFile, GENERIC_READ,
				0, (LPSECURITY_ATTRIBUTES)NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
				(HANDLE)NULL);

			DWORD numRead;
			ReadFile(hf, &numShape, sizeof(int), &numRead, NULL);
			char classname[20];
			int classnamelen;

			shapelist.clear();
			for (int i = 0; i < numShape; i++)
			{
				ReadFile(hf, &classnamelen, sizeof(int), &numRead, NULL);
				ReadFile(hf, &classname[0], classnamelen, &numRead, NULL);
				classname[classnamelen] = '\0';
				if (strcmp(classname, "Rectangle") == 0)
					pshape = new RectangleDraw();
				else if (strcmp(classname, "Line") == 0)
					pshape = new LineDraw();
				else if (strcmp(classname, "Ellipse") == 0)
					pshape = new EllipseDraw();
				else
					MessageBox(NULL, TEXT("错误"), TEXT("打开发生错误"), MB_OK);

				pshape->ReadFromFile(hf);
				shapelist.push_back(pshape);
			}

			InvalidateRect(hwnd, NULL, TRUE);
			CloseHandle(hf);
		}
		return;
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
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

		if (GetSaveFileName(&ofn))
		{
			hf = CreateFile(ofn.lpstrFile, GENERIC_WRITE,
				0, (LPSECURITY_ATTRIBUTES)NULL,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
				(HANDLE)NULL);

			DWORD numWitten;
			numShape = shapelist.size();
			WriteFile(hf, &numShape, sizeof(int), &numWitten, NULL);//先写入图形对象的个数
			for (it = shapelist.begin(); it != shapelist.end(); it++)
			{
				(*it)->WriteToFile(hf);
			}
			CloseHandle(hf);
		}
		return;
	case ID_LINE:
		shapetype = LINE;
		//MessageBox(hwnd, TEXT("选择直线"),TEXT(""), MB_OK);
		return;
	case ID_CIRCLE:
		shapetype = CIRCLE;
		return;
	case ID_RECT:
		shapetype = RECT;
		return;
	case ID_SELECT:
		shapetype = SELECT;
		return;
	case ID_ABOUT:
		MessageBox(hwnd, TEXT("2017080106021-高弘毅"), TEXT("简易画图程序"), MB_OK);
		return;
	default:
		return;

	}
}
