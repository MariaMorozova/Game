#pragma once
#include "AnimatedObject.h"
#include "stdafx.h"

class Bomb: public AnimatedObject
{
public:
	Bomb(int x, int y, int timer);

	void Update();
	void Draw(sf::RenderWindow* const window);
	int GetTimer() const { return _timer; }

	~Bomb();
private:
	int _timer, _x, _y;
	bool _isMoving;
};

