#include "stdafx.h"
#include "Ressources.h"
#include <iostream>
#include <mutex>
#ifdef WIN32
#include <direct.h>
#include <Windows.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

extern GameStat gameStat;
Ressources	*Ressources::ressources = nullptr;

Ressources	*Ressources::get()
{
	if (!ressources)
		ressources = new Ressources;
	return (ressources);
}

bool	Ressources::checkExtention(std::string fileName, std::string ext)
{
	size_t findPos;
	if ((findPos = fileName.find(ext)) != std::string::npos && findPos + ext.size() == fileName.size())
		return true;
	return false;
}

bool	Ressources::hasTexture(std::string const &fileName)
{
	std::unordered_map<std::string, sf::Texture>::iterator	it = textures.find(fileName);

	if (it == textures.end())
		return (0);
	return (1);
}

void Ressources::fileIdentitifation(std::string fileName)
{
	if (fileName.compare(".") == 0 || fileName.compare("..") == 0)
		return;
	if (checkExtention(fileName, ".png") && hasTexture(fileName) == false)
	{
		sf::Texture texture;
		std::string path = folderPath + "/" + fileName;
		if (!texture.loadFromFile(path))
		{
			std::cout << "ici marche pas" << std::endl;
			return;
		}
		textures.emplace(fileName, texture);
	}
}

Ressources::Ressources()
{
	//loadThread = std::thread(&Ressources::loadRessources, this);
}

void	Ressources::loadFolder(std::string const &_folderPath)
{
#ifdef WIN32
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	current_working_dir = buff;
	folderPath = _folderPath;
	current_working_dir += "\\" + folderPath + "\\*";
	WIN32_FIND_DATA data;
	std::cout << current_working_dir << std::endl;
	HANDLE hFind = FindFirstFile((current_working_dir).c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do
		{
			fileIdentitifation(data.cFileName);
			std::cout << data.cFileName << std::endl;
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
#endif

}

bool	Ressources::hasAnims(std::string const &name)
{
	std::unordered_map<std::string, AnimationSet>::iterator	it = anims.find(name);

	if (it == anims.end())
		return (0);
	return (1);
}

void	Ressources::loadRessources()
{
	while (gameStat != GameStat::GAME_END)
	{
		if (ressourcesToLoad.size() > 0)
		{
			for (ServerLoadRessourcesInfo info : ressourcesToLoad)
				actualiseAnimationSet(info);
			ressourcesToLoad.clear();
		}
	}
}

void Ressources::actualiseAnimationSet(ServerLoadRessourcesInfo const &info)
{
	if (hasAnims(info.entityName) != true)
	{
		AnimationSet set(this);
		set.configure(info);
		anims.emplace(info.entityName, set);
	}
	else if (anims.at(info.entityName).hasAnim(convertAnimTypeToString(info.type)) != true)
	{
		anims.at(info.entityName).configure(info);
	}
}

void	Ressources::setRessourcesToLoad(const std::vector<ServerLoadRessourcesInfo> &ressources)
{
	ressourcesToLoad = ressources;
}
sf::Sprite	const &Ressources::AnimationSet::getSprite(std::string const &name, const size_t &ite)
{
	if (ite < animSet.at(name).size())
		return animSet.at(name).at(ite);
	return animSet.at(name).at(0);
}

bool	Ressources::AnimationSet::hasAnim(std::string const &name)
{
	std::unordered_map<std::string, std::vector<sf::Sprite>>::iterator	it = animSet.find(name);

	if (it == animSet.end())
		return (0);
	return (1);
}

size_t		Ressources::AnimationSet::animSize(std::string const &name)
{
	if (hasAnim(name) == true)
	{
		return animSet.at(name).size();
	}
	return 0;
}

const sf::Sprite &Ressources::getAnimSprite(const std::string &name, const std::string &type, const size_t &frame)
{
//	std::cout << name << ":" << type << std::endl;
	if (hasAnims(name) == true &&
		anims.at(name).hasAnim(type) == true)
	{
//		std::cout << name << ":" << type << ":atFrame:" << frame << std::endl;
		return anims.at(name).getSprite(type, frame);
	}
	else if (hasAnims(name) == true &&
		anims.at(name).hasAnim("Static") == true)
	{
		return anims.at(name).getSprite("Static", 0);
	}
	errorSprite.setTexture(textures.at("errorSprite.png"));
	return errorSprite;
}

size_t				Ressources::getAnimSize(const std::string &name, const std::string &type)
{
	if (hasAnims(name) == true &&
		anims.at(name).hasAnim(type) == true)
	{
		return anims.at(name).animSize(type);
	}
	else if (hasAnims(name) == true &&
		anims.at(name).hasAnim("Static") == true)
	{
		return anims.at(name).animSize("Static");
	}
	return 1;
}

void Ressources::printAllType()
{
	for (const auto &pair : anims)
		std::cout << pair.first << ":";
	std::cout << std::endl;
}

void Ressources::AnimationSet::configure(ServerLoadRessourcesInfo const &info)
{
	sf::Sprite tmp;
	std::vector<sf::Sprite> vecSprite;
	if (res->hasTexture(info.fileName) == false)
	{
		vecSprite.emplace_back(res->errorSprite);
		animSet.emplace(convertAnimTypeToString(info.type), vecSprite);
		return;
	}
	tmp.setTexture(res->textures.at(info.fileName));
	int	x = info.beginX;
	int	y = info.beginY;
	if (info.scale == true)
		tmp.scale(sf::Vector2f(2.5f, 2.5f));
	for (unsigned int i = 0; i < info.nbSprite; i++)
	{
		tmp.setTextureRect(sf::IntRect(x, y, info.width, info.height));
		x += info.incX;
		vecSprite.push_back(tmp);
	}
	animSet.emplace(convertAnimTypeToString(info.type), vecSprite);
}