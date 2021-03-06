// starship1.cpp : définit les fonctions exportées pour l'application DLL.
//
#include "stdafx.h"
#include "IMonster.h"
#include <iostream>
#include <windows.h>
#include <list>
#include <vector>
#include "Hitbox.h"

class Monster : public IMonster {
public:
	Monster()
	{
		std::cerr << "Kamikaze constructor\n";
		name = "kamikaze";
		hp = 50;
		speed = 3;
		weapon = 1;
		ally = 0;
		filename = "r-typesheet23.png";

		addSprite(AnimationType::STATIC, 1, 6, 22, 33, 1, 33, 8);

		addHitbox(0, 0, 22, 33);

		sounds.emplace_back("vroum");
		elements.emplace_back("missile");
		elements.emplace_back("diag_missile");
	}


	~Monster()
	{
		std::cerr << "Kamikaze destructor\n";
	}

	void destroy()
	{
		delete this;
	}

	void ai(int frame)
	{
		moveleftright = -speed;
		pos.x += moveleftright;

		if (enemies.size() > 0)
		{
			int target = random % enemies.size();
		
			if (enemies.at(target).y > pos.y)
				pos.y += 1;
			else if (enemies.at(target).y < pos.y)
				pos.y += -1;
		}
		shoot = "";
	}
};

extern "C" __declspec(dllexport) IMonster* __cdecl create_monster()
{
	return new Monster;
}


