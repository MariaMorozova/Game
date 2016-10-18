
#include "stdafx.h"
#include "Player.h"


Player::Player(int x, int y, bool bot): 
	AnimatedObject(bot? "./Sprites/bot1.png" : "./Sprites/hero.png", x, y, 4, 4, false)
{
	_bot = bot;
	_isMoving = false;
	_playerHP = 3;
	_maxBombs = 1;
}

void Player::Update()
{
	AnimatedObject::Update();

	if (_isMoving)
	{
		_moveTimer--;
		if (_moveTimer)
		{
			_xP += _dx;
			_yP += _dy;
		}
		else
		{
			_isMoving = false;
			SetAnimationActive(false);
		}
	}
}

void Player::Draw(sf::RenderWindow * const window)
{
	AnimatedObject::Draw(window);
}

void Player::Move(int dx, int dy)
{
	if (!_isMoving)
	{
		SetAnimationActive(true);
		_isMoving = true;
		_x += dx;
		_y += dy;
		_dx = dx;
		_dy = dy;

		if (dx > 0)
			SetSpriteRow(2);
		else if (dx < 0)
			SetSpriteRow(1);
		else if (dy > 0)
			SetSpriteRow(0);
		else if (dy < 0)
			SetSpriteRow(3);

		_moveTimer = Pixels;
	}
}

void Player::IncMaxBombs() { _maxBombs++; }

void Player::IncPlayerHP() { _playerHP++; }

void Player::DecPlayerHP() { _playerHP--; }

Player::~Player()
{
}
