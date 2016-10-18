#include "stdafx.h"
#include "Bomb.h"

Bomb::Bomb(int x, int y, int timer): AnimatedObject("./Sprites/bomb.png", x, y, 4, 1, true)
{
	_x = x;
	_y = y; 
	_timer = timer;
}

void Bomb::Update()
{
	AnimatedObject::Update();
	_timer--;
	if (_timer == 100)
	{
		LoadTexture("./Sprites/boom.png");//boom
		SetSpeed(10);
	}
}

void Bomb::Draw(sf::RenderWindow * const window)
{
	AnimatedObject::Draw(window);
}

Bomb::~Bomb()
{
}
