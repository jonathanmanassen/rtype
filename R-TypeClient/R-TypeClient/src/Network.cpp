#include <iostream>
#include <thread>
#include "stdafx.h"
#include "Ressources.h"
#include "Network.h"
#include <Windows.h>
#include <mutex>

extern GameStat gameStat;
std::mutex mutexs;
Network	*Network::network = nullptr;

Network	*Network::get()
{
	if (!network)
		network = new Network;
	return (network);
}

Network::Network()
{
	tcpThread = std::thread(&Network::receiveTcpMessage, this);
	udpThread = std::thread(&Network::receiveUdpMessage, this);
	udpSendThread = std::thread(&Network::sendUdpMessages, this);
}

void	Network::sendUdpMessages()
{
	while (gameStat != GameStat::GAME_END)
	{
		if (gameStat == GameStat::IN_GAME && messagesInQueue.size() > 0)
			{
				ControllerAction message = messagesInQueue.front();
				if (udpSocket.send(&message, sizeof(ControllerAction), ip, 4243) != sf::Socket::Done)
				{
					std::cerr << "Can send message, server probably down" << std::endl;
				}
				messagesInQueue.pop_front();
			}
		Sleep(1);
	}
}

bool	Network::hasInGameInfo(const int &id)
{
	std::unordered_map<int, ServerInGameInfo>::iterator	it = inGameInfo.find(id);

	if (it == inGameInfo.end())
		return (false);
	return (true);
}

void Network::receiveUdpMessage()
{
	sf::IpAddress sender;
	std::size_t received;
	udpSocket.setBlocking(true);
	while (gameStat != GameStat::GAME_END)
	{
		if (gameStat == GameStat::IN_GAME)
			if (udpSocket.getLocalPort() != 0)
			{
				ServerInGameInfo data;
				sf::Socket::Status status;
				if ((status = udpSocket.receive(reinterpret_cast<char*>(&data), sizeof(ServerInGameInfo), received, sender, udpPort)) != sf::Socket::Done)
				{
					std::cout << "erreur dans donnée1 udp" << std::endl;
/*					gameStat = GameStat::CONNECTED;
					udpSocket.unbind();*/
				}
				else
				{
					if (isClearingInGameInfo == true)
						clearInGameInfo();
					if (inGameInfo.size() == 0 || hasInGameInfo(data.entityId) == false)
						inGameInfo.emplace(data.entityId, data);
					else
						inGameInfo.at(data.entityId) = data;
					isUnreadedUdpMessage = true;
				}
			}
	}
}

void Network::tcpDisconnnect()
{
	std::cout << "Error in data" << std::endl;
	if (gameStat != GameStat::CONNEXION)
		gameStat = GameStat::CONNEXION;
	lastAction = "Disconnect from Server";
	tcpSocket.disconnect();
}

void Network::receiveTcpMessage()
{
	std::size_t received;
	std::cout << "start receiving" << std::endl;
	while (gameStat != GameStat::GAME_END)
	{
		if (tcpSocket.getRemoteAddress() != sf::IpAddress::None)
		{
			std::cout << "receiving" << std::endl;
			char type[3];
			type[0] = '\0';
			if (tcpSocket.receive(reinterpret_cast<char*>(&type), sizeof(char[3]), received) != sf::Socket::Done)
				tcpDisconnnect();
			lastMessageType = type;
			std::cout << "receive:" << lastMessageType << std::endl;
			if (lastMessageType.compare("ID") == 0 && gameStat != GameStat::IN_GAME)
			{
				int sId;
				if (tcpSocket.receive(reinterpret_cast<char*>(&sId), sizeof(int), received) != sf::Socket::Done)
					tcpDisconnnect();
				id = sId;
			}
			else if (lastMessageType.compare("RO") == 0 && gameStat != GameStat::IN_GAME)
			{
				std::vector<ServerRoomsInfo> data;
				int sizeData;
				if (tcpSocket.receive(reinterpret_cast<char*>(&sizeData), sizeof(int), received) != sf::Socket::Done)
					tcpDisconnnect();
				std::cout << sizeData << std::endl;
				if (sizeData > 0)
				{
					data.resize(sizeData);
					if (tcpSocket.receive(reinterpret_cast<char*>(&data[0]), sizeof(ServerRoomsInfo) * sizeData, received) != sf::Socket::Done)
						tcpDisconnnect();
					std::cout << "Received " << received << " bytes" << std::endl;
					roomsInfo = data;
					std::cout << roomsInfo.at(0).ownerName << std::endl;
				}
				else
				{
					roomsInfo.clear();
					lastAction = "Receive Rooms!";
				}
				isUnreadedTcpMessage = true;
				gameStat = GameStat::CONNECTED;
			}
			else if (lastMessageType.compare("IN") == 0 && gameStat != GameStat::IN_GAME)
			{
				ServerInRoomInfo data;
				if (tcpSocket.receive(reinterpret_cast<char*>(&data), sizeof(ServerInRoomInfo), received) != sf::Socket::Done)
					tcpDisconnnect();
				else
				{
					lastAction = "Welcome in ";
					lastAction += data.playersName[0];
					lastAction += " room";
				}
				std::cout << "Received " << received << " bytes" << std::endl;
				inRoomInfo = data;
				isUnreadedTcpMessage = true;
				gameStat = GameStat::IN_ROOM;
			}
			else if (lastMessageType.compare("RE") == 0 && gameStat != GameStat::IN_GAME)
			{
				std::vector<ServerLoadRessourcesInfo> data;
				int sizeData;
				if (tcpSocket.receive(reinterpret_cast<char*>(&sizeData), sizeof(int), received) != sf::Socket::Done)
					tcpDisconnnect();
				std::cout << sizeData << std::endl;
				if (sizeData > 0)
				{
					data.resize(sizeData);
					if (tcpSocket.receive(reinterpret_cast<char*>(&data[0]), sizeof(ServerLoadRessourcesInfo) * sizeData, received) != sf::Socket::Done)
						tcpDisconnnect();
					else
						lastAction = "Ressources correctly received";
					std::cout << "Received " << received << " bytes" << std::endl;
					ressourcesInfo = data;
					Ressources::get()->loadFolder("sprites/level1");
					for (ServerLoadRessourcesInfo info : ressourcesInfo)
					{
						//std::cout << "EntityType:" << info.entityName << ":x:" << info.beginX<< ":y:" << info.beginY<< ":height:" << info.height<< ":width:" << info.width<< ":incX:" << info.incX<< ":nbSprite" << info.nbSprite<< std::endl;
						Ressources::get()->actualiseAnimationSet(info);
					}
					sendTcpMessage(ConnectionAction::LOADED, "", 0, "");
					std::cout << "loaded" << std::endl;
				}
			}
		}
	}
}

