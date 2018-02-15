#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <winsock.h>
#include "Client.h"
#include "IUDPSocket.h"
#include "GameRoomManager.h"

class UDPServer : public IUDPSocket
{
private:
	std::thread	t;
	WSADATA w;
	const int	port = 0;
	SOCKET sd;
	struct sockaddr_in server;
	struct sockaddr_in client;
	static UDPServer	*manager;
	UDPServer(int port);
	virtual ~UDPServer();
public:
	static	UDPServer *UDPServer::get(const int &port = 4243);

	virtual void	init();
	virtual void	bind();
	virtual void	start();
	virtual void	send(Client *client, char *buff, const int &size);
};