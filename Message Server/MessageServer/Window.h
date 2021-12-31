#pragma once

#define WIN32_LEAN_AND_MEAN 
#define MAXLOADSTRING			100

#define IDI_ICON				101

class Window {
public:
	HINSTANCE hInst;
	HWND hWnd;
	HWND hWndRich;

	Window();
	~Window();

	int CreateMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow);
	LRESULT ProcessMain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	WCHAR szWindowTitle[MAXLOADSTRING];
	WCHAR szWindowClass[MAXLOADSTRING];
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	HWND CreateRichEdit(HWND hwndOwner, int x, int y, int width, int height, HINSTANCE hinst);
	void Load();
};

extern Window window;