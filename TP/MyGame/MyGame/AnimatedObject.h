#pragma once
#include "Object.h"

class AnimatedObject: public BaseObject
{
public:
	AnimatedObject(std::string path, int x, int y, int width, int height, bool active);
	~AnimatedObject();

	void Update();
	void Draw(sf::RenderWindow* const window);

	void SetAnimationActive(bool active) { _animationActive = active; }
	void SetSpriteRow(int h)
	{
		_h = h % _height;
	}

	void SetSpeed(int speed) { _speed = speed; }
	
private:
	int _width, _height;
	int _w, _h;
	bool _animationActive;

	int _speed, _tick;
};

