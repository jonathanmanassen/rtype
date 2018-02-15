#pragma once
#include <memory>
#include "GameRoom.h"

class GameRoomManager
{
	static GameRoomManager	*manager;
	GameRoomManager() = default;
	~GameRoomManager() = default;
public:
	static	GameRoomManager *GameRoomManager::get();
	void addGameRoom(Client* client);

	void destroy() const;
	std::vector<std::shared_ptr<GameRoom>>	gameRooms;
};