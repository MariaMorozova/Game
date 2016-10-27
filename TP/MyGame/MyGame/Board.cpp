#include "stdafx.h"
#include "Board.h"
#include <fstream>
#include <iostream>

#define BoardSpace 0
#define NonDestroyableWall 1
#define DestroyableWall 2


Board::Board()
{
	_reLoad = 2;
}


void Board::Load(std::string path)
{
	_path = path;
	Clean();

	gameover = false;
	std::ifstream fileIn(path);
	if (fileIn)
	{
		fileIn >> _height >> _width;
		_board = new int*[_height];

		for (int y = 0; y < _height; y++)
			_board[y] = new int[_width];

		for (int y = 0; y < _height; y++)
		{
			char* str = new char[_width + 1];
			fileIn.read(str, _width + 1);

			
			for (int x = 0, k = 0; x < _width; x++, k++)
			{
				switch (str[k])
				{
				case 'b':
					_bots.push_back(new Player(x, y));
				case ' ':
					_board[y][x] = BoardSpace;
					break;
				case '#':
					_board[y][x] = NonDestroyableWall;		
					_walls[std::pair<int, int>(x, y)] = new BaseObject("./Sprites/wall1.png", x, y);
					break;
				case 'p':
					_board[y][x] = BoardSpace;
					_player = new Player(x, y, false);
					break;
				case '1':		
					_board[y][x] = DestroyableWall;
					_walls[std::pair<int, int>(x, y)] = new BaseObject("./Sprites/wall2.png", x, y);
					break;
				default:
					x--; // \n
					break;
				}
			}
		}
	}
}

void Board::UpdateBonus(std::pair<int, int> pos)
{
	for (auto i = _bonus.begin(); i != _bonus.end(); )
	{
		if ((*i)->GetPos() == pos)
		{
			auto texture = (*i)->GetNameTexture();
			if (texture.find("bonusbomb"))
			{
				_player->IncMaxBombs();
			}
			if (texture.find("bonuslife"))
			{
				_reLoad++;
			}
			i = _bonus.erase(i);
		}
		else
			++i;
	}
}

void Board::MovingBots(std::pair<int, int> pos)
{
	for (auto bot : _bots)
	{
		bot->Update();
		bool hor = rand() % 2,
			forward = rand() % 2;
		std::pair<int, int> botPos = bot->GetPos();

		if (botPos == pos)
		{
			Hit();
			return;
		}

		if (hor && forward && _board[botPos.second][botPos.first + 1] == BoardSpace) //right		
			bot->Move(1, 0);
		else if (hor && !forward && _board[botPos.second][botPos.first - 1] == BoardSpace) //left
			bot->Move(-1, 0);
		else if (!hor && forward && _board[botPos.second - 1][botPos.first] == BoardSpace) //up
			bot->Move(0, -1);
		else if (!hor && !forward && _board[botPos.second + 1][botPos.first] == BoardSpace) //down
			bot->Move(0, 1);
	}
}

void Board::UpdateBombs()
{
	for (auto it : _bombs )
	{
		it->Update();
		if (!it->GetTimer()) //BOOM!!
		{
			Boom(it->GetPos());
		}
		++it;
	}

	_bombs.erase(std::remove_if(_bombs.begin(), _bombs.end(), [](Bomb* b) { return b->GetTimer() == 0; }), _bombs.end());
}

