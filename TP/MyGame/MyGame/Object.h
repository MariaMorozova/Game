#pragma once
#include "stdafx.h"

class BaseObject
{
public:
	BaseObject(std::string path, int x, int y)
	{
		_x = x;
		_y = y;
		_xP = x * Pixels;
		_yP = y * Pixels;

		_path = path;
		LoadTexture(path);
	}
	
	~BaseObject() {}
	
	std::string GetNameTexture() const
	{
		return _path;
	}

	std::pair<int, int> GetPos() const
	{
		return std::pair<int, int>(_x, _y);
	}

	virtual void Update(){}
	virtual void Draw(sf::RenderWindow* const window)
	{
		_sprite.setPosition(_xP, _yP);
		window->draw(_sprite);
	}

protected:
	void LoadTexture(std::string path)
	{
		if (_texture.loadFromFile(path))
		{
			_sprite.setTexture(_texture);
		}
	}

	int _x, _y;
	int _xP, _yP;
	sf::Texture _texture;
	sf::Sprite _sprite;
	std::string _path;
};

