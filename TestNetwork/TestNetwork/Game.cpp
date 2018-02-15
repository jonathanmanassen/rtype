#include "stdafx.h"
#include "Game.h"

Game::Game(Client* _clients[4])
{
	for (int i = 0; i < 4; i++)
		clients[i] = _clients[i];
}

Game::~Game()
{
}

void Game::start()
{
	std::cout << "game started" << std::endl;
	while (1)
	{
	}
}
