#pragma once
#include <vector>
#include "Player.h"
#include "Bomb.h"
#include "AnimatedObject.h"
#include "stdafx.h"

class Board
{
public:
	Board();

	void Load(std::string path);
	void Update(sf::Event e);
	void Draw(sf::RenderWindow* const window);
	void Boom(std::pair<int, int> pos);
	void DrawBonus(std::pair<int, int> pos);
	~Board();

private:
	void Clean();

	int** _board;
	int _width, _height;
	bool gameover;
	std::string _path;

	Player* _player;
	std::vector<Player*> _bots;
	std::map<std::pair<int, int>, BaseObject*> _walls;
	std::vector<Bomb*> _bombs;
	std::vector<AnimatedObject*> _booms;
	std::vector<BaseObject*> _bonus;
};

