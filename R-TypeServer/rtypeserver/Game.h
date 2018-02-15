#pragma once
#include <iostream>
#include "PhysicSystem.h"
#include "Client.h"

class Game
{
	std::vector<int>	clientsId;
	Client	*clients[4];
	int		nbClients;
	int		level;
public:
	std::shared_ptr<PhysicSystem>	physic;
	std::shared_ptr<Ressource> res;

public:
	Game(const int &_level, Client* _clients[4], const int&);
	~Game();
	void start();
	void	gameLevel(const int &level, const int &i);

	std::string randomMonster() const;
	bool	gameIsFinished;
	void	update(const int&);
};

