#pragma once
#include "my_common.h"
#include <vector>
class Bullet;
class Player {
public:

	
	int playerSize = 50;
	float shotdelay = 1.f;

	int currentNode = 0;
	int MaxBullet = 20;
	float moveSpeed = 5.f;

	   std::vector<Bullet*> FiredBullet;

	   void create(int x_, int y_, int hp_);

	   void Fire();
	   //Mx and My are the change of position of the player , eg if Mx  =1 then the player is moved to the right for one pixel
	   void Onprint(float Mx,float My, HDC hdc);


	   int getX()const;
	   int getY()const;
	   int getMX() const;
	   int getMy() const;
	   int getCurrentBullet()const;

	   
	   void setX(int x_);
	   void setY(int y_);
	   void setMod(int x_, int y_);
	   void setCurrentBullet(int number);


	   
	

private:
	int x;
	int y;
	int ModX = 0;
	int ModY = 0;

	int currentBullet = 0;
	int hp = 100;
	HDC m_HDC;
};