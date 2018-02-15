#include "stdafx.h"
#include "PhysicSystem.h"
#include "PhysicComponent.h"
#include "Ressource.h"
#include <iostream>
#include "Game.h"

PhysicSystem::PhysicSystem()
{
}

PhysicSystem::~PhysicSystem()
{
	std::cout << "destruct PhysicSystem" << std::endl;
}

void	PhysicSystem::run(const int &frame, Client* _clients[4], int &_nbClients, std::vector<int> &clientsId)
{
	for (auto &entity : entities)
	{
		IMonster *monster = res->getMonster(entity.second->getName());
		monster->enemies.clear();
		monster->shoot = "";
		for (auto &component : entity.second->components)
		{
			if (component.second->type.compare("physic") == 0)
			{
				std::shared_ptr<PhysicComponent> physic = std::static_pointer_cast<PhysicComponent>(component.second);
				
				int clientPos = clientsIdPos(clientsId, entity.first);
				if (clientPos == -1)
				{
					monster->random = physic->random;
					if (clientsId.size() != 0)
					{
						for (int i = 0; i < _nbClients; i++)
						{
							if (i < clientsId.size())
								if (hasEntity(clientsId.at(i)))
								{
									std::shared_ptr<PhysicComponent> physic = std::static_pointer_cast<PhysicComponent>(entities.at(clientsId.at(i))->getComponent("physic"));
					
									Hitbox pos;
									pos.x = physic->pos.x;
									pos.y = physic->pos.y;

									monster->enemies.emplace_back(pos);
								}
						}
					}
					monster->pos.change(physic->pos.x, physic->pos.y);
					monster->ally = physic->ally;
					monster->ai(frame);

					physic->animationType = AnimationType::STATIC;
					if (monster->pos.y < physic->pos.y)
						physic->animationType = AnimationType::UP;
					else if (monster->pos.y > physic->pos.y)
						physic->animationType = AnimationType::DOWN;
					physic->pos.change(monster->pos.x, monster->pos.y);
					if (monster->shoot != "")
					{
						physic->animationType = AnimationType::ATTACK;
						createMonster("missile", physic->pos.x, physic->pos.y, physic->ally);
					}
				}
				else
				{
					clientRun(physic, _clients, clientPos);
				}
				collision(entity.first, physic);
			}
		}
	}
	removeDeadMonsters(_nbClients, clientsId);
}

void	PhysicSystem::clientRun(std::shared_ptr<PhysicComponent> physic, Client *_clients[4], int clientPos)
{
	physic->pos.y += 5 * _clients[clientPos]->getDir().down;
	physic->pos.y += -5 * _clients[clientPos]->getDir().up;
	physic->pos.x += -5 * _clients[clientPos]->getDir().left;
	physic->pos.x += 5 * _clients[clientPos]->getDir().right;
	if (_clients[clientPos]->getDir().up && _clients[clientPos]->getDir().down == false)
		physic->animationType = AnimationType::UP;
	else if (_clients[clientPos]->getDir().down && _clients[clientPos]->getDir().up == false)
		physic->animationType = AnimationType::DOWN;
	else
		physic->animationType = AnimationType::STATIC;

	int max_width_hitbox = 0;
	int max_height_hitbox = 0;
	for (auto &hitbox : physic->hitboxes)
	{
		if (hitbox.x + hitbox.width > max_width_hitbox)
			max_width_hitbox = hitbox.x + hitbox.width;
		if (hitbox.x + hitbox.height > max_height_hitbox)
			max_height_hitbox = hitbox.x + hitbox.height;
	}
	if (physic->pos.x + max_width_hitbox >= 1920)
		physic->pos.x = 1920 - max_width_hitbox;
	if (physic->pos.x < 0)
		physic->pos.x = 0;
	if (physic->pos.y + max_height_hitbox >= 1080)
		physic->pos.y = 1080 - max_height_hitbox;
	if (physic->pos.y < 0)
		physic->pos.y = 0;

	if (_clients[clientPos]->getShooting() == true)
	{
		physic->shootDelay += 1;
	}
	else if (_clients[clientPos]->getShooting() == false && physic->shootDelay <= 100 && physic->shootDelay > 5)
	{
		physic->shootDelay = 0;
		physic->animationType = AnimationType::ATTACK;
		createMonster("bluemissile", physic->pos.x, physic->pos.y, physic->ally);
	}
	else if (_clients[clientPos]->getShooting() == false && physic->shootDelay > 100)
	{
		physic->shootDelay = 0;
		physic->animationType = AnimationType::ATTACK;
		createMonster("bigbluemissile", physic->pos.x, physic->pos.y, physic->ally);
	}
}

