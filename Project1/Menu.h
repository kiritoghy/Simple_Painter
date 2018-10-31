#pragma once
#include <Windows.h>


class Menu
{
public:
	Menu();
	~Menu();
	void Command(HWND hwnd, UINT Msg);
	void CreateWinMenu(HWND hwnd);
};


