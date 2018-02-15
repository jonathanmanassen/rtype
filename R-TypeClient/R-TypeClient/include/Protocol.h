#ifndef Protocol_H_
# define Protocol_H_

struct winSize
{
	int height = 1080;
	int width = 1920;
};

enum class GameStat
{
	CONNEXION,
	CONNECTED,
	IN_ROOM,
	IN_GAME,
	GAME_END,
};

struct ControllerAction
{
	bool				right = false;
	bool				left = false;
	bool				up = false;
	bool				down = false;
	bool				isShooting = false;
	uint32_t			id;
};

enum class ConnectionAction
{
	HELLO = 0,
	CREATE_ROOM = 1,
	ENTER_ROOM = 2,
	LEAVE_ROOM = 3,
	READY = 4,
	LOADED = 5,
};

struct ConnectionMessage
{
	ConnectionAction	act;
	char			name[30];
	uint32_t		roomNb;
	uint32_t		level;
};

enum class AnimationType
{
	ATTACK = 0,
	UP,
	DOWN,
	RIGHT,
	LEFT,
	STATIC,
};

#include <iostream>
static std::string	convertAnimTypeToString(AnimationType const &type)
{
	std::string tmp = "";
	switch (type)
	{
	case AnimationType::ATTACK:
		tmp += "Attack";
		break;
	case AnimationType::UP:
		tmp += "Up";
		break;
	case AnimationType::DOWN:
		tmp += "Down";
		break;
	case AnimationType::RIGHT:
		tmp += "Right";
		break;
	case AnimationType::LEFT:
		tmp += "Left";
		break;
	default:
		tmp += "Static";
		break;
	}
	return tmp;
}

struct ServerLoadRessourcesInfo
{
	char	entityName[30];
	char	fileName[30];
	AnimationType type;
	uint32_t		beginX;
	uint32_t		beginY;
	uint32_t		height;
	uint32_t		width;
	uint32_t		nbSprite;
	uint32_t		incX;
	bool			scale = true;
};

struct ServerInGameInfo
{
	uint32_t		entityId;
	char			entityType[30];
	AnimationType	type;
	uint32_t		posX;
	uint32_t		posY;
	uint32_t		loopMode;
};

enum class RoomStat
{
	WAITING = 0,
	ALL_READY,
	LEAVING,
};

struct ServerInRoomInfo
{
	uint32_t		playerNb = 1;
	char	playersName[4][30]{ "", "", "", "" };
	bool	ready[4] = { false, false, false, false };
	bool	loaded[4] = { false, false, false, false };
	bool	startGame = true;
	bool	joined = true;
};

struct ServerRoomsInfo
{
	uint32_t		roomNb;
	uint32_t		nbPlayer;
	char			ownerName[30];
	uint32_t		lvlNb;
};

#endif /*!Protocol_H_*/