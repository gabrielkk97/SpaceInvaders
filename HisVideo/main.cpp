#include "my_common.h"
#include <vector>
#include "Player.h"
#include "BUllet.h"
#include "resource1.h"
#include <assert.h>
#include "common.h"

const wchar_t* szWindowClass = L"MyWindow";


//top left bottom left etc
bool OverlapTest(int left_REC1,int left_REC2,int right_REC1,int right_REC2,int top_REC1,int top_REC2,int bot_REC1,int bot_REC2)
{
	return  
		left_REC1 <= right_REC2 &&
		right_REC1 >= left_REC2 &&
		top_REC1 <= bot_REC2 &&
		bot_REC1 >= top_REC2;
}

Player player;
struct Color {
	uint8_t b = 0;
	uint8_t g = 0;
	uint8_t r = 0;
	uint8_t a = 0;

	void set(uint8_t r_, uint8_t g_, uint8_t b_) {
		r = r_;
		g = g_;
		b = b_;
		a = 0;
	}
};



class Enemy {
public :
	int x = 0;
	int y = 0;

	Enemy() {

	}

	bool active = false;

	void init(int x_, int y_, bool active_) {
		x = x_;
		y = y_;
		active = active_;
	}
};

class BitMap {
public:
	BitMap()
	{

	}
	~BitMap()
	{
		destroy();
	}
	BitMap(int resourceID)
	{

	}
	void  create(int width, int height) {
		//CreateBitmap(width, height, 1, 32);
	}

	void draw(HDC hdc, int x,int y ) {

		if (!m_bmp || !hdc)
		{
			
			return;
		}
		auto src = CreateCompatibleDC(hdc);
		SelectObject(src, m_bmp);

		BitBlt(hdc, x, y,width,height,src,0,0,SRCCOPY);
		//StretchBlt(hdc, x, y, width, height, src, 0, 0, 50, 50, SRCCOPY);
		DeleteDC(src);
		
	}

	void LoadBitMap(int resourceID) {
		destroy();

		//TODO use the resourceID to access the corresponding bitmap
		m_bmp = LoadBitmap(GetModuleHandle(nullptr), MAKEINTRESOURCE(resourceID));
		if (!m_bmp) {
			return;
		}

		BITMAP info;
		GetObject(m_bmp, sizeof(info), &info);
		width = info.bmWidth;
		height = info.bmHeight;


	}

	void destroy() {
		if (m_bmp) {
			DeleteObject(m_bmp);
			m_bmp = nullptr;
			width = 0;
			height = 0;
		}
	}


private:
	HBITMAP m_bmp = nullptr;
	int width = 0;
	int height = 0;
	int resourceID;
};

BitMap bitMap;


class EnemyController {
public:
	
	
	void create() {
		Enemies.resize(numbersofRow*numbersofColume);

		for (int i = 0; i < numbersofRow; i++)
		{
			
			for (int j = 0; j < numbersofColume; j++)
			{
				Enemies[i*numbersofColume + j] = new Enemy();
				Enemies[i*numbersofColume + j]->init(startX + j * vDistance, startY + i * hDistance, true);
				
				
			}
		}
	}

	void Onpaint(HDC hdc) {
		
		bitMap.LoadBitMap(IDB_BITMAP1);
		for (int i =0;i<Enemies.size();i++)
		{
			if (Enemies[i]->active == false)
			{
				continue;
			}
			
			for (int j = 0; j < player.FiredBullet.size(); j++) {
				if (!player.FiredBullet[j]->active)
				{
					continue;
				}
				bool ifHit = OverlapTest(Enemies[i]->x, player.FiredBullet[j]->x, Enemies[i]->x + 50, player.FiredBullet[j]->x + player.FiredBullet[j]->bulletSize, Enemies[i]->y, \
					player.FiredBullet[j]->y, Enemies[i]->y + 36, player.FiredBullet[j]->y + player.FiredBullet[j]->bulletSize);

				if (ifHit)
				{
					
					player.FiredBullet[j]->active = false;
					Enemies[i]->active = false;
					player.currentBullet--;
				}
			}

			bitMap.draw(hdc, Enemies[i]->x, Enemies[i]->y);
			

		}
	}


private:
	std::vector<Enemy*>Enemies;
	int vDistance = 80;
	int hDistance = 80;

	int startX = 50;
	int startY = 50;

	int numbersofRow = 5;
	int numbersofColume = 9;
};

class GameManger {
public:
	
	//Player player;

	
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
private:
	int refreshTime = 17;
	int currentTime = 0;
	int amountofEnemy = 20;
};

class BackBuffer {
public:
	BackBuffer(){}
	~BackBuffer() {
		destroy();
	}

