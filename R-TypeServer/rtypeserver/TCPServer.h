#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>
#include <winsock.h>
#include "ITCPSocket.h"
#include "Client.h"

class TCPServer : public ITCPSocket
{
	WSADATA w;
	const int	port = 0;
	SOCKET sd;
	struct sockaddr_in server;
	struct sockaddr_in client;
	std::unordered_map<SOCKET, std::shared_ptr<Client>>	clients;
public:
	TCPServer(const int&);
	virtual ~TCPServer();
	virtual void	init();
	virtual void	bind();
	virtual void	listen();
	virtual void	start();
	virtual void	recv(Client *client, char* str, const int &size);
	virtual void	send(Client *client, char* str, const int &size);
	virtual void	destroy(Client* client);
	virtual void	updateAll();
};