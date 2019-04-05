#pragma once
#include "my_common.h"
#include <vector>

class Player;
class Bullet {
public:
	static const int bulletSize = 10;


	Bullet();
	Bullet(int x_, int y_);

	void initialize(int x_, int y_, float v=10);
	int Onprint(HDC hdc, Player * player);

	void setActive(bool Active);
	void setX(int x_);
	void setY(int y_);


	int getX()const;
	int getY()const;
	bool getActive()const;

private:
	int x;
	int y;
	float velcoity = 10.f;
	bool active = false;
};