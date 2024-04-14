#include <windows.h>
#include "windowManagement.hpp"

void showConsole(bool show){
	HWND consoleWindow = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(consoleWindow, show);
}

void showExe(bool show){
	HWND rainWindow = FindWindowA(NULL, "Animations");
	ShowWindow(rainWindow, show);
}

void showConsole(){
	showConsole(true);
}

void exeToForeground(const char* title){
	showConsole(false);
	HWND rainWindow = FindWindowA(NULL, title);
	SetActiveWindow(rainWindow);
}
