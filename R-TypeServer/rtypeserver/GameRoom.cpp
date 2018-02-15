#include "stdafx.h"
#include <exception>
#include "GameRoom.h"
#include "Client.h"


GameRoom::GameRoom(Client *client, const int& level, const int &id) : level(level), id(id)
{
	clients[0] = client;
	client->setRoomId(id);
}

bool	GameRoom::addClient(Client *client)
{
	if (nbClients < 4)
	{
		client->setRoomId(id);
		clients[nbClients] = client;
		nbClients++;
		return (true);
	}
	return (false);
}

int GameRoom::removeClient(Client* client)
{
	for (int i = 0; i < nbClients; i++)
	{
		if (clients[i] == client)
		{
			for (i; i + 1 < nbClients; i++)
			{
				clients[i] = clients[i + 1];
				ready[i] = ready[i + 1];
				loaded[i] = loaded[i + 1];
			}
			ready[i] = false;
			loaded[i] = false;
			nbClients--;
			if (gameStarted && game && gameThread.joinable())
			{
				game->update(i);
				if (nbClients == 0)
					gameThread.detach();
			}
			return (nbClients);
		}
	}
	return (nbClients);
}

void GameRoom::readyClient(Client *client)
{
	for (int i = 0; i < nbClients; i++)
	{
		if (clients[i] == client)
		{
			if (ready[i])
			{
				ready[i] = false;
				if (loading)
				{
					loading = false;
					game.reset();
				}
			}
			else
			{
				ready[i] = true;
				bool start = true;
				for (int j = 0; j < nbClients; j++)
					if (!ready[j])
						start = false;
				loading = start;
				if (loading)
				{
					game = std::make_shared<Game>(level, clients, nbClients);
					sendRessourcesToAll();
				}
			}
			updateAll();
		}
	}
}

void GameRoom::loadClient(Client *client)
{
	for (int i = 0; i < nbClients; i++)
	{
		if (clients[i] == client)
		{
			loaded[i] = true;
			bool start = true;
			for (int j = 0; j < nbClients; j++)
				if (!loaded[j])
					start = false;
			started = start;
			updateAll();
			if (started && !gameStarted)
			{
				gameStarted = true;
				gameThread = std::thread(&Game::start, game);
			}
		}
	}
}

void	GameRoom::updateAll() const
{
	for (int i = 0; i < nbClients; i++)
	{
		clients[i]->update(*this);
	}
}

void	GameRoom::sendRessourcesToAll() const
{
	std::vector<ServerLoadRessourcesInfo> list;
	game->res->getRessourcesInfo(list);
	uint32_t size = static_cast<uint32_t>(list.size());

	for (int i = 0; i < nbClients; i++)
	{
		clients[i]->socket->send(clients[i], "RE", sizeof(char) * 3);
		clients[i]->socket->send(clients[i], reinterpret_cast<char*>(&size), sizeof(uint32_t));
		if (size > 0)
			clients[i]->socket->send(clients[i], reinterpret_cast<char*>(&list[0]), size * sizeof(ServerLoadRessourcesInfo));
	}
}

void	GameRoom::stop()
{
	if (gameStarted && gameThread.joinable())
	{
		gameThread.detach();
	}
}