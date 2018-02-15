#include <iostream>
#include "stdafx.h"
#include "ScrollComponent.h"
#include "Ressources.h"

void				ScrollComponent::configure(ServerInGameInfo const &info)
{
	name = "Scroll";
	type = info.entityType;
	width = 3000;
	secondScroll = width;
}

void				ScrollComponent::incrementRotation()
{
	scrollRotationRatio += scrollRotationInc;
	if (scrollRotationRatio += 360)
		scrollRotationRatio -= 360;
}

void				ScrollComponent::incrementScroll()
{
	firstScroll -= scrollSpeed;
	secondScroll -= scrollSpeed;
	if (firstScroll < -width)
		firstScroll = width;
	if (secondScroll < -width)
		secondScroll = width;
}