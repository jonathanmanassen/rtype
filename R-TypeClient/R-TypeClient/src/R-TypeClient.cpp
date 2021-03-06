// R-TypeClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Ressources.h"
#include "SystemGUI.h"
#include "Network.h"
#include "SystemManager.h"

GameStat gameStat = GameStat::CONNEXION;

int main()
{
	try {
		SystemManager manager;
		manager.execLoop();
	}
	catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}