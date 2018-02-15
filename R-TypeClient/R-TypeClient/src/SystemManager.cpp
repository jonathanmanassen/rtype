#include "stdafx.h"
#include "SystemManager.h"
#include "Entity.h"
#include "Network.h"
#include "System.h"
#include "SystemGUI.h"
#include "SystemController.h"
#include "AnimationComponent.h"
#include "PropertyComponent.h"
#include "TextComponent.h"
#include "CountComponent.h"
#include "SelectionComponent.h"
#include "NotificationComponent.h"
#include <mutex>
#include "ScrollComponent.h"

extern GameStat gameStat;
std::mutex mtx;

SystemManager::SystemManager()
{
}


SystemManager::~SystemManager()
{
}

void	SystemManager::initBaseSystems()
{
	systems.push_back(std::make_shared<SystemGUI>());
	systems.push_back(std::make_shared<SystemController>());
}

void	SystemManager::settingUpConnexionMenuEntity(std::shared_ptr<Entity> ent, std::shared_ptr<PropertyComponent>	propComp, const int &idx)
{
	if (propComp->getType().compare("Bouton") == 0)
	{
		std::string text = "";
		std::string edit = "";
		switch (idx)
		{
		case 0:
			text = "Name";
			edit = "Bellay";
			break;
		case 1:
			text = "Ip";
			edit = Network::get()->getAddress();
			break;
		case 2:
			text = "Port";
			edit = std::to_string(Network::get()->getPort());
			break;
		}
		std::shared_ptr<TextComponent> textComp = std::make_shared<TextComponent>("TitleText", false);
		textComp->configure(text, propComp->getEntityPos().x, propComp->getEntityPos().y - (propComp->getSize().y / 2));
		std::cout << textComp->getContent() << std::endl;
		ent->addComponent(textComp->getName(), textComp);

		std::shared_ptr<TextComponent> textEditComp = std::make_shared<TextComponent>("EditText", true);
		textEditComp->configure(edit, propComp->getEntityPos().x, propComp->getEntityPos().y);
		ent->addComponent(textEditComp->getName(), textEditComp);

		std::shared_ptr<SelectionComponent> selectComp = std::make_shared<SelectionComponent>();
		if (idx == 0)
			selectComp->changeSelection();
		ent->addComponent(selectComp->getName(), selectComp);
	}
}

void	SystemManager::settingUpConnectedMenuEntity(std::shared_ptr<Entity> ent, std::shared_ptr<PropertyComponent>	propComp)
{
	if (propComp->getType().compare("Bouton") == 0)
	{
		std::string text = "Create Room";
		std::shared_ptr<TextComponent> textComp = std::make_shared<TextComponent>("TitleText", false);
		textComp->configure(text, propComp->getEntityPos().x, propComp->getEntityPos().y - (propComp->getSize().y / 2));
		std::cout << textComp->getContent() << std::endl;
		ent->addComponent(textComp->getName(), textComp);

		std::shared_ptr<CountComponent> countComp = std::make_shared<CountComponent>("Count", true);
		std::vector<std::string> vec;
		vec.push_back("Level 1");
		vec.push_back("Level 2");
		vec.push_back("Level 3");
		countComp->configure(vec, propComp->getEntityPos().x, propComp->getEntityPos().y);
		ent->addComponent(countComp->getName(), countComp);

		std::shared_ptr<SelectionComponent> selectComp = std::make_shared<SelectionComponent>();
		selectComp->changeSelection();
		ent->addComponent(selectComp->getName(), selectComp);
	}
}

void	SystemManager::menuInitialisation()
{
	std::vector<ServerInGameInfo> baseEntities;
	int idx = 0;
	clearEntities();
	tmpEntities.clear();
	switch (gameStat)
	{
	case GameStat::CONNEXION:
		baseEntities = initBaseConnexionEntities();
		break;
	case GameStat::CONNECTED:
		baseEntities = initBaseConnectedEntities();
		break;
	case GameStat::IN_ROOM:
	case GameStat::IN_GAME:
		return;
		break;
	}
	for (const ServerInGameInfo &info : baseEntities)
	{
		std::shared_ptr<AnimationComponent>		animComp;
		std::shared_ptr<PropertyComponent>		propComp = std::make_shared<PropertyComponent>();
		std::shared_ptr<Entity>	ent = std::make_shared<Entity>(info.entityId);
		propComp->configure(info);
		animComp = std::make_shared<AnimationComponent>();
		animComp->configure(info);
		ent->addComponent(animComp->getName(), animComp);
		ent->addComponent(propComp->getName(), propComp);
		switch (gameStat)
		{
		case GameStat::CONNEXION:
			settingUpConnexionMenuEntity(ent, propComp, idx);
			addToSystems(ent);
			tmpEntities.push_back(ent);
			break;
		case GameStat::CONNECTED:
			settingUpConnectedMenuEntity(ent, propComp);
			addToSystems(ent);
			tmpEntities.push_back(ent);
			break;
		case GameStat::IN_ROOM:
			break;
		case GameStat::IN_GAME:
			break;
		}
		idx++;
	}
}

