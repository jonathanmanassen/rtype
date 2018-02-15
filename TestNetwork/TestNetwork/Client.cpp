#include "stdafx.h"
#include "GameRoomManager.h"
#include "Client.h"

Client::Client(ATCPSocket *socket, const int &id) : socket(socket), id(id)
{
	std::cout << "client created" << std::endl;
	t = std::thread(&Client::read, this);
	func.emplace(ConnectionAction::HELLO, &Client::hello);
	func.emplace(ConnectionAction::CREATE_ROOM, &Client::createRoom);
	func.emplace(ConnectionAction::ENTER_ROOM, &Client::enterRoom);
	func.emplace(ConnectionAction::LEAVE_ROOM, &Client::leaveRoom);
	func.emplace(ConnectionAction::READY, &Client::ready);
	func.emplace(ConnectionAction::LOADED, &Client::loaded);
}

void Client::read()
{
	while (true)
	{
		try
		{
			socket->recv(this, reinterpret_cast<char*>(&message), sizeof(ConnectionMessage));
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			socket->destroy(this);
			return;
		}
		if (message.act >= ConnectionAction::HELLO && message.act <= ConnectionAction::LOADED)
			(this->*func[message.act])();
	}
}

void Client::hello()
{
	int i = 0;
	while (i < 30 && message.name[i])
		name[i] = message.name[i++];
	name[i] = '\0';
	sendAllRooms();
}

void Client::sendAllRooms()
{
	std::vector<ServerRoomsInfo> rooms;
	for (auto& tmp : GameRoomManager::get()->gameRooms)
	{
		if (tmp->getStarted() == true || tmp->getNbClients() == 4)
			continue;
		ServerRoomsInfo	room;
		room.roomNb = tmp->getId();
		room.nbPlayer = tmp->getNbClients();
		room.lvlNb = tmp->getLevel();
		for (int j = 0; j < 30; j++)
			room.ownerName[j] = tmp->clients[0]->getName()[j];
		rooms.emplace_back(room);
	}
	socket->send(this, "RO", sizeof(char) * 3);
	int val = rooms.size();
	socket->send(this, reinterpret_cast<char*>(&val), sizeof(int));
	if (!rooms.empty())
	{
		socket->send(this, reinterpret_cast<char*>(&rooms[0]), val * sizeof(ServerRoomsInfo));
	}
}

void Client::createRoom()
{
	GameRoomManager::get()->addGameRoom(this);
}

void Client::enterRoom()
{
	for (auto& tmp : GameRoomManager::get()->gameRooms)
	{
		if (tmp->getId() == message.roomNb)
		{
			ServerInRoomInfo	info;
			if (tmp->addClient(this))
			{
				tmp->updateAll();
			}
		}
	}
}

void	Client::leaveRoom()
{
	for (auto &tmp : GameRoomManager::get()->gameRooms)
	{
		if (tmp->getId() == roomId)
		{
			tmp->removeClient(this);
			roomId = -1;
			break;
		}
	}
	sendAllRooms();
}

void	Client::update(const GameRoom &room)
{
	ServerInRoomInfo	info;

	info.playerNb = room.getNbClients();
	for (int i = 0; i < 4 && i < room.getNbClients(); i++)
	{
		for (int j = 0; j < 29 && room.clients[i]->getName()[j]; j++)
		{
			info.playersName[i][j] = room.clients[i]->getName()[j];
			info.playersName[i][j + 1] = '\0';
		}
		info.ready[i] = room.getReady(i);
		info.loaded[i] = room.getLoaded(i);
		std::cout << info.playersName[i] << std::endl;
	}
	info.startGame = room.getStarted();
	socket->send(this, "IN", sizeof(char) * 3);
	socket->send(this, reinterpret_cast<char*>(&info), sizeof(ServerInRoomInfo));

}

void	Client::ready()
{
	for (auto &tmp : GameRoomManager::get()->gameRooms)
	{
		if (tmp->getId() == roomId)
		{
			tmp->readyClient(this);
			break;
		}
	}
}

void Client::loaded()
{
	for (auto &tmp : GameRoomManager::get()->gameRooms)
	{
		if (tmp->getId() == roomId)
		{
			tmp->loadClient(this);
			break;
		}
	}
}
