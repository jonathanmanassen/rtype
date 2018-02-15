#pragma once

class AUDPSocket
{
public:
	AUDPSocket() = default;
	virtual ~AUDPSocket() = default;
	virtual void	init() = 0;
	virtual void	bind() = 0;
	virtual void	start() = 0;
	virtual void	send(char* str, int size) = 0;
};