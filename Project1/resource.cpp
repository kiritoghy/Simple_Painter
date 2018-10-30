#include "resource.h"

void OnCreateMenu(HWND hwnd){
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