int	PhysicSystem::clientsIdPos(const std::vector<int> &vec, const int &id)
{
	int	i = 0;
	for (auto &tmp : vec)
	{
		if (tmp == id)
			return (i);
		i++;
	}
	return (-1);
}

void	PhysicSystem::getEntitiesInfo(std::vector<ServerInGameInfo> &list)
{
	for (auto entity : entities)
	{
		if (entity.second->hasComponent("physic") == 1)
		{
			ServerInGameInfo info;
			info.entityId = entity.first;
			res->strcmp(info.entityType, entity.second->getName().c_str());
			std::shared_ptr<PhysicComponent> physic = std::static_pointer_cast<PhysicComponent>(entity.second->components.at("physic"));

			if (res->hasAnimationType(entity.second->getName(), physic->animationType) == 1)
			{
				info.type = physic->animationType;
				info.loopMode = res->monsters.at(entity.second->getName())->sprites.at(physic->animationType).loop;
			}
			else
				info.type = AnimationType::STATIC;
			info.posX = physic->pos.x;
			info.posY = physic->pos.y;

			list.emplace_back(info);
		}
	}
}

void	PhysicSystem::collision(const int &id, const std::shared_ptr<PhysicComponent> &physic)
{
	for (auto &entity : entities)
	{
		if (entity.first != id)
		{
			for (auto &component : entity.second->components)
			{
				if (component.second->type == "physic")
				{
					std::shared_ptr<PhysicComponent> physic2 = std::static_pointer_cast<PhysicComponent>(component.second);
					if (physic->ally != physic2->ally)
					{
						for (auto &hitbox : physic->hitboxes)
						{
							for (auto &hitbox2 : physic2->hitboxes)
							{
								if (physic2->pos.x + hitbox2.x >= physic->pos.x + hitbox.x &&
									physic2->pos.x + hitbox2.x <= physic->pos.x + hitbox.x + hitbox.width ||
									physic2->pos.x + hitbox2.x + hitbox2.width >= physic->pos.x + hitbox.x &&
									physic2->pos.x + hitbox2.x + hitbox2.width <= physic->pos.x + hitbox.x + hitbox.width ||
									physic->pos.x + hitbox.x >= physic2->pos.x + hitbox2.x &&
									physic->pos.x + hitbox.x <= physic2->pos.x + hitbox2.x + hitbox2.width ||
									physic->pos.x + hitbox.x + hitbox.width >= physic2->pos.x + hitbox2.x &&
									physic->pos.x + hitbox.x + hitbox.width <= physic2->pos.x + hitbox2.x + hitbox2.width)
								{
									if (physic2->pos.y + hitbox2.y >= physic->pos.y + hitbox.y &&
										physic2->pos.y + hitbox2.y <= physic->pos.y + hitbox.y + hitbox.height ||
										physic2->pos.y + hitbox2.y + hitbox2.height >= physic->pos.y + hitbox.y &&
										physic2->pos.y + hitbox2.y + hitbox2.height <= physic->pos.y + hitbox.y + hitbox.height ||
										physic->pos.y + hitbox.y >= physic2->pos.y + hitbox2.y &&
										physic->pos.y + hitbox.y <= physic2->pos.y + hitbox2.y + hitbox.height ||
										physic->pos.y + hitbox.y + hitbox.height >= physic2->pos.y + hitbox2.y &&
										physic->pos.y + hitbox.y + hitbox.height <= physic2->pos.y + hitbox2.y + hitbox2.height)
									{
										int tmphp = physic->hp;
										int tmphp2 = physic2->hp;
										if (tmphp2 > 0)
											physic->hp += -tmphp2;
										if (tmphp > 0)
											physic2->hp += -tmphp;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void	PhysicSystem::removeDeadMonsters(int &_nbClients, std::vector<int> &clientsId)
{
	std::list<int> removeList;

	for (auto &entity : entities)
	{
		for (auto &component : entity.second->components)
		{
			if (component.second->type == "physic")
			{
				std::shared_ptr<PhysicComponent> physic = std::static_pointer_cast<PhysicComponent>(component.second);
				int max_hitbox = 0;
				for (auto &hitbox : physic->hitboxes)
				{
					if (hitbox.x + hitbox.width > max_hitbox)
						max_hitbox = hitbox.x + hitbox.width;
				}
				
				
				
				if (physic->hp <= 0 || physic->pos.x + max_hitbox < 0 || physic->pos.x > 1920)
				{
					removeList.push_back(entity.first);
					int clientPos = clientsIdPos(clientsId, entity.first);
					if (clientPos != -1)
					{
						clientsId.erase(clientsId.begin() + clientPos);
					}
				}
			}
		}
	}

	for (auto &tmp : removeList) {
		entities.erase(tmp);
	}
}