#include "stdafx.h"
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

void	GameRoom::removeClient(Client *client)
{
	for (int i = 0; i < nbClients; i++)
	{
		if (clients[i] == client)
		{
			for (i; i + 1 < nbClients; i++)
			{
				clients[i] = clients[i + 1];
			}
			nbClients--;
			return;
		}
	}
}

void GameRoom::readyClient(Client *client)
{
	for (int i = 0; i < nbClients; i++)
	{
		if (clients[i] == client)
		{
			ready[i] = true;
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
			if (started == true)
			{
				game = std::make_shared<Game>(clients);
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
