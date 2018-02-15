#pragma once

#ifndef IMONSTER_H_
#define IMONSTER_H_

#include <list>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Hitbox.h"

class IMonster {
public:
	IMonster() {}
	~IMonster() {}
	virtual void	destroy() = 0;
	void addHitbox(const int &x, const int &y, const int &height, const int &width)
	{
		Hitbox	tmp(x, y, height, width);
		hitboxes.emplace_back(tmp);
	}
	void addSprite(const AnimationType &type,
		const int &x,
		const int &y,
		const int &height,
		const int &width,
		const int &loop = 0,
		const int &incX = 0,
		const int &nb_frame = 0)
	{
		Hitbox	tmp(x, y, height, width, loop, incX, nb_frame);
		sprites.emplace(type, tmp);
	}

	virtual void ai(int frame) = 0;

	std::string	name;
	int		hp = 0;
	int		speed;
	int		weapon;
	int		nbframe;
	int		ally;
	std::string			filename;
	std::list<Hitbox>	hitboxes;
	std::unordered_map<AnimationType, Hitbox>	sprites;

	std::vector<std::string>	sounds;
	std::vector<std::string>	elements;
	Hitbox	pos;
	std::vector<Hitbox>	enemies;

	int		moveupdown = 0;
	int		moveleftright = 0;
	std::string		shoot = "";
	int		random = 0;
};

#endif // IMONSTER_H_