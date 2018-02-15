#ifndef SystemGUI_H_
# define SystemGUI_H_

#include <vector>
#include <iostream>
#include <memory>
#include "System.h"
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

class Entity;

class SystemGUI : public System
{
	int ite = 0;
	sf::Clock clock;
	std::vector<std::shared_ptr<Entity>>	entities;
	sf::Font font;
	std::string name;
public:
	SystemGUI();
	virtual void	exec();
	virtual void	addEntity(std::shared_ptr<Entity>);
	virtual const std::string	&getName() const {	return name; }
	virtual void	removeEntity(const int&);
	virtual bool	hasEntity(const int&);
	virtual std::shared_ptr<Entity>	getEntity(const int&);
	virtual void	clearEntities() { entities.clear(); }
	void			checkTextComponent(std::shared_ptr<Entity> entity, const std::string &name, const int &size, const sf::Color &col);
	virtual std::vector<std::shared_ptr<Entity>>	getEntities() { return entities; }
};

#endif /*! SystemGUI_H_*/