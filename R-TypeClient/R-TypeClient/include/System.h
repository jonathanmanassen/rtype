#ifndef SYSTEM_H_
# define SYSTEM_H_

#include <vector>
#include <memory>
#include <string>

class Entity;

class System
{
	std::vector<std::shared_ptr<Entity>>	entities;
	std::string	name = "System";

public:
	System();
	~System();
	virtual void	exec() {}
	virtual void	addEntity(std::shared_ptr<Entity>);
	virtual void	removeEntity(const int&);
	virtual bool	hasEntity(const int&);
	virtual const std::string	&getName() const { return name; }
	virtual std::shared_ptr<Entity>	getEntity(const int&);
	virtual void	clearEntities() { entities.clear(); }
	virtual std::vector<std::shared_ptr<Entity>>	getEntities() { return entities; }
};

#endif /*! SYSTEM_H_*/