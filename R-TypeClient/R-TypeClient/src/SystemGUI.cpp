#include "stdafx.h"
#include "SystemGUI.h"
#include "Entity.h"
#include "Component.h"
#include "PropertyComponent.h"
#include "AnimationComponent.h"
#include "TextComponent.h"
#include "CountComponent.h"
#include "SelectionComponent.h"
#include "Ressources.h"
#include "ScrollComponent.h"
#include "NotificationComponent.h"
#include <memory>

extern GameStat gameStat;
sf::RenderWindow window;

SystemGUI::SystemGUI()
{
	name = "GUI";
	window.create(sf::VideoMode(1920, 1080), "R-Type Client");
	Ressources::get()->setWinSize(window.getSize().x, window.getSize().y);
	window.setFramerateLimit(60);
	if (!font.loadFromFile("Friz Quadrata Std Medium.otf"))
	{
		std::cout << "Can't load font" << std::endl;
		throw std::runtime_error("Error can't find font file");
	}
}

void	SystemGUI::checkTextComponent(std::shared_ptr<Entity> entity, const std::string &name, const int &size, const sf::Color &col)
{
	if (entity->hasComponent(name))
	{
		std::shared_ptr<TextComponent> textComp = std::static_pointer_cast<TextComponent>(entity->getComponent(name));
		sf::Text text;
		text.setFont(font);
		text.setString(textComp->getContent());
		text.setCharacterSize(size);
		text.setFillColor(col);
		text.setStyle(sf::Text::Bold);
		if (name.compare("NameText") != 0)
		{
			text.setOrigin(sf::Vector2f(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2));
			text.setPosition(sf::Vector2f(textComp->getPos().x, textComp->getPos().y));
		}
		else
			if (entity->hasComponent("Property"))
			{
				std::shared_ptr<PropertyComponent> prop = std::static_pointer_cast<PropertyComponent>(entity->getComponent("Property"));
				std::shared_ptr<AnimationComponent> anim = std::static_pointer_cast<AnimationComponent>(entity->getComponent("anim" + prop->getAnimStat()));
				sf::Sprite tmp = Ressources::get()->getAnimSprite(prop->getType(), prop->getAnimStat(), anim->getCurrentFrame());
				text.setPosition(sf::Vector2f(prop->getEntityPos().x - (tmp.getGlobalBounds().width / 2), prop->getEntityPos().y - (tmp.getGlobalBounds().height + (tmp.getGlobalBounds().height / 4))));
			}
		//std::cout << textComp->getPos().x << ":" << textComp->getPos().y << std::endl;
		window.draw(text);
	}
}

