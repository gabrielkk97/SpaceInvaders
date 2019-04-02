#pragma once
#include "Bullet.h"
class Enemy {
public:
	int x;
	int y;
	int radius;

	Enemy();
	Enemy(int x_, int y_, int radius_);
	void initialize(int x_, int y_, int radius_);
	void Enemy::Onpaint(PAINTSTRUCT &ps, int resourceId);

	void destroy();

private:
	HBITMAP m_bmp = nullptr;
	int width = 0;
	int height = 0;
};