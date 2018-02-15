#pragma once
#include "Component.h"
#include "Hitbox.h"
#include <list>
#include "Protocol.h"

class PhysicComponent :
	public Component
{
public:
	PhysicComponent();
	PhysicComponent(const std::string &name, const std::string &type);
	~PhysicComponent();

	Hitbox pos;
	std::string	shoot;
	int shootDelay;
	int hp;
	std::list<Hitbox>	hitboxes;
	int ally;
	AnimationType animationType;
	int random;
};
