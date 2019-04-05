#pragma once
#include "my_common.h"
#include <vector>

class Player;
class Bullet {
public:
	int x;
	int y;
	float velcoity = 10.f;

	static const int bulletSize = 10;

	bool active = false;

	Bullet();
	Bullet(int x_, int y_);

	void initialize(int x_, int y_, float v=10);

	void OverlapTest();

	int Onprint(HDC hdc, Player * player);
};