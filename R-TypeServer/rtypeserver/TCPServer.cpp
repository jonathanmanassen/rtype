#include "stdafx.h"
#include "TCPServer.h"

TCPServer::TCPServer(const int &port) : port(port)
{
	init();
	bind();
	listen();
}

TCPServer::~TCPServer()
{
	closesocket(sd);
	WSACleanup();
}

void TCPServer::init()
{
	if (WSAStartup(MAKEWORD(2, 2), &w) != 0) {
		throw std::runtime_error("Windows connection failed");
	}
	/* Open a datagram socket */
	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sd == INVALID_SOCKET)
	{
		WSACleanup();
		throw std::runtime_error("socket error");
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	std::cout << "server correctly initialised" << std::endl;
}

void TCPServer::bind()
{
	if (::bind(sd, reinterpret_cast<struct sockaddr*>(&server), sizeof(struct sockaddr_in)) == -1)
		throw std::runtime_error("bind failed");
}

void TCPServer::listen()
{
	if (::listen(sd, SOMAXCONN) != 0)
		throw std::runtime_error("listen failed");
}

void	TCPServer::start()
{
	int addrlen = sizeof(client);
	static int	id = 1;

	while (1)
	{
		SOCKET newClient = accept(sd, (SOCKADDR*)(&client), &addrlen);
		if (newClient != INVALID_SOCKET)
		{
			clients.emplace(newClient, std::make_shared<Client>(this, id++));
		}
	}
}

void	TCPServer::recv(Client *client, char *str, const int &size)
{
	for (auto &tmp : clients)
		if (client->getId() == tmp.second->getId())
			if (::recv(tmp.first, str, size, 0) <= 0)
				throw std::runtime_error("disconnected");
}

void	TCPServer::send(Client *client, char *str, const int &size)
{
	for (auto &tmp : clients)
		if (client->getId() == tmp.second->getId())
			if (::send(tmp.first, str, size, 0) <= 0)
				return;
}

void	TCPServer::destroy(Client *client)
{
	for (auto &tmp : clients)
		if (client->getId() == tmp.second->getId())
		{
			client->t.detach();
			clients.erase(clients.find(tmp.first));
			return;
		}
}

void TCPServer::updateAll()
{
	for (auto &tmp : clients)
	{
		if (tmp.second->getRoomId() == -1)
		{
			std::cout << tmp.second->getName() << std::endl;
			tmp.second->sendAllRooms();
		}
	}
}