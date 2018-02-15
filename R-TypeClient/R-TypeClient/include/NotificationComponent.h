#ifndef NotificationComponent_H_
# define NotificationComponent_H_

#include <string>
#include <vector>
#include <SFML\Graphics.hpp>
#include "Protocol.h"
#include "Component.h"

class NotificationComponent : public Component
{
	std::string	type;
	std::string	name;
	std::string content;
	float		NotificationSpeed = 0;
	bool		hasNotification = false;
	bool		isIncrementing = false;
	float		incNotification;
	float		width;
public:
	const std::string	&getType() const { return type; }
	const std::string	&getName() const { return name; }
	void				setName(const std::string &n) { name = n; }
	void				setType(const std::string &t) { type = t; }
	void				newNotification(const std::string &c) { hasNotification = true; isIncrementing = true; content = c; incNotification = 0; }
	const std::string	&getContent() { return content; }
	void				configure(ServerInGameInfo const &);
	void				setNotificationSpeed(const float &s) { NotificationSpeed = s; }
	const float			&getNotificationSpeed() { return NotificationSpeed; }
	const float			&getNotificationInc() { return incNotification; }
	void				incrementNotification();
};

#endif /*! NotificationNotificationComponent_H_*/