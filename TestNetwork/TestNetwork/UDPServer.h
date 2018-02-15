#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <winsock.h>
#include "Client.h"
#include "AUDPSocket.h"

class UDPServer : public AUDPSocket
{
private:
	WSADATA w;
	const int	port = 0;
	SOCKET sd;
	struct sockaddr_in server;
	struct sockaddr_in client;
	std::vector<std::shared_ptr<Client>>	clients;

public:
	UDPServer(int port);
	virtual ~UDPServer();
	virtual void	init();
	virtual void	bind();
	virtual void	start();
};

inline UDPServer::UDPServer(int port) : port(port)
{
	init();
	bind();
}

inline UDPServer::~UDPServer()
{
	closesocket(sd);
	WSACleanup();
}

inline void UDPServer::init()
{
	if (WSAStartup(MAKEWORD(2, 2), &w) != 0) {
		throw std::runtime_error("Windows connection failed");
	}

	/* Open a datagram socket */
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd == INVALID_SOCKET)
	{
		WSACleanup();
		throw std::runtime_error("socket error");
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	std::cout << "UDPServer correctly initialised" << std::endl;
}

inline void UDPServer::bind()
{
	if (::bind(sd, reinterpret_cast<struct sockaddr*>(&server), sizeof(struct sockaddr_in)) == -1)
	{
		throw std::runtime_error("bind failed");
	}
}

inline void UDPServer::start()
{
	while (true)
	{
		char	buff[1024];
		int client_length = static_cast<int>(sizeof(struct sockaddr_in));

		if (recv(sd, reinterpret_cast<char*>(&buff), 1024, 0) < 0)
		{
			std::cerr << "Could not receive datagram." << std::endl;
			return;
		}
	}
}
