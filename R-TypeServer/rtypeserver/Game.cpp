#include "stdafx.h"
#include "Game.h"
#include "IMonster.h"
#include <iostream>
#include <windows.h>
#include "PhysicSystem.h"
#include "PhysicComponent.h"
#include "Ressource.h"
#include <random>
#include <chrono>
#include <thread>
#include "UDPServer.h"

Game::Game(const int &_level, Client* _clients[4], const int &_nbClients)
{
	std::cout << "construct Game" << std::endl;
	level = _level;
	res = std::make_shared<Ressource>(level);
	physic = std::make_shared<PhysicSystem>(res);
	if (_clients == nullptr)
		return;
	nbClients = _nbClients;
	for (int i = 0; i < 4; i++)
		clients[i] = _clients[i];
}


Game::~Game()
{
}

void	Game::update(const int &clientPos)
{
	if (clientsId.size() != 0)
	{
		if (physic->hasEntity(clientsId.at(clientPos)))
		{
			physic->removeEntity(clientsId.at(clientPos));
			clientsId.erase(clientsId.begin() + clientPos);
		}
	}
	int i = clientPos;
	while (i < nbClients)
	{
		clients[i] = clients[i + 1];
		i++;
	}
	nbClients--;
}

void	Game::start()
{
	std::vector<std::string> players;

	players.emplace_back("StarshipBlue");
	players.emplace_back("StarshipYellow");
	players.emplace_back("StarshipGreen");
	players.emplace_back("StarshipRed");

	for (int i = 0; i < nbClients; i++)
		clientsId.emplace_back(physic->createMonster(players.at(i) , 200, 200 * (i + 1), 1));

	std::vector<ServerLoadRessourcesInfo> list;
	res->getRessourcesInfo(list);
	
	gameIsFinished = false;
	std::chrono::time_point<std::chrono::system_clock> t = std::chrono::system_clock::now();
	int i = 0;
	while (gameIsFinished != true)
	{
		gameLevel(level, i);

		physic->run(i, clients, nbClients, clientsId);
		i++;

		std::vector<ServerInGameInfo> vec;
		physic->getEntitiesInfo(vec);

		for (auto &tmp : vec)
		{
			for (int j = 0; j < nbClients; j++)
			{
				UDPServer::get()->send(clients[j], reinterpret_cast<char*>(&tmp), sizeof(ServerInGameInfo));
			}
		}
		if (clientsId.size() == 0 || nbClients == 0)
			gameIsFinished = true;
		t += std::chrono::milliseconds(8);
		std::this_thread::sleep_until(t);
	}
}

void	Game::gameLevel(const int &level, const int &i)
{
	std::default_random_engine generator(static_cast<unsigned int>(time(0)));
	std::uniform_int_distribution<int> distribution(1, 10000);
	int dice_roll = distribution(generator);

	if (level == 1)
	{
		if (i % 1000 == 0)
			physic->createMonster(randomMonster(), 1920, 500 % 1080, 0);
		if (i % 1130 == 0)
		{
			physic->createMonster(randomMonster(), 1920, 600 % 1080, 0);
			physic->createMonster(randomMonster(), 1920, 400 % 1080, 0);
		}
		if (i % 1160 == 0)
		{
			physic->createMonster(randomMonster(), 1920, 700 % 1080, 0);
			physic->createMonster(randomMonster(), 1920, 300 % 1080, 0);
		}

		if (i % 1500 == 0)
			physic->createMonster(randomMonster(), 1920, distribution(generator) % 1080, 0);
	}
	else if (level == 2)
	{
		if (i % 100 == 0)
			physic->createMonster(randomMonster(), 1920, distribution(generator) % 1080, 0);
	}
	else
	{
		if (i == 1)
			physic->createMonster(randomMonster(), 1320, 150, 0);
	}
}

std::string	Game::randomMonster() const
{
	std::default_random_engine generator(static_cast<unsigned int>(time(0)));
	std::uniform_int_distribution<int> distribution(1, 10000);
	int dice_roll = distribution(generator);

	size_t size = res->monsters.size();
	std::cout << size << dice_roll << std::endl;

	std::vector<std::string>	list;

	for (auto &monster : res->monsters)
	{
		list.emplace_back(monster.first);
	}
	std::string m;
	while (true)
	{
		m = list[distribution(generator) % list.size()];
		if (m.find("missile") == std::string::npos && m.find("starship") == std::string::npos &&
			m.find("Starship") == std::string::npos && m.find("MonsterGeneric") == std::string::npos)
			return (m);
	}
	return ("missile");
}