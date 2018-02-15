#include <iostream>
#include "stdafx.h"
#include "NotificationComponent.h"
#include "Ressources.h"

void				NotificationComponent::configure(ServerInGameInfo const &info)
{
	name = "Notification";
	type = info.entityType;
	width = Ressources::get()->anims.at(info.entityType).getSprite(convertAnimTypeToString(info.type), 0).getGlobalBounds().width;
}

void				NotificationComponent::incrementNotification()
{
	if (hasNotification == true)
	{
		if (isIncrementing == true)
			incNotification -= NotificationSpeed;
		else
			incNotification += NotificationSpeed;
//		std::cout << incNotification << std::endl;
		if (incNotification > 0)
			hasNotification = false;
		if (incNotification < -width)
			isIncrementing = false;
	}
}