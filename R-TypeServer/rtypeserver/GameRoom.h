#pragma once
#include "Client.h"
#include "Game.h"

class Game;
class Client;

class GameRoom
{
	const int id = 0;
	int	nbClients = 1;
	int	level = 0;
	bool	ready[4] = { false, false, false, false };
	bool	loaded[4] = { false, false, false, false };
	bool	started = false;
	bool	loading = false;
	bool	gameStarted = false;
	std::shared_ptr<Game>	game;
	std::thread				gameThread;
public:
	Client	*clients[4];
public:
	GameRoom() = default;
	GameRoom(Client *client, const int &level, const int &id);
	bool	addClient(Client *client);
	int		removeClient(Client* client);
	void	readyClient(Client *client);
	void	loadClient(Client *client);
	void	updateAll() const;
	void	sendRessourcesToAll() const;
	void stop();

	const int &getId() const { return id; }
	const int &getNbClients() const { return nbClients; }
	const int &getLevel() const { return level; }
	const bool	&getStarted() const { return started; }
	const bool	&getReady(const int &i) const { return ready[i]; }
	const bool	&getLoaded(const int &i) const { return loaded[i]; }
};
