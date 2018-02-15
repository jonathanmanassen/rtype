#include <iostream>
#include <thread>
#include "stdafx.h"
#include "Ressources.h"
#include "SfmlTcpSocket.h"
#include <Windows.h>
#include <mutex>

void	SfmlTcpSocket::init()
{

}

bool	SfmlTcpSocket::connect(const std::string &ip, const unsigned short &port)
{
	this->port = port;
	this->ip = ip;
	sf::Socket::Status status = tcpSocket.connect(this->ip, this->port);
	if (status != sf::Socket::Done)
	{
		std::cout << "can't connect" << std::endl;
		return false;
	}
	return true;
}

void	SfmlTcpSocket::disconnect()
{
	tcpSocket.disconnect();
}

bool	SfmlTcpSocket::receive(char* str, const int &size)
{
	size_t	received;
	size_t s = size;
//	int id = 5000;
	//std::cout << "Avant receive" << std::endl;
	if (tcpSocket.receive(&str, size, received) != sf::Socket::Done)//reinterpret_cast<char*>(&id), sizeof(int), received) != sf::Socket::Done)
	{
		std::cerr << "Error receiving message" << std::endl;
		return false;
	}
//	std::cout << id << std::endl;
	return true;
}

bool	SfmlTcpSocket::send(char* str, const int &size)
{
	size_t s = size;
	if (tcpSocket.send(str, s) != sf::Socket::Done)
	{
		std::cerr << "Can't send message, server probably down" << std::endl;
		return false;
	}
	return true;
}

bool	SfmlTcpSocket::isConnected()
{
	if (tcpSocket.getRemoteAddress() != sf::IpAddress::None)
		return true;
	else
		return false;
}