void	SystemManager::addToSystems(std::shared_ptr<Entity> ent)
{
	for (std::shared_ptr<System> sys : systems)
	{
		if (sys->getName().compare("Controller") == 0 && (ent->hasComponent("Selection") == true || ent->hasComponent("Notification") == true))
			sys->addEntity(ent);
		else if (sys->getName().compare("GUI") == 0)
			sys->addEntity(ent);
	}
}

void	SystemManager::clearEntities()
{
	for (std::shared_ptr<System> sys : systems)
		sys->clearEntities();
	entityNb = 1;
	for (std::shared_ptr<Entity> ent : starfieldEntities)
		addToSystems(ent);
	addToSystems(notificationEnt);
}

std::vector<ServerInGameInfo>	SystemManager::initBaseInRoomEntities()
{
	std::vector<ServerInGameInfo> baseEntities;
	int w = Ressources::get()->getWinSize().width;
	int h = Ressources::get()->getWinSize().height;
	sf::FloatRect tmp = Ressources::get()->anims.at("Bouton2").getSprite(convertAnimTypeToString(AnimationType::STATIC), 0).getGlobalBounds();
	baseEntities.emplace_back(genInGameInfo("Bouton2", w / 2, static_cast<int>(tmp.height / 2) + static_cast<int>(tmp.height * 0.1), 0, AnimationType::STATIC));
	baseEntities.emplace_back(genInGameInfo("Bouton", w / 4, h / 3, 0, AnimationType::STATIC));
	baseEntities.emplace_back(genInGameInfo("Bouton", w - (w / 4), h / 3, 0, AnimationType::STATIC));
	baseEntities.emplace_back(genInGameInfo("Bouton", w / 4, h - (h / 4), 0, AnimationType::STATIC));
	baseEntities.emplace_back(genInGameInfo("Bouton", w - (w / 4), h - (h / 4), 0, AnimationType::STATIC));
	return baseEntities;
}

std::vector<ServerInGameInfo>	SystemManager::initBaseConnectedEntities()
{
	std::vector<ServerInGameInfo> baseEntities;
	sf::FloatRect tmp = Ressources::get()->anims.at("Bouton").getSprite(convertAnimTypeToString(AnimationType::STATIC), 0).getGlobalBounds();
	baseEntities.emplace_back(genInGameInfo("Bouton", static_cast<int>(tmp.width), static_cast<int>(tmp.height), 0, AnimationType::STATIC));
	return baseEntities;
}

std::vector<ServerInGameInfo>	SystemManager::initBaseConnexionEntities()
{
	std::vector<ServerInGameInfo> baseEntities;
	int w = Ressources::get()->getWinSize().width;
	int h = Ressources::get()->getWinSize().height;
	baseEntities.emplace_back(genInGameInfo("Bouton", w / 2, h / 4, 0, AnimationType::STATIC));
	baseEntities.emplace_back(genInGameInfo("Bouton", w / 2, h / 2, 0, AnimationType::STATIC));
	baseEntities.emplace_back(genInGameInfo("Bouton", w / 2, h - (h / 4), 0, AnimationType::STATIC));
	return baseEntities;
}

