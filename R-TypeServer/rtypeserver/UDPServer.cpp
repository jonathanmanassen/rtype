#include "stdafx.h"
#include "UDPServer.h"

UDPServer	*UDPServer::manager = nullptr;

UDPServer *UDPServer::get(const int &port)
{
	if (!manager)
		manager = new UDPServer(port);
	return (manager);
}

UDPServer::UDPServer(int port) : port(port)
{
	init();
	bind();
	t = std::thread(&UDPServer::start, this);
}

UDPServer::~UDPServer()
{
	closesocket(sd);
	WSACleanup();
}

void UDPServer::init()
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

void UDPServer::bind()
{
	if (::bind(sd, reinterpret_cast<struct sockaddr*>(&server), sizeof(struct sockaddr_in)) == -1)
	{
		throw std::runtime_error("bind failed");
	}
}

void UDPServer::start()
{
	while (true)
	{
		ControllerAction	buff;
		int client_length = static_cast<int>(sizeof(struct sockaddr_in));

		if (recvfrom(sd, reinterpret_cast<char*>(&buff), sizeof(ControllerAction), 0, reinterpret_cast<sockaddr*>(&client), &client_length) < 0)
		{
			std::cerr << "Could not receive datagram." << std::endl;
			return;
		}
		for (auto &tmp : GameRoomManager::get()->gameRooms)
		{
			for (int i = 0; i < tmp->getNbClients(); i++)
			{
				if (tmp->clients[i]->getId() == buff.id)
				{
					tmp->clients[i]->udp = this->client;
					tmp->clients[i]->udpInit = true;
					tmp->clients[i]->setDir(buff.up, buff.right, buff.down, buff.left);
					tmp->clients[i]->setShooting(buff.isShooting);
				}
			}
		}
	}
}

void	UDPServer::send(Client *client, char *buff, const int &size)
{
	if (client->udpInit)
		sendto(sd, buff, size, 0, reinterpret_cast<sockaddr*>(&client->udp), sizeof(struct sockaddr_in));
}