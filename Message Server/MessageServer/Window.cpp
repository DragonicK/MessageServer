#include "stdafx.h"
#include "MessageServer.h"
#include "Server.h"
#include "Window.h"

#include <CommCtrl.h>
#include <Richedit.h>

Window window;

Window::Window() {
	hWnd = NULL;
	hInst = NULL;
	hWndRich = NULL;
}

Window::~Window() {
	hWnd = NULL;
	hInst = NULL;
}

void Window::Load() {
	wcsncpy_s(szWindowTitle, L"Message Server", 15);
	wcsncpy_s(szWindowClass, L"MessageServer", 14);
}

int Window::CreateMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	Load();
  
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(30, 30, 30)); 
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm =LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON));
	
	RegisterClassExW(&wcex);
		
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	hWndRich = CreateRichEdit(hWnd, 25, 25, 506, 334, hInst);

	CHARFORMAT2 cf;

	cf.cbSize = sizeof(CHARFORMAT2);
	cf.dwMask = CFM_COLOR | CFM_BACKCOLOR | CFM_EFFECTS2; // I'm setting only the style information

	cf.crTextColor = RGB(255, 255, 255);
	cf.crBackColor = RGB(30, 30, 30);
	cf.dwEffects = CFE_BOLD;

	SendMessage(hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

	ServerInit();

	MSG msg;
	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT Window::ProcessMain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

	case EM_SETCHARFORMAT:
		if (wParam == SCF_SELECTION) {
			MessageBoxExW(0, L"TENSO", L"CU", 0, 0);
		}
		break; 

	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_CLOSE:
		ServerClose();
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}

	return false;
}

BOOL Window::InitInstance(HINSTANCE hInstance, int nCmdShow) {
	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindowExW(WS_EX_CLIENTEDGE, szWindowClass, szWindowTitle, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT, 0, 581, 429, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

#pragma region RichText

// hwndOwner Dialogbox
// hinst = Application or DLL
HWND Window::CreateRichEdit(HWND hwndOwner,	int x, int y, int width, int height, HINSTANCE hinst) {
	//InitCommonControls();

	LoadLibrary(TEXT("Msftedit.dll"));

	HWND hwndEdit = CreateWindowExW(0, MSFTEDIT_CLASS, L"Type here",
		ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_READONLY | ES_AUTOVSCROLL,
		x, y, width, height,
		hwndOwner, nullptr, hinst, nullptr);

	return hwndEdit;
}

#pragma endregion