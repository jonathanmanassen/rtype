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
		std::cerr << "Greenboss constructor\n";
		name = "greenboss";
		hp = 5000;
		speed = 3;
		weapon = 1;
		ally = 0;
		filename = "r-typesheet17.png";

		addSprite(AnimationType::STATIC, 0, 0, 132, 65, 1, 65, 8);

		addHitbox(0, 0, 132, 65);

		sounds.emplace_back("vroum");
		elements.emplace_back("missile");
		elements.emplace_back("diag_missile");
	}


	~Monster()
	{
		std::cerr << "Greenboss destructor\n";
	}

	void destroy()
	{
		delete this;
	}

	void ai(int frame)
	{
		moveupdown = 4.5 * sin(frame * 0.01);
		pos.y += moveupdown;

		moveleftright = 1.5 * sin(frame * 0.003);
		pos.x += moveleftright;

		if ((frame + random) % 15 == 0)
		{
			shoot = "missile";
		}
		else
			shoot = "";
	}
};

extern "C" __declspec(dllexport) IMonster* __cdecl create_monster()
{
	return new Monster;
}


