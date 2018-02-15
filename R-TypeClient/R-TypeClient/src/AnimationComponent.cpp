#include <iostream>
#include "stdafx.h"
#include "AnimationComponent.h"
#include "Ressources.h"

void				AnimationComponent::nextFrame()
{
	animStack++;
	if (currentFrame == nbSprite && loopMode == 0)
		currentFrame = 0;
	else if (currentFrame < nbSprite && animStack >= animSpeed)
	{
		animStack = 0;
		currentFrame++;
	}
}

void				AnimationComponent::configure(ServerInGameInfo const &info)
{
	animStat = convertAnimTypeToString(info.type);
	name = "anim" + animStat;
	type = info.entityType;
 	loopMode = info.loopMode;
	if (loopMode < 0)
		loopMode = 0;
	startFrame = 0;
	nbSprite = Ressources::get()->getAnimSize(info.entityType, convertAnimTypeToString(info.type));
	sf::FloatRect tmp = Ressources::get()->getAnimSprite(info.entityType, convertAnimTypeToString(info.type), 0).getGlobalBounds();
	size.x = tmp.width;
	size.y = tmp.height;
//	std::cout << name << ":nbSprite:" << nbSprite << ":LoopMode:" << loopMode << std::endl;
}

void				AnimationComponent::setPos(const subPosition &p)
{
	pos = p;
	name = "sub" + name;
}

void	AnimationComponent::testSprite()
{
/*	sf::RenderWindow win;
	win.create(sf::VideoMode(1080, 960), "R-Type Client");
	animSprites.at(0).setPosition(15, 15);
	win.draw(animSprites.at(0));
	win.display();
	while (1);*/
}
