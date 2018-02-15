#pragma once
#include <unordered_map>
#include "Entity.h"
#include "Ressource.h"

class Client;

class System
{
protected:
	std::unordered_map<int, std::shared_ptr<Entity>>	entities;
	std::shared_ptr<Ressource>	res;

public:
	System();
	System(std::shared_ptr<Ressource> _res) : res(_res) {};
	virtual ~System();
	int		createMonster(const std::string &name, const int &x, const int &y, const int &ally);
	virtual int 	addEntity(const std::shared_ptr<Entity>&);
	virtual void	removeEntity(const int &id);
	virtual bool	hasEntity(const int&);
	virtual std::shared_ptr<Entity>	&getEntity(const int &id);
	virtual void	run(const int &frame, Client* _clients[4], int &nbClients, std::vector<int> &clientsId) = 0;
	int	randomInt();
};