	void create(HDC hdc,int width ,int height)
	{
		m_HDC = CreateCompatibleDC(hdc);
		m_Bitmap = CreateCompatibleBitmap(m_HDC, width, height);

		SelectObject(m_HDC, m_Bitmap);
		RECT rc;
		rc.left = 0;
		rc.right = width;
		rc.top = 0;
		rc.bottom = height;

		auto brush = (HBRUSH)GetStockObject(WHITE_BRUSH);

		FillRect(m_HDC, &rc, brush);

		m_width = width;
		m_height = height;
	}

	void draw(HDC hdc, int x, int y) {
		if (!hdc || !m_Bitmap || !m_Bitmap)
		{
			return;
		}
		

		BitBlt(hdc, x, y, m_width, m_height, m_HDC, 0, 0, SRCCOPY);

		HBITMAP newBitMap = CreateCompatibleBitmap(m_HDC, m_width, m_height);
		SelectObject(m_HDC, newBitMap);
		DeleteObject(m_Bitmap);
		RECT rc;
		rc.left = 0;
		rc.right = m_width;
		rc.top = 0;
		rc.bottom = m_height;

		auto brush = (HBRUSH)GetStockObject(WHITE_BRUSH);

		FillRect(m_HDC, &rc, brush);
		m_Bitmap = newBitMap;
	}
	void destroy() {
		if (m_Bitmap)
		{
			DeleteObject(m_Bitmap);
			m_Bitmap = nullptr;
		}

		if (m_HDC) {

			DeleteDC(m_HDC);
			m_HDC = nullptr;
		}
		m_width = 0;
		m_height = 0;
	}

	HDC getHDC() {
		return m_HDC;
	}


private:
	HDC m_HDC;
	HBITMAP m_Bitmap;
	int m_width = 0;
	int m_height=0;
};

EnemyController eC;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//PAINTSTRUCT ps;
	//BeginPaint(hWnd, &ps);
	switch (message)
	{

	case WM_CREATE: {
		
		
		bitMap.LoadBitMap(IDB_BITMAP1);
		eC.create();
		player.create(300, 690, 100);
		

		//The main game loop and trying to go for 60fps
		SetTimer(hWnd, NULL, 17, nullptr);

		
	}
	case WM_PAINT: {

		InvalidateRect(hWnd, nullptr, true);
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);

		eC.Onpaint(ps.hdc);     // put these two lines to WM_TImer and paint them to the backBuffer
		player.Onprint(player.getMX(),player.getMy(),ps.hdc);

		//backbuffer.draw(ps.hdc, 0,0);
	
		EndPaint(hWnd, &ps);
		
	}break;

	case WM_DESTROY:
		bitMap.destroy();
		

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

	

	case WM_LBUTTONDOWN: {
		

		player.Fire();
		
	
	}break;

	case WM_KEYDOWN: {
		switch (wParam)
		{

		//pressing W key 
		case 0x57: {
			/*PAINTSTRUCT ps;

			BeginPaint(hWnd, &ps);
			player.Onprint(0, -player.moveSpeed, backbuffer.getHDC());
			EndPaint(hWnd, &ps);*/

			player.setMod(0, -player.moveSpeed);
		}break;

		//pressing s key
		case 0x53:{
			/*PAINTSTRUCT ps;

			BeginPaint(hWnd, &ps);
			player.Onprint(0, player.moveSpeed, backbuffer.getHDC());
			EndPaint(hWnd, &ps);*/

			player.setMod(0, player.moveSpeed);
		}break;

		//pressing A key
		case 0x41: {
			/*PAINTSTRUCT ps;

			BeginPaint(hWnd, &ps);
			player.Onprint(-player.moveSpeed, 0, backbuffer.getHDC());
			EndPaint(hWnd, &ps);*/
			
			player.setMod(-player.moveSpeed, 0);
		}break;

		//pressing D key
		case 0x44: {
			/*PAINTSTRUCT ps;

			BeginPaint(hWnd, &ps);
			player.Onprint(player.moveSpeed,0, backbuffer.getHDC());
			EndPaint(hWnd, &ps);*/

			player.setMod(player.moveSpeed, 0);
		}break;
		}
	}break;
	case WM_TIMER: {
		//paint things to the backbuffer
	

		//eC.Onpaint(backbuffer.getHDC());     // put these two lines to WM_TImer and paint them to the backBuffer
		//player.Onprint(0,0,backbuffer.getHDC());
		
		//printf("player 's position is %d, and %d \n", player.x, player.y);
		InvalidateRect(hWnd, nullptr, true);
		
	}
	} // switch    
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	
	


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
		WS_OVERLAPPEDWINDOW&(~WS_MAXIMIZEBOX),
		CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight,
		nullptr,
		nullptr,
		hInstance,
		0);

	ShowWindow(hwnd, SW_NORMAL);
	UpdateWindow(hwnd);

	//create the player object and set the inital location 
	
	GameManger gM = GameManger(17);
	// Main message loop:  
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		//gM.Loop(hwnd);
	}

	return msg.wParam;
}

int main() {
	return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_NORMAL);
}