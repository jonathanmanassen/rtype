#pragma once
#include <unordered_map>
#include "Component.h"
#include <memory>

class Entity
{
protected:
	std::string	name;

public:
	std::unordered_map<std::string, std::shared_ptr<Component>>	components;
	Entity() {}
	Entity(std::string _name);
	~Entity();
	void	addComponent(std::string, std::shared_ptr<Component>);
	void	removeComponent(const std::string&);
	bool	hasComponent(const std::string&);
	std::shared_ptr<Component>	getComponent(const std::string&);
	const std::string	&getName() const { return name; }
};