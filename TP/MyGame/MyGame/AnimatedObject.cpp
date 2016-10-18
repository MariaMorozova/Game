#include "stdafx.h"
#include "AnimatedObject.h"


AnimatedObject::AnimatedObject(std::string path, int x, int y, int width, int height, bool active):
	BaseObject(path, x, y)
{
	_width = width;
	_height = height;
	_w = _h = 0;
	_animationActive = active;
	_speed = 10;
	_tick = 0;
}

AnimatedObject::~AnimatedObject()
{
}

void AnimatedObject::Update()
{
	BaseObject::Update();
	if (_animationActive)
	{
		_tick = (_tick + 1) % _speed;

		if (!_tick)
			_w = (_w + 1) % _width;
	}
}

void AnimatedObject::Draw(sf::RenderWindow * const window)
{
	_sprite.setTextureRect(sf::IntRect(_w * Pixels, _h * Pixels, Pixels, Pixels));
	BaseObject::Draw(window);
}
