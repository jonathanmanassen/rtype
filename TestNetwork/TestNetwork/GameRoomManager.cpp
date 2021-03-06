#include "stdafx.h"
#include "GameRoomManager.h"

GameRoomManager	*GameRoomManager::manager = nullptr;

GameRoomManager *GameRoomManager::get()
{
	if (!manager)
		manager = new GameRoomManager;
	return (manager);
}

void GameRoomManager::addGameRoom(Client *client)
{
	static int i = 1;
	gameRooms.emplace_back(std::make_shared<GameRoom>(client, 1, i));
	gameRooms[gameRooms.size() - 1]->updateAll();
	i++;
}

void GameRoomManager::destroy() const
{
	delete manager;
}
