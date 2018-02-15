#ifndef ENTITY_H_
# define ENITY_H_

#include <unordered_map>
#include <memory>
#include "Component.h"

class Entity
{
	int	id;
	std::unordered_map<std::string, std::shared_ptr<Component>>	components;
	bool	isUsed;
public:
	Entity(int _id) : id(_id) {}
	void	addComponent(std::string const &, std::shared_ptr<Component>);
	void	removeComponent(const std::string&);
	bool	hasComponent(const std::string&);
	std::shared_ptr<Component>	&getComponent(const std::string&);
	const int	&getId() const { return id; }
	const bool			&getIsUsed() { return isUsed; }
	void	IsNotUsed() { isUsed = false; }
	void	IsUsed() { isUsed = true; }
};

#endif /*!ENTITY_H_*/