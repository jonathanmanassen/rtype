#include "stdafx.h"
#include "System.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <functional>
#include <iostream>
#include "PhysicComponent.h"

System::System()
{
}


System::~System()
{
}

int 	System::createMonster(const std::string &name, const int &x, const int &y, const int &ally = -1)
{
	IMonster *dll = res->getMonster(name);
	std::shared_ptr<Entity>			monster = std::make_shared<Entity>(name);

	int id = addEntity(monster);
	std::shared_ptr<PhysicComponent>		monsterPhysic = std::make_shared<PhysicComponent>("physic", "physic");
	monster->addComponent(monsterPhysic->name, monsterPhysic);

	monsterPhysic->hitboxes = dll->hitboxes;
	monsterPhysic->hp = dll->hp;
	monsterPhysic->shootDelay = 0;
	monsterPhysic->ally = dll->ally;
	if (ally != -1)
		monsterPhysic->ally = ally;

	monsterPhysic->pos.change(x, y);
	monsterPhysic->random = randomInt();
	return (id);
}

int	System::randomInt()
{
	std::default_random_engine generator(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<int> distribution(1000000, 9999999);
	auto dice = std::bind(distribution, generator);

	return dice();
}

int		System::addEntity(const std::shared_ptr<Entity> &obj)
{
	int	id = randomInt();
	entities.emplace(id, obj);

	return (id);
}

void	System::removeEntity(const int &id)
{
	entities.erase(id);
}

bool	System::hasEntity(const int &id)
{
	std::unordered_map<int, std::shared_ptr<Entity>>::iterator	it = entities.find(id);

	if (it == entities.end())
		return (false);
	return (true);
}

std::shared_ptr<Entity>	&System::getEntity(const int &id)
{
	return (entities[id]);
}