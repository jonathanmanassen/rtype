#pragma once

class Client;

class ATCPSocket
{
public:
	ATCPSocket() = default;
	virtual ~ATCPSocket() = default;
	virtual void	init() = 0;
	virtual void	bind() = 0;
	virtual void	listen() = 0;
	virtual void	start() = 0;
	virtual void	recv(Client *client, char* str, int size) = 0;
	virtual void	send(Client *client, char* str, int size) = 0;
	virtual void	destroy(Client *client) = 0;
};