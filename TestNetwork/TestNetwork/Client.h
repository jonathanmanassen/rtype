#pragma once

#include <thread>
#include <unordered_map>
#include <iostream>
#include "ATCPSocket.h"
#include "Protocol.h"
#include "GameRoom.h"

class GameRoom;

class Client
{
	typedef void (Client::*fct)();

	int					id;
	ATCPSocket			*socket;
	ConnectionMessage	message;
	char				name[30];
	int					roomId = -1;
	std::unordered_map<ConnectionAction, fct>	func;
public:
	std::thread			t;
public:
	Client() = default;
	Client(ATCPSocket*, const int&);
	void	read();
	void	hello();
	void	sendAllRooms();
	void	createRoom();
	void	enterRoom();
	void	leaveRoom();
	void	update(const GameRoom & room);
	void	ready();
	void	loaded();

	void	setRoomId(const int &id) { roomId = id; }
	char	*getName() { return name; }
	const int	&getId() const { return id; }
};
