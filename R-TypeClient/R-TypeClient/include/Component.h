#ifndef Component_H_
# define Component_H_

#include <string>
#include <vector>

struct EntityPos
{
	float x;
	float y;
};

class Component
{
	std::string	type;
	std::string	name;
public:
	Component();
	~Component();
	const std::string	&getType() const { return type; }
	const std::string	&getName() const { return name; }
};

#endif /*! Component_H_*/