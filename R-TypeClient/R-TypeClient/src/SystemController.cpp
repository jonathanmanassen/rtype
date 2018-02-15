#include "stdafx.h"
#include "SystemController.h"
#include "Entity.h"
#include "Component.h"
#include "PropertyComponent.h"
#include "AnimationComponent.h"
#include "Ressources.h"
#include "Network.h"
#include "SelectionComponent.h"
#include "TextComponent.h"
#include "CountComponent.h"
#include "NotificationComponent.h"
#include <memory>

extern GameStat gameStat;
extern sf::RenderWindow window;

SystemController::SystemController()
{
	name = "Controller";
}

SystemController::~SystemController()
{
}

void	SystemController::MenuSelectionDown()
{
	if (entities.size() == 0 && gameStat == GameStat::IN_GAME)
		return;
	std::shared_ptr<Entity> prevEnt;
	int idx = 0;
	for (int i = 0; i < entities.size(); i++)
	{
		idx = i;
		if (entities.at(i)->hasComponent("Selection") == true)
			break;
	}
	prevEnt = entities.at(idx);
	bool nextSelect = false;
	idx = 0;
	for (std::shared_ptr<Entity> entity : entities)
	{
		if (entity->hasComponent("Selection"))
		{
			std::shared_ptr<SelectionComponent> select = std::static_pointer_cast<SelectionComponent>(entity->getComponent("Selection"));
			if (nextSelect == true)
			{
				nextSelect = false;
				select->changeSelection();
				break;
			}
			if (select->getIsSelected() == true)
			{
				nextSelect = true;
				select->changeSelection();
			}
		}
		idx++;
	}
	if (nextSelect == true && prevEnt->hasComponent("Selection") == true)
		std::static_pointer_cast<SelectionComponent>(prevEnt->getComponent("Selection"))->changeSelection();
}

void	SystemController::MenuSelectionUp()
{
	if (entities.size() == 0 && gameStat == GameStat::IN_GAME)
		return;
	std::shared_ptr<Entity> prevEnt;
	int idx = 0;
	for (int i = 0; i < entities.size(); i++)
		if (entities.at(i)->hasComponent("Selection"))
			idx = i;
	prevEnt = entities.at(idx);
	for (std::shared_ptr<Entity> entity : entities)
	{
		if (entity->hasComponent("Selection"))
		{
			std::shared_ptr<SelectionComponent> select = std::static_pointer_cast<SelectionComponent>(entity->getComponent("Selection"));
			if (select->getIsSelected() == true)
			{
				select->changeSelection();
				break;
			}
			prevEnt = entity;
		}
	}
	if (prevEnt->hasComponent("Selection"))
	{
		std::shared_ptr<SelectionComponent> select = std::static_pointer_cast<SelectionComponent>(prevEnt->getComponent("Selection"));
		select->changeSelection();
	}
}

void	SystemController::MenuSelectionCount(const bool &isRight)
{
	if (entities.size() == 0 && gameStat == GameStat::IN_GAME)
		return;
	for (std::shared_ptr<Entity> entity : entities)
	{
		if (entity->hasComponent("Selection"))
		{
			std::shared_ptr<SelectionComponent> select = std::static_pointer_cast<SelectionComponent>(entity->getComponent("Selection"));
			if (select->getIsSelected() == true)
			{
				if (entity->hasComponent("Count"))
				{
					if (isRight == true)
						std::static_pointer_cast<CountComponent>(entity->getComponent("Count"))->nextContent();
					else
						std::static_pointer_cast<CountComponent>(entity->getComponent("Count"))->prevContent();
				}
				break;
			}
		}
	}
}

void	SystemController::connexionMenuEnter()
{
	if (entities.size() == 0 && gameStat == GameStat::IN_GAME)
		return;
	std::string name;
	std::string ip;
	int port;
	for (std::shared_ptr<Entity> entity : entities)
	{
		if (entity->hasComponent("TitleText"))
		{
			std::string title = std::static_pointer_cast<TextComponent>(entity->getComponent("TitleText"))->getContent();
			if (title.compare("Name") == 0 && entity->hasComponent("EditText"))
				name = std::static_pointer_cast<TextComponent>(entity->getComponent("EditText"))->getContent();
			else if (title.compare("Ip") == 0 && entity->hasComponent("EditText"))
				ip = std::static_pointer_cast<TextComponent>(entity->getComponent("EditText"))->getContent();
			else if (title.compare("Port") == 0 && entity->hasComponent("EditText"))
				port = std::atoi(std::static_pointer_cast<TextComponent>(entity->getComponent("EditText"))->getContent().c_str());
		}
	}
	std::cout << name << ":" << ip << ":" << port << std::endl;
	if (Network::get()->initTcpNetwork(ip, port))
		Network::get()->sendTcpMessage(ConnectionAction::HELLO, name, 0, "");
	else
		std::cout << "cant't connect" << std::endl;
	for (std::shared_ptr<Entity> entity : entities)
		if (entity->hasComponent("Notification"))
			std::static_pointer_cast<NotificationComponent>(entity->getComponent("Notification"))->newNotification(Network::get()->getLastAction());
}

