#include "Player.h"
#include"Bullet.h"
#include"common.h"

void Player::create(int x_, int y_, int hp_)
{
	x = x_;
	y = y_;
	hp = hp_;

	FiredBullet.resize(MaxBullet);
	for (std::vector<Bullet*>::iterator it = FiredBullet.begin(); it != FiredBullet.end();it++) {
		*it = new Bullet(0,0);
	}
}

void Player::Fire()
{	
	

	if (currentBullet >= MaxBullet) return;
	else {

		//set the position of the bullet to the middle of the player 
		FiredBullet[currentNode]->initialize(x+playerSize/2-Bullet::bulletSize/2, y -10);
		FiredBullet[currentNode]->active = true;

		currentBullet++;

		if (currentNode >= MaxBullet-1)
		{
			currentNode = 0;
		}
		else {
			currentNode++;
		}
	}
	

}
//Mx and My are the change of position of the player , eg if Mx  =1 then the player is moved to the right for one pixel
//TODO draw to backBuffer
void Player::Onprint(float Mx, float My, HDC hdc)
{	
	if (x + Mx < 0 || y + My < 0 || (x+Mx+playerSize>=windowWidth))
	{
		
	}

	else {
		x += Mx;
		y += My;
	}


	SelectObject(hdc, GetStockObject(GRAY_BRUSH));
	Rectangle(hdc, x , y , x + playerSize, y + playerSize);


	for (std::vector<Bullet*>::iterator it = FiredBullet.begin(); it != FiredBullet.end(); it++)
	{	
		if ((*it)->active == false)
		{
			continue;
		}

		int ans = -1;
		ans = (*it)->Onprint(hdc, this);
		if(ans == 0){
			currentBullet--;
		}
		
	}

	ModX = 0;
	ModY = 0;
	

}

int Player::getMX() const{ return ModX; }
int Player::getMy() const{ return ModY; }

void Player::setMod(int x_, int y_) {
	ModX = x_;
	ModY = y_;
}