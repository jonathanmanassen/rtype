#ifndef CountComponent_H_
# define CountComponent_H_
#include "Component.h"
#include <iostream>

class CountComponent : public Component
{
	size_t			id;
	std::vector<std::string>	content;
	std::string	name;
	EntityPos	pos;
public:
	CountComponent(const std::string &_name, const bool &editStat) : name(_name), id(0){}
	size_t					getId() { return id; }
	const std::string	&getContent() const { return content.at(id); }
	void				nextContent() { if (id < content.size() - 1) id++; else id = 0; }
	void				prevContent() { if (id > 0) id--; else id = content.size() - 1; }
	void				addContent(const std::string &c) { content.push_back(c); }
	const std::string	&getName() const { return name; }
	void				configure(const std::vector<std::string> &name, const float &x, const float &y);
	const EntityPos		&getPos() { return pos; }
};

#endif /*!CountComponent*/