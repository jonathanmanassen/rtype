#pragma once
#ifndef Protocol_H_
# define Protocol_H_

enum class ControllerDirection
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
	NONE
};

struct ControllerAction
{
	ControllerDirection dir;
	bool				isShooting;
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
	char				name[30];
	uint32_t			roomNb;
	uint32_t			level;
};

struct ServerInGameInfo
{
	char	entityName[30];
	int		beginX;
	int		beginY;
	int		height;
	int		width;
	int		posX;
	int		posY;
};

enum class RoomStat
{
	WAITING,
	ALL_READY,
	LEAVING,
};

struct ServerInRoomInfo
{
	int		playerNb = 1;
	char	playersName[4][30] { "", "", "", "" };
	bool	ready[4] = { false, false, false, false };
	bool	loaded[4] = { false, false, false, false };
	bool	startGame = true;
	bool	joined = true;
};

struct ServerRoomsInfo
{
	int		roomNb;
	int		nbPlayer;
	char	ownerName[30];
	int		lvlNb;
};

#endif /*!Protocol_H_*/