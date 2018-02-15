#ifndef SystemController_H_
# define SystemController_H_

#include <vector>
#include <iostream>
#include <memory>
#include "System.h"
#include "Protocol.h"
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

class Entity;

class SystemController : public System
{
	sf::Clock clock;
	std::vector<std::shared_ptr<Entity>>	entities;
	sf::Event event;
	std::string		name;
	bool	right;
	bool	left;
	bool	up;
	bool	down;
	bool isShooting;
public:
	SystemController();
	~SystemController();
	virtual void	exec();
	virtual const std::string	&getName() const { return name; }
	virtual void	addEntity(std::shared_ptr<Entity>);
	virtual void	removeEntity(const int&);
	virtual bool	hasEntity(const int&);
	virtual std::shared_ptr<Entity>	getEntity(const int&);
	void			MenuSelectionDown();
	void			MenuSelectionUp();
	void			MenuSelectionCount(const bool &isRight);
	void			connexionMenuEnter();
	void			connectedMenuEnter();
	void			editSelectedText();
	virtual void	clearEntities() { entities.clear(); }
	void			inGameControl();
	virtual std::vector<std::shared_ptr<Entity>>	getEntities() { return entities; }
};

#endif /*! SystemController_H_*/