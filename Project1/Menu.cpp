#include "Menu.h"
#include <Windows.h>
#include "shape.h"
#include "Paint.h"
#include <list>
#include "resource.h"

extern std::list<Shape*>shapelist; //����ÿ��ͼ��
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
	//���˵���
	AppendMenu(hMain, MF_POPUP, (UINT)hFile, TEXT("�ļ�"));
	AppendMenu(hMain, MF_POPUP, (UINT)hShape, TEXT("ͼ��"));
	AppendMenu(hMain, MF_POPUP, (UINT)hSelect, TEXT("����"));
	AppendMenu(hMain, MF_STRING, ID_ABOUT, TEXT("����"));
	//�ļ��˵�
	AppendMenu(hFile, MF_STRING, ID_NEW, TEXT("�½�"));
	AppendMenu(hFile, MF_STRING, ID_OPEN, TEXT("��"));
	AppendMenu(hFile, MF_STRING, ID_SAVE, TEXT("����"));
	//ͼ�β˵�
	AppendMenu(hShape, MF_STRING, ID_LINE, TEXT("ֱ��"));
	AppendMenu(hShape, MF_STRING, ID_CIRCLE, TEXT("Բ"));
	AppendMenu(hShape, MF_STRING, ID_RECT, TEXT("����"));
	//ѡȡ�˵�
	AppendMenu(hSelect, MF_STRING, ID_SELECT, TEXT("ѡȡ"));
	AppendMenu(hSelect, MF_STRING, ID_DELETE_ALL, TEXT("���"));

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
		ofn.lpstrTitle = TEXT("��");
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
					MessageBox(NULL, TEXT("����"), TEXT("�򿪷�������"), MB_OK);

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
		ofn.lpstrTitle = TEXT("���浽");
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
			WriteFile(hf, &numShape, sizeof(int), &numWitten, NULL);//��д��ͼ�ζ���ĸ���
			for (it = shapelist.begin(); it != shapelist.end(); it++)
			{
				(*it)->WriteToFile(hf);
			}
			CloseHandle(hf);
		}
		return;
	case ID_LINE:
		shapetype = LINE;
		//MessageBox(hwnd, TEXT("ѡ��ֱ��"),TEXT(""), MB_OK);
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
		MessageBox(hwnd, TEXT("2017080106021-�ߺ���"), TEXT("���׻�ͼ����"), MB_OK);
		return;
	default:
		return;

	}
}
