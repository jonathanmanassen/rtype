#pragma once
#include "System.h"
#include "PhysicComponent.h"
#include "Protocol.h"

class Client;

class PhysicSystem : public System
{
public:
	PhysicSystem();
	PhysicSystem(std::shared_ptr<Ressource> res) : System(res) {}
	virtual ~PhysicSystem();
	virtual void run(const int &frame, Client* _clients[4], int &nbClients, std::vector<int> &clientsId);
	void	clientRun(std::shared_ptr<PhysicComponent> physic, Client *_clients[4], int clientPos);

	int	clientsIdPos(const std::vector<int> &vec, const int &id);

	void getEntitiesInfo(std::vector<ServerInGameInfo> &list);

	void collision(const int &id, const std::shared_ptr<PhysicComponent> &physic);
	void removeDeadMonsters(int &_nbClients, std::vector<int> &clientsId);
};

