#ifndef PropertyComponent_H_
# define PropertyComponent_H_

#include <string>
#include <vector>
#include <SFML\Graphics.hpp>
#include "Component.h"
#include "Protocol.h"

class PropertyComponent : public Component
{
	std::string	type;
	std::string	name;
	std::string animStat;
	EntityPos	pos;
	EntityPos	size;
	AnimationType lastAction;
	AnimationType currentAction;
	int			roomNb = 0;
	bool		isReady = false;
	bool		isLoaded = false;
	std::vector<std::string>	subAnim;
	bool		isSubAnim = false;
public:
	PropertyComponent();
	~PropertyComponent();
	const std::string	&getType() const { return type; }
	const std::string	&getName() const { return name; }
	const std::string	&getAnimStat() const { return animStat; }
	const AnimationType	&getLastAction() const { return lastAction; }
	const AnimationType	&getCurrentAction() const { return currentAction; }
	const EntityPos		&getEntityPos() const { return pos; }
	const EntityPos		&getSize() const { return size; }
	const bool			&getIsReady() const { return isReady; }
	const bool			&getIsLoaded() const { return isLoaded; }
	void				changeIsReady() { if (isReady == true) isReady = false; else isReady = true; }
	void				changeIsLoaded() { if (isLoaded == true) isLoaded = false; else isLoaded = true; }
	void				configure(ServerInGameInfo const &);
	void				setRoomNb(const int &nb) { roomNb = nb; }
	const int			&getRoomNb() { return roomNb; }
	const std::vector<std::string> &getSubAnim() { return subAnim; }
	void				addSubAnim(const std::string &name) { subAnim.emplace_back(name); isSubAnim = true; }
	const bool			&getIsSubAnim() { return isSubAnim; }
};

#endif /*! PropertyPropertyComponent_H_*/