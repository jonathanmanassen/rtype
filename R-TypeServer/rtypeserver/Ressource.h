#pragma once

#include <string>
#include <memory>
#include <Windows.h>
#include "IMonster.h"
#include "Protocol.h"
#include <vector>

class Ressource
{
	std::list<HINSTANCE> dll_handles;

public:
	Ressource() {}
	Ressource(const int &level);
	~Ressource();
	std::unordered_map<std::string, IMonster*>	monsters;

	int			load(std::string, const int&);
	int			loadAll(const int&);
	IMonster	*getMonster(const std::string &name);
	void getRessourcesInfo(std::vector<ServerLoadRessourcesInfo>&);
	void strcmp(char dist[30], const std::string &str);
	bool	Ressource::hasAnimationType(std::string name, AnimationType type);
};

