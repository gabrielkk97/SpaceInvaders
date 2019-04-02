#include "my_common.h"
#include <vector>
#include "Player.h"
#include "BUllet.h"
#include"Enemy.h"

const wchar_t* szWindowClass = L"MyWindow";
const int windowWidth = 800;
const int windowHeight = 800;





Player player;
class GameManger {
public:
	int refreshTime = 17;
	int currentTime=0;
	int amountofEnemy = 20;
	//Player player;

	std::vector<Enemy*> enemies;
	GameManger(int rT) {
		refreshTime = rT;
	}
	void Loop(HWND &hwnd)
	{	
		if (currentTime++ > 17) {
			currentTime = 0;
			
			SetTimer(hwnd, NULL, 17, nullptr);
		}
		else {
			return;
		}
	}

	void Onpaint(PAINTSTRUCT &ps)
	{

	}
};
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//PAINTSTRUCT ps;
	//BeginPaint(hWnd, &ps);
	switch (message)
	{
	case WM_PAINT: {

		InvalidateRect(hWnd, nullptr, true);
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);

		player.Onprint(0,0,ps);
	
		EndPaint(hWnd, &ps);
		
	}break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE: {
		int msg = MessageBox(
			hWnd,
			L"DO you want to close the window ?",
			L"CLose Window",
			MB_YESNO | MB_APPLMODAL | MB_ICONWARNING
		);

		if (msg == IDYES) {
			DestroyWindow(hWnd);
		}
		return 0;
	}
		break;
	case WM_SIZING: {
		auto* rc = reinterpret_cast<RECT*>(lParam);
		rc->right = rc->left + 640;
	}break;

	case WM_RBUTTONDOWN: {
		player.Fire();
	}break;

	case WM_LBUTTONDOWN: {
		

		player.Fire();
		printf("Left Button\n");
	
	}break;

	case WM_KEYDOWN: {
		switch (wParam)
		{
		case VK_UP: {
			PAINTSTRUCT ps;

			BeginPaint(hWnd, &ps);
			player.Onprint(0, -player.moveSpeed, ps);
			EndPaint(hWnd, &ps);
		}break;

		case VK_DOWN:{
			PAINTSTRUCT ps;

			BeginPaint(hWnd, &ps);
			player.Onprint(0, player.moveSpeed, ps);
			EndPaint(hWnd, &ps);
		}break;

		case VK_LEFT: {
			PAINTSTRUCT ps;

			BeginPaint(hWnd, &ps);
			player.Onprint(-player.moveSpeed, 0, ps);
			EndPaint(hWnd, &ps);
		}break;

		case VK_RIGHT: {
			PAINTSTRUCT ps;

			BeginPaint(hWnd, &ps);
			player.Onprint(player.moveSpeed,0, ps);
			EndPaint(hWnd, &ps);
		}break;
		}
	}break;
	case WM_TIMER: {
		InvalidateRect(hWnd, nullptr, false);
		
	}
	} // switch    
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	
	printf("Hello\n");

	//Window Class
	WNDCLASSEX wcex;
	//ZeroMemory(&wcex, sizeof(wcex)); // bzero
	//memset(&wcex, 0 sizeof(wcex));

	my_bzero(wcex);
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	RegisterClassEx(&wcex);

	//------------------------
	HWND hwnd = CreateWindowEx(0,
		szWindowClass,
		L"MyWindow Title",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight,
		nullptr,
		nullptr,
		hInstance,
		0);

	ShowWindow(hwnd, SW_NORMAL);
	UpdateWindow(hwnd);

	player.create(400, 400, 100);
	GameManger gM = GameManger(17);
	// Main message loop:  
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		gM.Loop(hwnd);
	}

	return msg.wParam;
}

int main() {
	return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_NORMAL);
}