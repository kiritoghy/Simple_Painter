//杂七杂八的一些定义声明
#ifndef _RESOURCE_H
#define _RESOURCE_H
#include <windows.h>

#define ID_NEW           1001
#define ID_OPEN          1002
#define ID_SAVE          1003
#define ID_LINE          1004
#define ID_CIRCLE        1005
#define ID_RECT          1006
#define ID_SELECT        1007
#define ID_DELETE_ALL    1008
#define ID_ABOUT         1009
#define LINE             1
#define CIRCLE           2
#define RECT             3
#define SELECT           4

void OnCreateMenu(HWND);
int OnCommand(HWND, int);
#endif


