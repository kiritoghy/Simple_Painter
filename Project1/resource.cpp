#include "resource.h"

void OnCreateMenu(HWND hwnd){
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

