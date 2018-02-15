#pragma once

class IUDPSocket
{
public:
	IUDPSocket() = default;
	virtual ~IUDPSocket() = default;
	virtual void	init() = 0;
	virtual void	bind() = 0;
	virtual void	start() = 0;
	virtual void	send(Client *client, char *buff, const int &size) = 0;
};