void	SystemManager::genLoadRessourcesInfo(const std::string &name,
	const std::string &fileName,
	const int &beginX,
	const int &beginY,
	const int &height,
	const int &width,
	const int &incX,
	const AnimationType &type,
	const int &nbSprite,
	const bool &scale)
{
	ServerLoadRessourcesInfo	baseEntity;
	int					i;
	for (i = 0; i < fileName.size() && i < 29; i++)
		baseEntity.fileName[i] = fileName[i];
	baseEntity.fileName[i] = '\0';
	for (i = 0; i < name.size() && i < 29; i++)
		baseEntity.entityName[i] = name[i];
	baseEntity.entityName[i] = '\0';
	baseEntity.type = type;
	baseEntity.beginX = beginX;
	baseEntity.beginY = beginY;
	baseEntity.height = height;
	baseEntity.width = width;
	baseEntity.incX = incX;
	baseEntity.scale = scale;
	baseEntity.nbSprite = nbSprite;
	Ressources::get()->actualiseAnimationSet(baseEntity);
}

void	SystemManager::initBaseRessources()
{
	Ressources::get()->loadFolder("sprites/base");
	std::vector<std::string> vec;
	vec.push_back("PlayerBlue");
	vec.push_back("PlayerRed");
	vec.push_back("PlayerYellow");
	vec.push_back("PlayerGreen");
	std::vector<AnimationType> types;
	types.push_back(AnimationType::DOWN);
	types.push_back(AnimationType::STATIC);
	types.push_back(AnimationType::UP);
	int	beginY = 2;
	for (const std::string &name : vec)
	{
		int beginX = 33;
		int incX = -33;
		for (const AnimationType &animType : types)
		{
			int nbSprite = 0;
			if (animType == AnimationType::STATIC)
				nbSprite = 1;
			else
				nbSprite = 2;
			genLoadRessourcesInfo(name, "players.png", beginX, beginY, 15, 33, incX, animType, nbSprite, true);
			beginX += 33;
			incX += 33;
		}
		beginY += 17;
	}
	genLoadRessourcesInfo("Bouton", "bouton.png", 0, 0, 100, 200, 0, AnimationType::STATIC, 1, true);
	genLoadRessourcesInfo("Bouton2", "bouton2.png", 0, 0, 75, 400, 0, AnimationType::STATIC, 1, true);
	genLoadRessourcesInfo("BoutonReady", "boutonReady.png", 0, 0, 50, 50, 0, AnimationType::STATIC, 1, true);
	genLoadRessourcesInfo("Starfield", "starfield.png", 0, 0, 1080, 3000, 0, AnimationType::STATIC, 1, false);
	genLoadRessourcesInfo("Sun", "sun.png", 0, 0, 500, 500, 0, AnimationType::STATIC, 1, false);
	genLoadRessourcesInfo("Notification", "notification.png", 0, 0, 100, 200, 0, AnimationType::STATIC, 1, true);
	genLoadRessourcesInfo("RickAndMorty", "ricketmorty.png", 0, 0, 50, 50, 0, AnimationType::STATIC, 1, false);
	genLoadRessourcesInfo("DeathStar", "deathStar.png", 0, 0, 200, 200, 0, AnimationType::STATIC, 1, false);
	genLoadRessourcesInfo("NyanCat", "nyancat.png", 0, 0, 75, 75, 0, AnimationType::STATIC, 1, false);
}

ServerInGameInfo				SystemManager::genInGameInfo(const std::string &name,
	const int &posX,
	const int& posY,
	const int &loopMode,
	const AnimationType &type)
{
	ServerInGameInfo	baseEntity;
	int	i;

	baseEntity.entityId = entityNb++;
	for (i = 0; i < name.size() && i < 29; i++)
		baseEntity.entityType[i] = name[i];
	baseEntity.entityType[i] = '\0';
	baseEntity.posX = posX;
	baseEntity.posY = posY;
	baseEntity.loopMode = loopMode;
	baseEntity.type = AnimationType::STATIC;
	return baseEntity;
}

std::vector<ServerInGameInfo>	SystemManager::initStarfieldEntityInfo()
{
	std::vector<ServerInGameInfo> baseEntities;
	int w = Ressources::get()->getWinSize().width;
	int h = Ressources::get()->getWinSize().height;

	baseEntities.emplace_back(genInGameInfo("Starfield", 0, h / 2, 0, AnimationType::STATIC));
	baseEntities.emplace_back(genInGameInfo("Sun", w, h, 0, AnimationType::STATIC));
	baseEntities.emplace_back(genInGameInfo("DeathStar", w + w / 2, h / 10, 0, AnimationType::STATIC));
	baseEntities.emplace_back(genInGameInfo("RickAndMorty", w + w / 3, h / 3, 0, AnimationType::STATIC));
	baseEntities.emplace_back(genInGameInfo("NyanCat", w, h - (h / 4), 0, AnimationType::STATIC));
	return baseEntities;
}

