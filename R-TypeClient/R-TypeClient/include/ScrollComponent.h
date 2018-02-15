#ifndef ScrollComponent_H_
# define ScrollComponent_H_

#include <string>
#include <vector>
#include <SFML\Graphics.hpp>
#include "Protocol.h"
#include "Component.h"

class ScrollComponent : public Component
{
	std::string	type;
	std::string	name;
	float		scrollSpeed = 0;
	float		scrollRotationRatio = 0;
	float		scrollRotationInc = 0;
	float		firstScroll = 0;
	float		secondScroll = 0;
	float		width = 0;
public:
	const std::string	&getType() const { return type; }
	const std::string	&getName() const { return name; }
	void				setName(const std::string &n) { name = n; }
	void				setType(const std::string &t) { type = t; }
	void				configure(ServerInGameInfo const &);
	void				setScrollSpeed(const float &s) { scrollSpeed = s; }
	void				setScrollRotationInc(const float &r) { scrollRotationInc = r; }
	const float			&getScrollSpeed() { return scrollSpeed; }
	const float			&getScrollRotationRatio() { return scrollRotationRatio; }
	void				incrementRotation();
	const float			&getFirstScrollInc() { return firstScroll; }
	const float			&getSecondScrollInc() { return secondScroll; }
	void				incrementScroll();
};

#endif /*! ScrollScrollComponent_H_*/