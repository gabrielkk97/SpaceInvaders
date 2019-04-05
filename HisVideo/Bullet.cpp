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



//TODO draw to backBuffer
int Bullet::Onprint(HDC hdc, Player * player)
{
	
	y -= velcoity;
	if (y <= 0) {
		active = false;
		return 0;
	}
	SelectObject(hdc, GetStockObject(GRAY_BRUSH));
	Rectangle(hdc, x, y, x +bulletSize, y + bulletSize);
	
	return 1;
}

void Bullet::setActive(bool Active)
{
	active = Active;
}

void Bullet::setX(int x_)
{
	x = x_;
}

void Bullet::setY(int y_)
{
	y = y_;
}

int Bullet::getX()const
{
	return x;
}

int Bullet::getY()const
{
	return y;
}

bool Bullet::getActive()const
{
	return active;
}
