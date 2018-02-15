#include <iostream>
#include <thread>
#include "stdafx.h"
#include "Ressources.h"
#include "SfmlUdpSocket.h"
#include <Windows.h>
#include <mutex>

void	SfmlUdpSocket::init()
{

}

bool	SfmlUdpSocket::bind(const unsigned short &port)
{
	this->port = port;
	sf::Socket::Status status = udpSocket.bind(port);
	if (status != sf::Socket::Done)
		return false;
	return true;
}

void	SfmlUdpSocket::unbind()
{
	udpSocket.unbind();
}

bool	SfmlUdpSocket::receive(char* str, const size_t &size, unsigned short port)
{
	size_t	received;
	sf::IpAddress sender;
	if (udpSocket.receive(str, size, received, sender, port) != sf::Socket::Done)
	{
		std::cout << "erreur dans données udp" << std::endl;
		return false;
	}
	return true;
}

bool	SfmlUdpSocket::send(char* str, const int &size, const std::string &ip, const unsigned short &port)
{
	if (udpSocket.send(str, size, ip, port) != sf::Socket::Done)
	{
		std::cerr << "Can't send message, server probably down" << std::endl;
		return false;
	}
	return true;
}
