#include "stdafx.h"
#include "TextComponent.h"


void	TextComponent::configure(const std::string &name, const float &x, const float &y)
{
	content = name;
	pos.x = x;
	pos.y = y;
}
