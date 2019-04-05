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


	Enemy() {
		x = 0;
		y = 0;
	}

	void init(int x_, int y_, bool active_) {
		x = x_;
		y = y_;
		active = active_;
	}

	void setX(int x_) {	x = x_;	}
	void setY(int y_) {y = y_;}
	void setActive(bool Active) { active = Active; }
	

	int getX()const {return x;}
	int getY()const { return y; }
	bool getActive()const { return active; }

private:
	int x;
	int y;
	int m_width;
	int m_height;

	bool active = false;
	
};

class BitMap {
public:
	BitMap(){}

	~BitMap(){
		destroy(); 
		if (m_HDC) {
			DeleteDC(m_HDC);
		}
	}

	BitMap(int resourceID){}

	void CreateHDC(HDC hdc) {
		m_HDC = CreateCompatibleDC(hdc);
		SelectObject(m_HDC, m_bmp);
	}

	void draw(HDC hdc, int x,int y ) {

		if (!m_bmp )
		{
			return;
		}
		if (!hdc) {
			m_HDC = CreateCompatibleDC(hdc);
			SelectObject(m_HDC, m_bmp);
		}
	
		BitBlt(hdc, x, y,width,height,m_HDC,0,0,SRCCOPY);
		
	}

	void LoadBitMap(int resourceID) {
		destroy();

		
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

	int getWidth() const{	return width;	}
	int getHeight() const{	return height;	}


private:
	HBITMAP m_bmp = nullptr;
	HDC m_HDC;
	int width = 0;
	int height = 0;
	int resourceID;
};



class EnemyController {
public:
	
	~EnemyController(){
		for (int i = 0; i < Enemies.size();i++) {
			if (Enemies[i]) {
				delete Enemies[i];
			}
		}
		Enemies.clear();
	}
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

	void LoadBitMap(int resourceID)
	{
		m_bitmap.LoadBitMap(resourceID);
		Bit_width = m_bitmap.getWidth();
		Bit_height  = m_bitmap.getHeight();

		printf("the width is %d and height is %d \n", Bit_width, Bit_height);
	}

	void CreateHDC(HDC hdc)
	{
		m_bitmap.CreateHDC(hdc);
	}
	void Onpaint(HDC hdc) {
		
		
		int left_0;
		int left_1;
		int right_0;
		int right_1;
		int top_0;
		int top_1;
		int bot_0;
		int bot_1;

		for (int i =0;i<Enemies.size();i++)
		{
			if (Enemies[i]->getActive() == false)
			{
				continue;
			}
			
			for (int j = 0; j < player.FiredBullet.size(); j++) {
				if (!(player.FiredBullet[j]->getActive()))
				{
					continue;
				}

				//TODO refactor
				left_0 = Enemies[i]->getX();
				left_1 = player.FiredBullet[j]->getX();

				right_0 = left_0 + Bit_width;
				right_1 = left_1 + player.FiredBullet[j]->bulletSize;

				top_0 = Enemies[i]->getY();
				top_1 = player.FiredBullet[j]->getY();

				bot_0 = top_0 + Bit_height;
				bot_1 = top_1 + player.FiredBullet[j]->bulletSize;

				bool ifHit = OverlapTest(left_0, left_1, right_0, right_1, top_0 , top_1, bot_0, bot_1);

				if (ifHit)
				{
					
					player.FiredBullet[j]->setActive(false);

					Enemies[i]->setActive(false);
					player.setCurrentBullet(-1);
				}
			}

			m_bitmap.draw(hdc, Enemies[i]->getX(), Enemies[i]->getY());
			

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

	int Bit_width = 0;
	int Bit_height = 0;
	BitMap m_bitmap;
};

//TODO put other manager and logic in the loop function 
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
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		
		eC.LoadBitMap(IDB_BITMAP1);
		eC.create();
		eC.CreateHDC(ps.hdc);
		player.create(300, 690, 100);
		

		//The main game loop and trying to go for 60fps
		SetTimer(hWnd, NULL, TIMER_INTERVAL_MS, nullptr);

		EndPaint(hWnd, &ps);
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
		WS_OVERLAPPEDWINDOW&(~WS_THICKFRAME),
		CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight,
		nullptr,
		nullptr,
		hInstance,
		0);

	ShowWindow(hwnd, SW_NORMAL);
	UpdateWindow(hwnd);

	
	
	//GameManger gM = GameManger(17);
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