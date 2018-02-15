#pragma once

#include <thread>
#include <unordered_map>
#include <iostream>
#include <winsock.h>
#include "ITCPSocket.h"
#include "Protocol.h"
#include "GameRoom.h"

class GameRoom;

class Client
{
	struct Dir
	{
		bool up = false;
		bool right = false;
		bool down = false;
		bool left = false;
	};
	typedef void (Client::*fct)();

	int					id;
	ConnectionMessage	message;
	char				name[30];
	int					roomId = -1;
	std::unordered_map<ConnectionAction, fct>	func;
	Dir					dir;
	bool				shooting = false;
public:
	bool						udpInit = false;
	struct sockaddr_in			udp;
	ITCPSocket			*socket;
	std::thread			t;
public:
	Client() = default;
	Client(ITCPSocket*, const int&);
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
	void	setDir(const Dir &dir) { this->dir = dir; }
	void	setDir(const bool &up, const bool &right, const bool &down, const bool &left) { dir.up = up; dir.right = right; dir.down = down; dir.left = left; }
	void	setShooting(const bool &shoot) { shooting = shoot; }
	const Dir &getDir() const { return dir; }
	const bool &getShooting() const { return shooting; }
	const int &getRoomId() const { return roomId; }
};