#include "stdafx.h"
#include "PhysicComponent.h"


PhysicComponent::PhysicComponent()
{
	type = "physic";
	name = "physic";
}

PhysicComponent::PhysicComponent(const std::string &_name, const std::string &_type)
{
	name = _name;
	type = _type;
}

PhysicComponent::~PhysicComponent()
{
}
