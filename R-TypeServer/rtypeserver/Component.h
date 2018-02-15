#pragma once
#include <string>

class Component
{
public:
	Component() {}
	Component(const std::string &_type, const std::string &_name);
	~Component();
	const std::string	&getType() const { return type; }
	const std::string	&getName() const { return name; }
	std::string	name;
	std::string	type;
};
