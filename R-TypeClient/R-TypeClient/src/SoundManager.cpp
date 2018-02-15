#include "stdafx.h"
#include "SoundManager.h"
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

//SoundManager*	SoundManager soundManager = nullptr;

SoundManager::SoundManager()
{
}