void	SystemManager::initNotification()
{
	std::string name = "Notification";
	int w = Ressources::get()->getWinSize().width;
	ServerInGameInfo	baseEntity = genInGameInfo("Notification", w, 0, 0, AnimationType::STATIC);

	std::shared_ptr<AnimationComponent>		animComp;
	std::shared_ptr<PropertyComponent>		propComp = std::make_shared<PropertyComponent>();
	notificationEnt = std::make_shared<Entity>(baseEntity.entityId);
	propComp->configure(baseEntity);
	std::cout << baseEntity.entityType << std::endl;
	animComp = std::make_shared<AnimationComponent>();
	animComp->configure(baseEntity);
	notificationEnt->addComponent(animComp->getName(), animComp);
	notificationEnt->addComponent(propComp->getName(), propComp);
	if (propComp->getType().compare("Notification") == 0)
	{
		std::shared_ptr<NotificationComponent> notComp = std::make_shared<NotificationComponent>();
		notComp->configure(baseEntity);
		notComp->setNotificationSpeed(5);
		notificationEnt->addComponent(notComp->getName(), notComp);
	}
}

void	SystemManager::initStarfield()
{
	std::vector<ServerInGameInfo> baseEntities = initStarfieldEntityInfo();
	float idx = 0.5;
	for (const ServerInGameInfo &info : baseEntities)
	{
		std::shared_ptr<AnimationComponent>		animComp;
		std::shared_ptr<PropertyComponent>		propComp = std::make_shared<PropertyComponent>();
		std::shared_ptr<Entity>	ent = std::make_shared<Entity>(info.entityId);
		propComp->configure(info);
		std::cout << info.entityType << std::endl;
		animComp = std::make_shared<AnimationComponent>();
		animComp->configure(info);
		ent->addComponent(animComp->getName(), animComp);
		ent->addComponent(propComp->getName(), propComp);
		std::shared_ptr<ScrollComponent> scrollComp = std::make_shared<ScrollComponent>();
		scrollComp->configure(info);
		scrollComp->setScrollSpeed(idx);
		if (propComp->getType().compare("Starfield") != 0)
		{
			if (propComp->getType().compare("DeathStar") == 0)
				scrollComp->setScrollRotationInc(idx * 0.1 * -1);
			else
				scrollComp->setScrollRotationInc(idx * 0.1);
		}
		ent->addComponent(scrollComp->getName(), scrollComp);
		starfieldEntities.emplace_back(ent);
		addToSystems(ent);
		idx += 0.2;
	}
}

void	SystemManager::actualiseEntity(ServerInGameInfo const &info)
{
	//std::cout << "InGameEntityId:" << info.entityId << ":type:" << info.entityType << ":Action:" << convertAnimTypeToString(info.type) << std::endl;
	//	Ressources::get()->printAllType();
	std::shared_ptr<AnimationComponent>		animComp;
	std::shared_ptr<PropertyComponent>		propComp = std::make_shared<PropertyComponent>();
	propComp->configure(info);
	for (std::shared_ptr<System> sys : systems)
	{
		if (sys->getName().compare("GUI") == 0)
		{
			if (sys->hasEntity(info.entityId) == true)
			{
				sys->getEntity(info.entityId)->IsUsed();
				propComp = std::static_pointer_cast<PropertyComponent>(sys->getEntity(info.entityId)->getComponent("Property"));
				propComp->configure(info);
				if (sys->getEntity(info.entityId)->hasComponent("anim" + convertAnimTypeToString(propComp->getCurrentAction())) == false)
				{
					animComp = std::make_shared<AnimationComponent>();
					animComp->configure(info);
					sys->getEntity(info.entityId)->addComponent(animComp->getName(), animComp);
				}
			}
			else
			{
				std::shared_ptr<Entity> ent = std::make_shared<Entity>(info.entityId);
				animComp = std::make_shared<AnimationComponent>();
				animComp->configure(info);
				ent->addComponent(animComp->getName(), animComp);
				ent->addComponent(propComp->getName(), propComp);
				ent->IsUsed();
				sys->addEntity(ent);
				if (propComp->getType().find("Starship") != std::string::npos)
				{
					std::shared_ptr<TextComponent> textComp = std::make_shared<TextComponent>("NameText", false);
					std::vector<std::string> col;
					col.emplace_back("Blue");
					col.emplace_back("Yellow");
					col.emplace_back("Green");
					col.emplace_back("Red");
					int i;
					for (i = 0; i < col.size(); i++)
						if (propComp->getType().find(col.at(i)) != std::string::npos)
							break;
					textComp->configure(playerNames[i], propComp->getEntityPos().x, propComp->getEntityPos().y - (propComp->getSize().y / 2));
					sys->getEntity(info.entityId)->addComponent(textComp->getName(), textComp);
				}
			}
		}
	}
}

