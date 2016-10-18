#pragma once
#include "AnimatedObject.h"

class Player: public AnimatedObject
{
public:
	Player(int x, int y, bool bot = true);

	void Update();
	void Draw(sf::RenderWindow* const window);
	void Move(int dx, int dy);

	int GetMaxBombs() const { return _maxBombs; }
	void IncMaxBombs();

	void Hit() { _playerHP--; }
	int GetPlayerHP() const { return _playerHP; }
	void IncPlayerHP();
	void DecPlayerHP();

	~Player();

private:
	bool _bot;
	bool _isMoving;
	int _moveTimer;
	int _dx, _dy;

	int _playerHP;
	int _maxBombs; 
};

