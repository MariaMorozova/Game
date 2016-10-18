#include "stdafx.h"
#include "Board.h"
#include <fstream>
#include <iostream>

Board::Board()
{

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

		for (int i = 0; i < _height; i++)
			_board[i] = new int[_width];

		for (int i = 0; i < _height; i++)
		{
			char* str = new char[_width + 1];
			fileIn.read(str, _width + 1);

			for (int j = 0, k = 0; j < _width; j++, k++)
			{
				switch (str[k])
				{
				case 'b':
					_bots.push_back(new Player(j, i));
				case ' ':
					_board[i][j] = 0;
					break;
				case '#':
					_board[i][j] = 1;		//нерушимые стены
					_walls[std::pair<int, int>(j, i)] = new BaseObject("./Sprites/wall1.png", j, i);
					break;
				case 'p':
					_board[i][j] = 0;
					_player = new Player(j, i, false);
					break;
				case '1':		//разрушаемые
					_board[i][j] = 2;
					_walls[std::pair<int, int>(j, i)] = new BaseObject("./Sprites/wall2.png", j, i);
					break;
				case 'q':

					_board[i][j] = -1;
					break;
				default:
					j--; // \n
					break;
				}
			}
		}
	}
}

void Board::Update(sf::Event e)
{
	if (gameover)
		return;

	std::pair<int, int> pos = _player->GetPos();
	_player->Update();

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
					_player->IncPlayerHP();
				}
				i = _bonus.erase(i);
			}
			else
				++i;
		}
	

	for (auto bot : _bots)
	{
		bot->Update();
		bool hor = rand() % 2,
			forward = rand() % 2;
		std::pair<int, int> botPos = bot->GetPos();

		if (botPos == pos)
		{
			_player->Hit();
			if (_player->GetPlayerHP() != 0)
					Load(_path);
			
			return;
		}

		if (hor && forward && _board[botPos.second][botPos.first + 1] <= 0) //right		
			bot->Move(1, 0);
		else if (hor && !forward && _board[botPos.second][botPos.first - 1] <= 0) //left
			bot->Move(-1, 0);
		else if (!hor && forward && _board[botPos.second - 1][botPos.first] <= 0) //up
			bot->Move(0, -1);
		else if (!hor && !forward && _board[botPos.second + 1][botPos.first] <= 0) //down
			bot->Move(0, 1);
	}

	for (auto it = _bombs.begin(); it != _bombs.end(); )
	{
		(*it)->Update();
		if (!(*it)->GetTimer())
		{
			//BOOM!!
			Boom((*it)->GetPos());
			it = _bombs.erase(it);
		}
		else
		{
			++it;
		}
	}

	int maxBombs = _player->GetMaxBombs();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		gameover = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		if (_board[pos.second][pos.first - 1] <= 0)
			_player->Move(-1, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		if (_board[pos.second][pos.first + 1] <= 0)
			_player->Move(1, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		if (_board[pos.second - 1][pos.first] <= 0)
			_player->Move(0, -1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		if (_board[pos.second + 1][pos.first] <= 0)
			_player->Move(0, 1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		//if (_bombs.size() <= maxBombs)
			_bombs.push_back(new Bomb(pos.first, pos.second, 500));	//todo: check bombs count
	}
}

void Board::DrawBonus(std::pair<int, int> pos) 
{
	int bonus = rand() % 10;
	if (bonus == 1)
	{
		_bonus.push_back(new BaseObject("./Sprites/bonuslife.png", pos.first, pos.second));
	}
	else if (bonus == 2)
	{
		_bonus.push_back(new BaseObject("./Sprites/bonusbomb.png", pos.first, pos.second));
	}
}

void Board::Boom(std::pair<int, int> pos)
{
	if (_board[pos.second][pos.first - 1] == 2)
	{
		_board[pos.second][pos.first - 1] = 0;
		_walls.erase(std::pair<int, int>(pos.first - 1, pos.second));
		DrawBonus(std::pair<int, int>(pos.first - 1, pos.second));
	}
	if (_board[pos.second][pos.first + 1] == 2)
	{
		_board[pos.second][pos.first + 1] = 0;
		_walls.erase(std::pair<int, int>(pos.first + 1, pos.second));
		DrawBonus(std::pair<int, int>(pos.first + 1, pos.second));
	}
	if (_board[pos.second - 1][pos.first] == 2)
	{
		_board[pos.second - 1][pos.first] = 0;
		_walls.erase(std::pair<int, int>(pos.first, pos.second - 1));
		DrawBonus(std::pair<int, int>(pos.first, pos.second - 1));
	}
	if (_board[pos.second + 1][pos.first] == 2)
	{
		_board[pos.second + 1][pos.first] = 0;
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
		_player->Hit();
		if (_player->GetPlayerHP() == 0)
		{
			gameover = true;
			return;
		}
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