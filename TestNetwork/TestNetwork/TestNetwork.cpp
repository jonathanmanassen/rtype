// TestNetwork.cpp : Définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <iostream>
#include "TCPServer.h"
#include "GameRoomManager.h"

int main(int argc, char **argv)
{
	try
	{
		//std::vector<std::shared_ptr<Server>>	vector;
		//vector.push_back(std::make_shared<Server>(4242));
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
