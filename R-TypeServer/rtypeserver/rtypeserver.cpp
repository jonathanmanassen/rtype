#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "TCPServer.h"
#include "GameRoomManager.h"
#include "Game.h"
#include "UDPServer.h"

int main()
{
	try
	{
		UDPServer::get();
		TCPServer server(4242);

		server.start();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	GameRoomManager::get()->destroy();
	return 0;
}
