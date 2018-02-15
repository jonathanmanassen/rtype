#include "stdafx.h"
#include "CountComponent.h"


void	CountComponent::configure(const std::vector<std::string> &name, const float &x, const float &y)
{
	content = name;
	pos.x = x;
	pos.y = y;
}
