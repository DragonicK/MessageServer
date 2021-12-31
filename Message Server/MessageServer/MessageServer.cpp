#include <Windows.h>
#include <sdkddkver.h>

#include "MessageServer.h"
#include "Window.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return window.ProcessMain(hWnd, uMsg, wParam, lParam);
}

int APIENTRY WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {

	return window.CreateMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
};