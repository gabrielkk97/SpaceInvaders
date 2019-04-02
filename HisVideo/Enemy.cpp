#include "Enemy.h"

Enemy::Enemy()
{

}

Enemy::Enemy(int x_, int y_, int radius_)
{
	x = x_;
	y = y_;
	radius = radius_;
}

void Enemy::initialize(int x_, int y_, int radius_)
{
	x = x_;
	y = y_;
	radius = radius_;
}

void Enemy::Onpaint(PAINTSTRUCT &ps, int resourceId) {
	destroy();

	//TODO use the resourceID to access the corresponding bitmap
	m_bmp = LoadBitmap(GetModuleHandle(nullptr), MAKEINTRESOURCE(spaceInvaders.bmp));
}

void Enemy::destroy() {
	if(m_bmp) {
		DeleteObject(m_bmp);
		m_bmp = nullptr;
		width = 0;
		height = 0;
	}
}