void Board::Update(sf::Event e)
{
	if (gameover)
		return;

	std::pair<int, int> pos = _player->GetPos();
	_player->Update();

	if (_bots.size() == 0)
	{
		//load level 2
		_reLoad = 3;
		Load("./Board/lvl_2.txt");	
	}

	UpdateBonus(pos);
	MovingBots(pos);
	UpdateBombs();

	int maxBombs = _player->GetMaxBombs();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		if (_board[pos.second][pos.first - 1] == BoardSpace)
			_player->Move(-1, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		if (_board[pos.second][pos.first + 1] == BoardSpace)
			_player->Move(1, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		if (_board[pos.second - 1][pos.first] == BoardSpace)
			_player->Move(0, -1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		if (_board[pos.second + 1][pos.first] == BoardSpace)
			_player->Move(0, 1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (_bombs.size() <= maxBombs)
			_bombs.push_back(new Bomb(pos.first, pos.second, 500));	
	}
}

void Board::Hit()
{
	_reLoad--;
	if (!_reLoad)
	{
		gameover = true;
	}
	else
	{
		Load(_path);
	}
	return;
}

void Board::DrawBonus(std::pair<int, int> pos)
{
	int bonus = rand() % 20;
	if (bonus == 1)
	{
		_bonus.push_back(new BaseObject("./Sprites/bonuslife.png", pos.first, pos.second));
	}
	else if (bonus == 2 || bonus == 4 || bonus == 8)
	{
		_bonus.push_back(new BaseObject("./Sprites/bonusbomb.png", pos.first, pos.second));
	}
}

void Board::Boom(std::pair<int, int> pos)
{
	if (_board[pos.second][pos.first - 1] == DestroyableWall)
	{
		_board[pos.second][pos.first - 1] = BoardSpace;
		_walls.erase(std::pair<int, int>(pos.first - 1, pos.second));
		DrawBonus(std::pair<int, int>(pos.first - 1, pos.second));
	}
	if (_board[pos.second][pos.first + 1] == DestroyableWall)
	{
		_board[pos.second][pos.first + 1] = BoardSpace;
		_walls.erase(std::pair<int, int>(pos.first + 1, pos.second));
		DrawBonus(std::pair<int, int>(pos.first + 1, pos.second));
	}
	if (_board[pos.second - 1][pos.first] == DestroyableWall)
	{
		_board[pos.second - 1][pos.first] = BoardSpace;
		_walls.erase(std::pair<int, int>(pos.first, pos.second - 1));
		DrawBonus(std::pair<int, int>(pos.first, pos.second - 1));
	}
	if (_board[pos.second + 1][pos.first] == DestroyableWall)
	{
		_board[pos.second + 1][pos.first] = BoardSpace;
		_walls.erase(std::pair<int, int>(pos.first, pos.second + 1));
		DrawBonus(std::pair<int, int>(pos.first, pos.second + 1));
	}

	for (auto bot = _bots.begin(); bot != _bots.end();)
	{
		auto botPos = (*bot)->GetPos();
		if (pos == botPos || (pos.first == botPos.first && pos.second == botPos.second - 1)
			|| (pos.first == botPos.first && pos.second == botPos.second + 1)
			|| (pos.first == botPos.first - 1 && pos.second == botPos.second)
			|| (pos.first == botPos.first + 1 && pos.second == botPos.second))
			bot = _bots.erase(bot);
		else
			bot++;
	}

	auto playerPos = _player->GetPos();
	if (pos == playerPos || (pos.first == playerPos.first && pos.second == playerPos.second - 1)
		|| (pos.first == playerPos.first && pos.second == playerPos.second + 1)
		|| (pos.first == playerPos.first - 1 && pos.second == playerPos.second)
		|| (pos.first == playerPos.first + 1 && pos.second == playerPos.second))
	{
		Hit();
	}
}

void Board::Draw(sf::RenderWindow* const window)
{
	if (gameover)
	{
		auto end = new BaseObject("./Sprites/gameover.png", 0, 0);
		end->Draw(window);
		delete end;
		return;
	}

	for (auto wall : _walls)
		wall.second->Draw(window);
	for (auto bonus : _bonus)
		bonus->Draw(window);
	_player->Draw(window);
	for (auto bot : _bots)
		bot->Draw(window);
	for (auto bomb : _bombs)
		bomb->Draw(window);
}

Board::~Board()
{
	Clean();
}

void Board::Clean()
{
	for (auto bot : _bots)
		delete bot;
	for (auto bomb : _bombs)
		delete bomb;
	for (auto wall : _walls)
		delete wall.second;
	for (auto bonus : _bonus)
		delete bonus;
	if (_player)
		delete _player;
	if (_board)
		delete[] _board;
	_bombs.clear();
	_bots.clear();
	_walls.clear();
	_bonus.clear();
}