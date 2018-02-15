#include "stdafx.h"
#include "System.h"
#include "Entity.h"


System::System()
{
}


System::~System()
{
}

void	System::addEntity(std::shared_ptr<Entity> entity)
{
	entities.emplace_back(entity);
}

void	System::removeEntity(const int &id)
{
	int i = 0;
	for (std::shared_ptr<Entity> ent : entities)
	{
		if (ent->getId() == id)
			break;
		i++;
	}
	if (i < entities.size())
		entities.erase(entities.begin() + i);
}

bool	System::hasEntity(const int &id)
{
	for (std::shared_ptr<Entity> ent : entities)
		if (ent->getId() == id)
			return true;
	return false;
}

std::shared_ptr<Entity>	System::getEntity(const int &id)
{
	int i = 0;
	for (std::shared_ptr<Entity> ent : entities)
	{
		if (ent->getId() == id)
			break;
		i++;
	}
	if (i < entities.size())
		return entities.at(i);
	return nullptr;
}