ServerInGameInfo	SystemManager::genRoomStruct(const ServerRoomsInfo &info, const int &roomNb)
{
	ServerInGameInfo	baseEntity;
	std::string		name = "Bouton";
	size_t	i;
	int w = Ressources::get()->getWinSize().width;
	int h = Ressources::get()->getWinSize().height;
	sf::FloatRect size = Ressources::get()->getAnimSprite("Bouton", "Static", 0).getGlobalBounds();

	baseEntity.entityId = entityNb++;
	for (i = 0; i < name.size() && i < 29; i++)
		baseEntity.entityType[i] = name[i];
	baseEntity.entityType[i] = '\0';
	baseEntity.posX = w / 2 + (w / 4);
	baseEntity.posY = static_cast<unsigned int>(size.height * 1.1 * roomNb);
	baseEntity.loopMode = 0;
	baseEntity.type = AnimationType::STATIC;
	return baseEntity;
}

void	SystemManager::initBasePlayerEntInfo()
{
	ServerInGameInfo	baseEntity;
	std::vector<std::string> vec;
	vec.push_back("PlayerBlue");
	vec.push_back("PlayerRed");
	vec.push_back("PlayerYellow");
	vec.push_back("PlayerGreen");
	int i;
	for (const std::string &name : vec)
	{
		baseEntity.entityId = entityNb++;
		for (i = 0; i < name.size() && i < 29; i++)
			baseEntity.entityType[i] = name[i];
		baseEntity.entityType[i] = '\0';
		baseEntity.posX = 15;
		baseEntity.posY = 15;
		baseEntity.loopMode = 0;
		baseEntity.type = AnimationType::STATIC;
		entPlayerInfo.emplace_back(baseEntity);
	}

	std::string name = "BoutonReady";
	baseEntity.entityId = entityNb++;
	for (i = 0; i < name.size() && i < 29; i++)
		baseEntity.entityType[i] = name[i];
	baseEntity.entityType[i] = '\0';
	baseEntity.posX = 50;
	baseEntity.posY = 50;
	baseEntity.loopMode = 0;
	baseEntity.type = AnimationType::STATIC;
	entReadyButtonInfo = baseEntity;
}

void	SystemManager::restoreBaseEnt()
{
	for (std::shared_ptr<System> sys : systems)
		for (std::shared_ptr<Entity> ent : tmpEntities)
		{
			if (sys->getName().compare("Controller") == 0 && ent->hasComponent("Selection") == true)
				sys->addEntity(ent);
			else if (sys->getName().compare("GUI") == 0)
				sys->addEntity(ent);
		}
}

