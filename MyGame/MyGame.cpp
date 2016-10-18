// MyGame.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Board.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1250, 650), "Bomberman");
	Board board;
	board.Load("./Board/lvl_1.txt");

	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		board.Update(e);
		board.Draw(&window);

		window.display();
	}
    return 0;
}

