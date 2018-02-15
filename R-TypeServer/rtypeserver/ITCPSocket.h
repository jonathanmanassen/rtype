#pragma once

class Client;

class ITCPSocket
{
public:
	ITCPSocket() = default;
	virtual ~ITCPSocket() = default;
	virtual void	init() = 0;
	virtual void	bind() = 0;
	virtual void	listen() = 0;
	virtual void	start() = 0;
	virtual void	recv(Client *client, char* str, const int &size) = 0;
	virtual void	send(Client *client, char* str, const int &size) = 0;
	virtual void	destroy(Client *client) = 0;
	virtual void	updateAll() = 0;
};