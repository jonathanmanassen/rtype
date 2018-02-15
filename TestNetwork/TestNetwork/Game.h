#pragma once
#include <iostream>
#include "Client.h"

class Game
{
	Client	*clients[4];
public:
	Game(Client* _clients[4]);
	~Game();
	void start();
};

