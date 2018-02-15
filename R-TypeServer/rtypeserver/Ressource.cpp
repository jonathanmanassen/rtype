#include "stdafx.h"
#include "Ressource.h"
#include <Windows.h>
#include <memory>
#include "IMonster.h"
#include <iostream>

typedef IMonster* (__cdecl *imonster_factory)();

Ressource::Ressource(const int &level)
{
	std::cout << "construct ressource level " << level << std::endl;
	loadAll(level);
}

Ressource::~Ressource()
{
	for (auto tmp : monsters)
		tmp.second->destroy();
	for (auto tmp : dll_handles)
		FreeLibrary(tmp);
}

#ifdef _WIN32

int	Ressource::load(std::string filename, const int &level)
{
	//std::string s("C:\\Users\\Christophe Mei\\source\\repos\\cpp_rtype\\R-TypeRessource\\x64\\Debug\\");
	std::string s("..\\..\\R-TypeRessource\\level");
	s += std::to_string(level) + "\\" + filename;
	if (s.find(".dll") == std::string::npos)
		s += ".dll";
	std::wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR path = stemp.c_str();

	std::cout << s << std::endl;

	HINSTANCE dll_handle = LoadLibrary(path);
	if (!dll_handle) {
		std::cerr << "Unable to load DLL!" << std::endl;
		return 1;
	}

	imonster_factory factory_func = reinterpret_cast<imonster_factory>(
		GetProcAddress(dll_handle, "create_monster"));
	if (!factory_func) {
		std::cerr << "Unable to load create_monster from DLL!" << std::endl;
		FreeLibrary(dll_handle);
		return 1;
	}
	IMonster *instance = factory_func();
	if (filename.find(".dll") != std::string::npos)
		filename = filename.substr(0, filename.find(".dll"));
	monsters.emplace(filename, instance);
	dll_handles.emplace_back(dll_handle);
	return (0);
}

#endif //_WIN32

#ifdef __unix__
int	Ressource::load(std::string name)
{
	std::cout << "unix" << std::endl;
	return (0);
}
#endif //__unix__

int Ressource::loadAll(const int &level)
{
	//std::string pattern("..\\..\\R - TypeRessource\\x64\\Debug");
	//pattern.append("\\*");
	WIN32_FIND_DATA data;
	HANDLE hFind;
	std::string tmppath("..\\..\\R-TypeRessource\\level");
	tmppath = tmppath + std::to_string(level) + "\\*.dll";
	std::wstring stemp = std::wstring(tmppath.begin(), tmppath.end());
	LPCWSTR path = stemp.c_str();

	std::cout << tmppath << std::endl;

	if ((hFind = FindFirstFile(path, &data)) != INVALID_HANDLE_VALUE) {
		do {
			char* tmp = new char[260];

			sprintf_s(tmp, 260, "%ws", data.cFileName);
			std::string name(tmp);
			load(name, level);
		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}
	return (0);
}

void	Ressource::strcmp(char dist[30], const std::string &str)
{
	int i = 0;
	while (i < 29 && str[i] != 0)
	{
		dist[i] = str[i];
		i++;
	}
	dist[i] = 0;
}

void Ressource::getRessourcesInfo(std::vector<ServerLoadRessourcesInfo> &list)
{
	for (auto &monster : monsters)
	{
		for (auto &sprite : monster.second->sprites)
		{
			ServerLoadRessourcesInfo	info;
			strcmp(info.entityName, monster.second->name);
			strcmp(info.fileName, monster.second->filename);
			info.type = sprite.first;
			info.beginX = sprite.second.x;
			info.beginY = sprite.second.y;
			info.height = sprite.second.height;
			info.width = sprite.second.width;
			info.nbSprite = sprite.second.nb_frame;
			info.incX = sprite.second.inc_x;
			list.emplace_back(info);
		}
	}
}

IMonster	*Ressource::getMonster(const std::string &name)
{
	std::unordered_map<std::string, IMonster*>::iterator	it = monsters.find(name);

	if (it == monsters.end())
		return (monsters["missile"]);
	return (monsters[name]);
}

bool	Ressource::hasAnimationType(std::string name, AnimationType type)
{
	IMonster *monster = monsters.at(name);
	std::unordered_map<AnimationType, Hitbox>::iterator	it = monster->sprites.find(type);

	if (it == monster->sprites.end())
		return (0);
	return (1);
}