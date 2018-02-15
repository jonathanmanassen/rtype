#ifndef AnimationComponent_H_
# define AnimationComponent_H_

#include <string>
#include <vector>
#include <SFML\Graphics.hpp>
#include "Protocol.h"
#include "Component.h"

enum class subPosition
{
	RIGHT,
	MIDDLE,
	LEFT,
	TOP,
	BOTTOM,
};

class AnimationComponent : public Component
{
	std::string	type;
	std::string	name;
	std::string animStat;
	EntityPos	size;
	size_t			currentFrame = 0;
	int			loopMode = 0;
	int			startFrame = 0;
	size_t		nbSprite;
	subPosition	pos;
	int			animSpeed = 10;
	int			animStack = 0;
public:
	const std::string	&getType() const { return type; }
	const std::string	&getAnimStat() const { return animStat; }
	const std::string	&getName() const { return name; }
	void				setName(const std::string &n) { name = n; }
	void				setType(const std::string &t) { type = t; }
	const size_t		&getCurrentFrame() { return currentFrame; }
	void				nextFrame();
	void				resetFrame() { currentFrame = startFrame; }
	void				configure(ServerInGameInfo const &);
	void				setPos(const subPosition &pos);
	const subPosition	&getPos() const { return pos; }
	const EntityPos		&getSize() const { return size; }
	void				testSprite();
};

#endif /*! AnimationAnimationComponent_H_*/