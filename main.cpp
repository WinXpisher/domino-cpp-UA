#include "Header/Game.h"
#include <Windows.h>
#include <cstdlib>
#include <ctime>

int main() 
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(NULL));

	Game game;
	while (game.Run())
	{
		game.Draw();
	}
}