void	SystemManager::actualiseRooms()
{
	clearEntities();
	restoreBaseEnt();
	int roomNb = 1;
	std::cout << "acutaliseRoom" << std::endl;
	for (const ServerRoomsInfo &info : Network::get()->getRoomsInfo())
	{
		ServerInGameInfo	entInfo = genRoomStruct(info, roomNb);
		std::shared_ptr<AnimationComponent>		animComp;
		std::shared_ptr<PropertyComponent>		propComp = std::make_shared<PropertyComponent>();
		std::shared_ptr<Entity>	ent = std::make_shared<Entity>(entInfo.entityId);
		propComp->configure(entInfo);
		for (std::shared_ptr<System> sys : systems)
		{
			animComp = std::make_shared<AnimationComponent>();
			animComp->configure(entInfo);
			ent->addComponent(animComp->getName(), animComp);
			ent->addComponent(propComp->getName(), propComp);
			if (propComp->getType().compare("Bouton") == 0)
			{
				std::string text = "Partie de ";
				text += info.ownerName;
				text += ":Level " + std::to_string(info.lvlNb);
				propComp->setRoomNb(info.roomNb);
				std::shared_ptr<TextComponent> textComp = std::make_shared<TextComponent>("TitleText", false);
				textComp->configure(text, propComp->getEntityPos().x, propComp->getEntityPos().y - (propComp->getSize().y / 2));
				std::cout << textComp->getContent() << std::endl;
				ent->addComponent(textComp->getName(), textComp);

				for (unsigned int i = 0; i < info.nbPlayer; i++)
				{
					text = "subAnim" + std::to_string(i);
					std::shared_ptr<AnimationComponent>		subAnimComp = std::make_shared<AnimationComponent>();
					subAnimComp->configure(entPlayerInfo.at(i));
					switch (i)
					{
					case 0:
						subAnimComp->setPos(subPosition::LEFT);
					case 1:
						subAnimComp->setPos(subPosition::RIGHT);
					case 2:
						subAnimComp->setPos(subPosition::TOP);
					case 3:
						subAnimComp->setPos(subPosition::BOTTOM);
					}
					ent->addComponent(subAnimComp->getName(), subAnimComp);
				}

				std::shared_ptr<SelectionComponent> selectComp = std::make_shared<SelectionComponent>();
				ent->addComponent(selectComp->getName(), selectComp);
			}
			if (sys->getName().compare("Controller") == 0 && ent->hasComponent("Selection") == true)
				sys->addEntity(ent);
			else if (sys->getName().compare("GUI") == 0)
				sys->addEntity(ent);
		}
		roomNb++;
	}
}

void	SystemManager::actualiseInRoom()
{
	clearEntities();
	tmpEntities.clear();
	std::vector<ServerInGameInfo> baseEntities = initBaseInRoomEntities();
	ServerInRoomInfo rInfo = Network::get()->getInRoomInfo();
	for (unsigned int i = 0; i < rInfo.playerNb && i < 4; i++)
		playerNames[i] = rInfo.playersName[i];
	unsigned int idx = 0;
	for (const ServerInGameInfo &info : baseEntities)
	{
		std::shared_ptr<AnimationComponent>		animComp;
		std::shared_ptr<PropertyComponent>		propComp = std::make_shared<PropertyComponent>();
		std::shared_ptr<Entity>	ent = std::make_shared<Entity>(info.entityId);
		propComp->configure(info);
		std::cout << info.entityType << std::endl;
		animComp = std::make_shared<AnimationComponent>();
		animComp->configure(info);
		ent->addComponent(animComp->getName(), animComp);
		ent->addComponent(propComp->getName(), propComp);
		if (propComp->getType().compare("Bouton2") == 0)
		{
			std::string text = "Partie créé par ";
			text += rInfo.playersName[0];
			std::shared_ptr<TextComponent> textComp = std::make_shared<TextComponent>("TitleText", false);
			textComp->configure(text, propComp->getEntityPos().x, propComp->getEntityPos().y);
			std::cout << textComp->getContent() << std::endl;
			ent->addComponent(textComp->getName(), textComp);
		}
		if (propComp->getType().compare("Bouton") == 0)
		{
			if (idx < rInfo.playerNb)
			{
				std::cout << idx << std::endl;
				if (rInfo.ready[idx] == true)
				{
					propComp->changeIsReady();
					std::cout << "true" << std::endl;
				}
				else
					std::cout << "true" << std::endl;
				if (rInfo.loaded[idx] == true)
					propComp->changeIsLoaded();
				std::string text = "Player" + std::to_string(idx) + " : ";
				text += rInfo.playersName[idx];
				std::shared_ptr<TextComponent> textComp = std::make_shared<TextComponent>("TitleText", false);
				textComp->configure(text, propComp->getEntityPos().x, propComp->getEntityPos().y - (propComp->getSize().y / 2));
				std::cout << textComp->getContent() << std::endl;
				ent->addComponent(textComp->getName(), textComp);

				std::shared_ptr<AnimationComponent> subAnimComp = std::make_shared<AnimationComponent>();
				if (idx < entPlayerInfo.size())
					subAnimComp->configure(entPlayerInfo.at(idx));
				subAnimComp->setPos(subPosition::LEFT);
				propComp->addSubAnim(subAnimComp->getName());
				ent->addComponent(subAnimComp->getName(), subAnimComp);
				std::shared_ptr<AnimationComponent> isReadyButtonComp = std::make_shared<AnimationComponent>();
				isReadyButtonComp->configure(entReadyButtonInfo);
				isReadyButtonComp->setPos(subPosition::RIGHT);
				isReadyButtonComp->setName("BoutonReady");
				propComp->addSubAnim(isReadyButtonComp->getName());
				ent->addComponent(isReadyButtonComp->getName(), isReadyButtonComp);
			}
			else
			{
				std::string text = "Empty";
				std::shared_ptr<TextComponent> textComp = std::make_shared<TextComponent>("TitleText", false);
				textComp->configure(text, propComp->getEntityPos().x, propComp->getEntityPos().y);
				std::cout << textComp->getContent() << std::endl;
				ent->addComponent(textComp->getName(), textComp);
			}
			idx++;
		}
		addToSystems(ent);
		tmpEntities.push_back(ent);
	}
	unsigned int i;
	for (i = 0; i < rInfo.playerNb; i++)
		if (rInfo.loaded[i] != true)
			break;
	if (i == rInfo.playerNb)
		if (Network::get()->initUdpNetwork() == true)
			gameStat = GameStat::IN_GAME;
}

