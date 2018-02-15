#include "stdafx.h"
#include "Entity.h"
#include <iostream>


Entity::Entity(std::string _name) : name(_name)
{

}


Entity::~Entity()
{
}

void	Entity::addComponent(std::string name, std::shared_ptr<Component> obj)
{
	components.emplace(name, obj);
}

void	Entity::removeComponent(const std::string &name)
{
	components.erase(name);
}

bool	Entity::hasComponent(const std::string &name)
{
	std::unordered_map<std::string, std::shared_ptr<Component>>::iterator	it = components.find(name);

	if (it == components.end())
		return (0);
	return (1);
}

std::shared_ptr<Component>	Entity::getComponent(const std::string &name) 
{
	return (components[name]);
}