void	SystemGUI::exec()
{
	window.clear();
	Ressources::get()->setWinSize(window.getSize().x, window.getSize().y);
	sf::Time frameTime = clock.restart();
	double framerate = 1 / (frameTime.asMilliseconds() * 0.001);
	//	std::cout << "FPS:" << framerate << std::endl;
	window.setTitle("RType:" + std::to_string(framerate));
	for (std::shared_ptr<Entity> entity : entities)
	{
		if (entity->hasComponent("Property"))
		{
			std::shared_ptr<PropertyComponent> prop = std::static_pointer_cast<PropertyComponent>(entity->getComponent("Property"));
			if (entity->hasComponent("anim" + prop->getAnimStat()))
			{
				std::shared_ptr<AnimationComponent> anim = std::static_pointer_cast<AnimationComponent>(entity->getComponent("anim" + prop->getAnimStat()));
				sf::Sprite tmp = Ressources::get()->getAnimSprite(prop->getType(), prop->getAnimStat(), anim->getCurrentFrame());
				tmp.setOrigin(sf::Vector2f(tmp.getLocalBounds().width / 2, tmp.getLocalBounds().height / 2));
				tmp.setPosition(sf::Vector2f(prop->getEntityPos().x, prop->getEntityPos().y));
				if (entity->hasComponent("Selection") == true &&
					std::static_pointer_cast<SelectionComponent>(entity->getComponent("Selection"))->getIsSelected() == true)
					tmp.setColor(sf::Color::Cyan);
				if (entity->hasComponent("Scroll"))
				{
					std::shared_ptr<ScrollComponent> scroll = std::static_pointer_cast<ScrollComponent>(entity->getComponent("Scroll"));
					tmp.rotate(scroll->getScrollRotationRatio());
					tmp.setPosition(sf::Vector2f(prop->getEntityPos().x + scroll->getFirstScrollInc(), prop->getEntityPos().y));
					window.draw(tmp);
					tmp.rotate(scroll->getScrollRotationRatio());
					tmp.setPosition(sf::Vector2f(prop->getEntityPos().x + scroll->getSecondScrollInc(), prop->getEntityPos().y));
					scroll->incrementScroll();
					scroll->incrementRotation();
				}
				if (entity->hasComponent("Notification") == true)
				{
					if (gameStat != GameStat::IN_GAME)
					{
						std::shared_ptr<NotificationComponent> not = std::static_pointer_cast<NotificationComponent>(entity->getComponent("Notification"));
						tmp.setOrigin(0, 0);
						tmp.setPosition(sf::Vector2f(prop->getEntityPos().x + not->getNotificationInc(), prop->getEntityPos().y));
						not->incrementNotification();
						sf::Text text;
						text.setFont(font);
						text.setString(not->getContent());
						if (not->getContent().size() > 20)
							text.setCharacterSize(30);
						else
							text.setCharacterSize(50);
						text.setFillColor(sf::Color::Red);
						text.setStyle(sf::Text::Bold);
						text.setOrigin(sf::Vector2f(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2));
						text.setPosition(sf::Vector2f(prop->getEntityPos().x + not->getNotificationInc() + (tmp.getGlobalBounds().width / 2), prop->getEntityPos().y + (tmp.getGlobalBounds().height / 2)));
						window.draw(tmp);
						window.draw(text);
					}
				}
				else
					window.draw(tmp);
				/*				ite++;
				if (ite == window.getSize().x - 15)
				ite = 0;*/
				anim->nextFrame();
			}
			if (prop->getIsSubAnim() == true)
				for (const std::string &name : prop->getSubAnim())
					if (entity->hasComponent(name))
					{
						std::shared_ptr<AnimationComponent> anim = std::static_pointer_cast<AnimationComponent>(entity->getComponent(name));
						sf::Sprite tmp = Ressources::get()->getAnimSprite(anim->getType(), anim->getAnimStat(), anim->getCurrentFrame());
						sf::Vector2f	pos(0,0);
						if (entity->hasComponent("anim" + prop->getAnimStat()))
						{
							std::shared_ptr<AnimationComponent> mainAnim = std::static_pointer_cast<AnimationComponent>(entity->getComponent("anim" + prop->getAnimStat()));
							sf::FloatRect	size = Ressources::get()->getAnimSprite(mainAnim->getType(), mainAnim->getAnimStat(), mainAnim->getCurrentFrame()).getGlobalBounds();
							switch (anim->getPos())
							{
							case subPosition::LEFT:
								pos.x = prop->getEntityPos().x - (size.width / 2) + (size.width / 4);
								pos.y = prop->getEntityPos().y;
								break;
							case subPosition::RIGHT:
								pos.x = prop->getEntityPos().x + (size.width / 2) - (size.width / 4);
								pos.y = prop->getEntityPos().y;
								break;
							case subPosition::MIDDLE:
								pos.x = prop->getEntityPos().x;
								pos.y = prop->getEntityPos().y;
								break;
							case subPosition::TOP:
								pos.x = prop->getEntityPos().x;
								pos.y = prop->getEntityPos().y - (size.height / 2) + (size.height / 4);
								break;
							case subPosition::BOTTOM:
								pos.x = prop->getEntityPos().x;
								pos.y = prop->getEntityPos().y + (size.height / 2) - (size.height / 4);
								break;
							}
						}
						tmp.setOrigin(sf::Vector2f(tmp.getLocalBounds().width / 2, tmp.getLocalBounds().height / 2));
						tmp.setPosition(pos);
						if (name.compare("BoutonReady") == 0)
						{
							if (prop->getIsReady() == true && prop->getIsLoaded() == true)
								tmp.setColor(sf::Color::Yellow);
							else if (prop->getIsReady() == true)
								tmp.setColor(sf::Color::Green);
							else
								tmp.setColor(sf::Color::Red);
						}
						window.draw(tmp);
						anim->nextFrame();
					}

			checkTextComponent(entity, "TitleText", 24, sf::Color::Red);
			checkTextComponent(entity, "EditText", 50, sf::Color::Blue);
			checkTextComponent(entity, "NameText", 30, sf::Color::White);
			if (entity->hasComponent("Count"))
			{
				std::shared_ptr<CountComponent> countComp = std::static_pointer_cast<CountComponent>(entity->getComponent("Count"));
				sf::Text text;
				text.setFont(font);
				text.setString(countComp->getContent());
				text.setCharacterSize(50);
				text.setFillColor(sf::Color::Blue);
				text.setStyle(sf::Text::Bold);
				text.setOrigin(sf::Vector2f(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2));
				text.setPosition(sf::Vector2f(countComp->getPos().x, countComp->getPos().y));
				window.draw(text);
			}
		}
		entity->IsNotUsed();
	}
	window.display();
}

void	SystemGUI::addEntity(std::shared_ptr<Entity> entity)
{
	entities.emplace_back(entity);
}

void	SystemGUI::removeEntity(const int &id)
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

bool	SystemGUI::hasEntity(const int &id)
{
	for (std::shared_ptr<Entity> ent : entities)
		if (ent->getId() == id)
			return true;
	return false;
}

std::shared_ptr<Entity>	SystemGUI::getEntity(const int &id)
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