void	SystemManager::interpretNetworkInfo()
{
	if (Network::get()->getIsUndredeTcpMessage() == true)
	{
		switch (gameStat)
		{
		case GameStat::CONNECTED:
			if (notificationEnt->hasComponent("Notification"))
				std::static_pointer_cast<NotificationComponent>(notificationEnt->getComponent("Notification"))->newNotification(Network::get()->getLastAction());
			actualiseRooms();
			break;
		case GameStat::IN_ROOM:
			if (notificationEnt->hasComponent("Notification"))
				std::static_pointer_cast<NotificationComponent>(notificationEnt->getComponent("Notification"))->newNotification(Network::get()->getLastAction());
			actualiseInRoom();
			break;
		}
		Network::get()->setTcpMessageReaded();
	}
	if (Network::get()->getIsUndredeUdpMessage() == true)
	{
		if (gameStat == GameStat::IN_GAME)
		{
			std::unordered_map<int, ServerInGameInfo> infos = Network::get()->getInGameInfo();
			Network::get()->askToClearInGameMap();
			int id = 0;
			for (const auto &info : infos)
			{
				id = info.first;
				if (id >= 1000000)
					actualiseEntity(info.second);
			}
			if (id >= 1000000)
				deleteUnusedEntities();
			Network::get()->setUdpMessageReaded();
		}
	}
}

void	SystemManager::deleteUnusedEntities()
{
	std::vector<int>	entities;
	for (std::shared_ptr<System> sys : systems)
		if (sys->getName().compare("GUI") == 0)
			for (std::shared_ptr<Entity> ent : sys->getEntities())
				if (ent->getIsUsed() == false)
					entities.emplace_back(ent->getId());
	for (std::shared_ptr<System> sys : systems)
		if (sys->getName().compare("GUI") == 0)
			for (int id : entities)
				if (id > 100)
					if (sys->hasEntity(id))
						sys->removeEntity(id);
}

void	SystemManager::execLoop()
{
	isGameRunning = true;
	initBaseSystems();
	initBaseRessources();
	initBasePlayerEntInfo();
	initStarfield();
	initNotification();
	//	gameStat = GameStat::CONNECTED;
	menuInitialisation();
	GameStat	lastStat = gameStat;
	while (isGameRunning == true)
	{
		if (lastStat != gameStat)
		{
			if (notificationEnt->hasComponent("Notification"))
				std::static_pointer_cast<NotificationComponent>(notificationEnt->getComponent("Notification"))->newNotification(Network::get()->getLastAction());
			menuInitialisation();
			lastStat = gameStat;
		}
		interpretNetworkInfo();
		for (std::shared_ptr<System> sys : systems)
			sys->exec();
	}
}

void	SystemManager::addSystem(std::shared_ptr<System> sys)
{
	systems.emplace_back(sys);
}

void	SystemManager::removeSystem(const std::string &name)
{
	int i = 0;
	for (std::shared_ptr<System> sys : systems)
	{
		if (sys->getName().compare(name) == 0)
			break;
		i++;
	}
	if (i < systems.size())
		systems.erase(systems.begin() + i);
}