bool Network::initUdpNetwork()
{
	udpPort = port + 1;
	/*if (udpSocket.bind(4244) != sf::Socket::Done)
		return false;*/
	ControllerAction message;
	message.id = id;
	std::cout << "control init" << std::endl;
	if (udpSocket.send(&message, sizeof(ControllerAction), ip, 4243) != sf::Socket::Done)
	{
		std::cerr << "Can send message, server probably down" << std::endl;
		udpSocket.unbind();
		return false;
	}
	return true;
}

bool Network::initTcpNetwork(std::string const &ip, int const &port)
{
	this->port = port;
	this->ip = ip;
	this->address = ip;
	sf::Socket::Status status = tcpSocket.connect(ip, port);
	if (status != sf::Socket::Done)
	{
		lastAction = "Failed To Connect";
		return false;
	}
	lastAction = "Succesfully connected to server " + ip;
	return true;
}

void	Network::sendTcpMessage(const ConnectionAction &act, std::string const &name, const int &roomNb, const std::string &level)
{
	ConnectionMessage message;
	switch (act)
	{
	case ConnectionAction::HELLO:
	int i;
	for (i = 0; i < name.size() && i < 29; i++)
		message.name[i] = name.c_str()[i];
	message.name[i] = '\0';
			break;
	case ConnectionAction::ENTER_ROOM:
	case ConnectionAction::CREATE_ROOM :
		if (level.find("Level ") != std::string::npos)
		{
			message.level = std::atoi(level.substr(6).c_str());
			std::cout << "Level:" << message.level << std::endl;
			message.act = ConnectionAction::CREATE_ROOM;
		}
		else
		{
			message.level = 0;
			message.act = ConnectionAction::ENTER_ROOM;
		}
		message.roomNb = roomNb;
		break;
	case ConnectionAction::LEAVE_ROOM:
		break;
	case ConnectionAction::READY:
		break;
	case ConnectionAction::LOADED:
		break;
	}
	message.act = act;
	if (tcpSocket.send(&message, sizeof(ConnectionMessage)) != sf::Socket::Done)
	{
		std::cerr << "Can send message, server probably down" << std::endl;
	}
}

void	Network::sendControllerMessage(const bool&right, const bool&left, const bool&up, const bool&down, bool const &isShooting)
{
	ControllerAction message;
	message.isShooting = isShooting;
	message.right = right;
	message.left = left;
	message.up = up;
	message.down = down;
	message.id = id;
//	std::cout << "right:" <<right << ":left:" << left << ":up:" << up << ":down:" << down << ":shoot:" << isShooting << std::endl;
	messagesInQueue.push_back(message);
/*	if (udpSocket.send(&message, sizeof(ControllerAction), ip, 4243) != sf::Socket::Done)
	{
		std::cerr << "Can send message, server probably down" << std::endl;
	}*/
}

void	Network::disconnectTcpFromServer()
{
	tcpSocket.disconnect();
}

void	Network::disconnectUdpFromServer()
{
	udpSocket.unbind();
}
