#include "stdafx.h"
#include "PropertyComponent.h"
#include "Protocol.h"
#include "Ressources.h"

PropertyComponent::PropertyComponent()
{
	this->name = "Property";
}


PropertyComponent::~PropertyComponent()
{
}

void	PropertyComponent::configure(ServerInGameInfo const &info)
{
	type = info.entityType;
	this->animStat = convertAnimTypeToString(info.type);
	this->pos.x = static_cast<float>(info.posX);
	this->pos.y = static_cast<float>(info.posY);
	sf::FloatRect tmp = Ressources::get()->getAnimSprite(info.entityType, convertAnimTypeToString(info.type), 0).getGlobalBounds();
	size.x = tmp.width;
	size.y = tmp.height;
	lastAction = currentAction;
	currentAction = info.type;
}