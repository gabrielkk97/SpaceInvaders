#include "Bullet.h"
#include "Player.h"

Bullet::Bullet()
{
	x = 0; y = 0;
}
Bullet::Bullet(int x_, int y_ ) {
	x = x_;
	y = y_;
	
}

void Bullet::initialize(int x_,int y_,float v) {
	x = x_;
	y = y_;
	velcoity = v;
}

void Bullet::OverlapTest() {


}

int Bullet::Onprint(PAINTSTRUCT &ps, Player * player)
{
	printf("DLLM\n");
	y -= velcoity;
	if (y <= 0) {
		active = false;
		return 0;
	}
	SelectObject(ps.hdc, GetStockObject(GRAY_BRUSH));
	Rectangle(ps.hdc, x, y, x +bulletSize, y + bulletSize);
	TextOut(ps.hdc, 200,300,L"Fired", 5);
	return 1;
}