#ifndef SelectionComponent_H_
# define SelectionComponent_H_

#include <string>
#include <vector>
#include <SFML\Graphics.hpp>
#include "Protocol.h"
#include "Component.h"

class SelectionComponent : public Component
{
	std::string	type;
	std::string	name = "Selection";
	bool		isSelected = false;
public:
	SelectionComponent() {}
	const bool	&getIsSelected() { return isSelected; }
	void		changeSelection() { if (isSelected == true) isSelected = false; else isSelected = true; }
	const std::string	&getName() const { return name; }
};

#endif /*! SelectionComponent_H_*/