void	SystemController::connectedMenuEnter()
{
	if (entities.size() == 0 && gameStat == GameStat::IN_GAME)
		return;
	for (std::shared_ptr<Entity> entity : entities)
	{
		if (entity->hasComponent("Selection") &&
			std::static_pointer_cast<SelectionComponent>(entity->getComponent("Selection"))->getIsSelected() == true)
		{
			if (entity->hasComponent("Count"))
				Network::get()->sendTcpMessage(ConnectionAction::CREATE_ROOM, "", 0, std::static_pointer_cast<CountComponent>(entity->getComponent("Count"))->getContent().c_str());
			else if (entity->hasComponent("Property") && std::static_pointer_cast<PropertyComponent>(entity->getComponent("Property"))->getType().compare("Bouton") == 0)
				Network::get()->sendTcpMessage(ConnectionAction::ENTER_ROOM, "", std::static_pointer_cast<PropertyComponent>(entity->getComponent("Property"))->getRoomNb(), "");
		}
	}
}

void	SystemController::editSelectedText()
{
	if (entities.size() == 0 && gameStat == GameStat::IN_GAME)
		return;
	if (event.text.unicode == 8)
		for (std::shared_ptr<Entity> entity : entities)
		{
			if (entity->hasComponent("Selection"))
			{
				std::shared_ptr<SelectionComponent> select = std::static_pointer_cast<SelectionComponent>(entity->getComponent("Selection"));
				if (select->getIsSelected() == true && entity->hasComponent("EditText"))
				{
					std::static_pointer_cast<TextComponent>(entity->getComponent("EditText"))->delLetter();
					break;
				}
			}
		}
	if (event.text.unicode > 32 && event.text.unicode < 127)
		for (std::shared_ptr<Entity> entity : entities)
		{
			if (entity->hasComponent("Selection"))
			{
				std::shared_ptr<SelectionComponent> select = std::static_pointer_cast<SelectionComponent>(entity->getComponent("Selection"));
				if (select->getIsSelected() == true)
				{
					if (entity->hasComponent("EditText"))
					{
						std::shared_ptr<TextComponent> text = std::static_pointer_cast<TextComponent>(entity->getComponent("EditText"));
						text->addLetter(static_cast<char>(event.text.unicode));
					}
				}
			}
		}
}

void	SystemController::inGameControl()
{
	Network::get()->sendControllerMessage(right, left, up, down, isShooting);
}

void	SystemController::exec()
{
	if (gameStat == GameStat::IN_GAME)
	{
		window.pollEvent(event);
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			gameStat = GameStat::GAME_END;
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Space)
				isShooting = true;
			if (event.key.code == sf::Keyboard::Down)
				down = true;
			if (event.key.code == sf::Keyboard::Up)
				up = true;
			if (event.key.code == sf::Keyboard::Right)
				right = true;
			if (event.key.code == sf::Keyboard::Left)
				left = true;
			if (event.key.code == sf::Keyboard::Escape)
			{
				Network::get()->sendTcpMessage(ConnectionAction::LEAVE_ROOM, "", 0, "");
				gameStat = GameStat::CONNECTED;
				return;
			}
			break;
		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::Space)
				isShooting = false;
			if (event.key.code == sf::Keyboard::Down)
				down = false;
			if (event.key.code == sf::Keyboard::Up)
				up = false;
			if (event.key.code == sf::Keyboard::Right)
				right = false;
			if (event.key.code == sf::Keyboard::Left)
				left = false;
			break;
		}
		inGameControl();
	}
	else if (window.pollEvent(event))
	{
		isShooting = false;
		down = false;
		up = false;
		right = false;
		left = false;
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			gameStat = GameStat::GAME_END;
			break;
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::Tab:
			case sf::Keyboard::Down:
				MenuSelectionDown();
				break;
			case sf::Keyboard::Up:
				MenuSelectionUp();
				break;
			case sf::Keyboard::Right:
				MenuSelectionCount(true);
				break;
			case sf::Keyboard::Left:
				MenuSelectionCount(false);
				break;
			case sf::Keyboard::Return:
				switch (gameStat)
				{
				case GameStat::CONNEXION:
					connexionMenuEnter();
					break;
				case GameStat::CONNECTED:
					connectedMenuEnter();
					break;
				case GameStat::IN_ROOM:
					Network::get()->sendTcpMessage(ConnectionAction::READY, "", 0, "");
					break;
				}
				break;
			case sf::Keyboard::Escape:
				switch (gameStat)
				{
				case GameStat::CONNECTED:
					Network::get()->disconnectTcpFromServer();
					gameStat = GameStat::CONNEXION;
					break;
				case GameStat::IN_ROOM:
					Network::get()->sendTcpMessage(ConnectionAction::LEAVE_ROOM, "", 0, "");
					Network::get()->startWait();
					break;
				}
			}
			break;
		case sf::Event::TextEntered:
			editSelectedText();
			break;
		}
	}
}

void	SystemController::addEntity(std::shared_ptr<Entity> entity)
{
	entities.emplace_back(entity);
}

void	SystemController::removeEntity(const int &id)
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

bool	SystemController::hasEntity(const int &id)
{
	for (std::shared_ptr<Entity> ent : entities)
		if (ent->getId() == id)
			return true;
	return false;
}

std::shared_ptr<Entity>	SystemController::getEntity(const int &id)
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