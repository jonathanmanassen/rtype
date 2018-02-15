#ifndef RESSOURCES_H_
# define RESSOURCES_H_

#include <string>
#include <unordered_map>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <thread>
#include "Protocol.h"

class Ressources
{
	class AnimationSet
	{
		Ressources *res;
		std::unordered_map<std::string, std::vector<sf::Sprite>> animSet;
	public:
		AnimationSet(Ressources *_res) : res(_res) {}
		bool	hasAnim(std::string const &name);
		sf::Sprite	const &getSprite(std::string const &name, const size_t &ite);
		void	configure(ServerLoadRessourcesInfo const &);
		size_t		animSize(std::string const &);
	};
	std::string current_working_dir;
	//std::thread loadThread;
	std::string folderPath;
	sf::Sprite	errorSprite;
	static Ressources *ressources;
	winSize	winS;
	std::vector<ServerLoadRessourcesInfo> ressourcesToLoad;
	Ressources();
//	bool	loadingData;
public:
	static Ressources *Ressources::get();
	void loadFolder(std::string const &folderPath);
	void fileIdentitifation(std::string fileName);
	bool checkExtention(std::string fileName, std::string ext);
	std::unordered_map<std::string, sf::Texture> textures;
	std::unordered_map<std::string, sf::Sound>	sounds;
	std::unordered_map<std::string, AnimationSet> anims;
	size_t			getAnimSize(const std::string &name, const std::string &type);
	void actualiseAnimationSet(ServerLoadRessourcesInfo const &);
	const sf::Sprite &getAnimSprite(std::string const&, std::string const&, const size_t &);
	bool	hasAnims(std::string const &name);
	const winSize	&getWinSize() { return winS; }
	void			setWinSize(const int &w, const int &h) { winS.height = h; winS.width = w; }
	void			setRessourcesToLoad(const std::vector<ServerLoadRessourcesInfo> &ressources);
	void			loadRessources();
	bool			hasTexture(std::string const &fileName);
	void			printAllType();
};

#endif /*! RESSOURCES_H_*/