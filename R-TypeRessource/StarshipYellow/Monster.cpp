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
		std::cerr << "Monster starship constructor\n";
		name = "StarshipYellow";
		hp = 100;
		speed = 1;
		weapon = 1;
		ally = 1;
		filename = "r-typesheet42.png";

		addSprite(AnimationType::DOWN, 33, 17, 15, 33, 1, 33, 2);
		addSprite(AnimationType::STATIC, 66, 17, 15, 33, 0, 0, 1);
		addSprite(AnimationType::UP, 99, 17, 15, 33, 1, 33, 2);

		addHitbox(22, 6, 11, 8);

		sounds.emplace_back("vroum");
		elements.emplace_back("missile");
		elements.emplace_back("diag_missile");
	}


	~Monster()
	{
		std::cerr << "Monster starship destructor\n";
	}

	void destroy()
	{
		delete this;
	}

	void ai(int frame)
	{
		moveupdown = static_cast<int>(5 * sin(frame));
		moveleftright = -1;
		pos.x += moveleftright;
		pos.y += moveupdown;

		if (frame % 10 == 0)
		{
			shoot = "missile";

			for (auto enemy : enemies)
			{
				int a = std::abs(enemy.x - pos.x);
				int b = std::abs(enemy.y - pos.y);

				if (a > b - 20 && a < b + 20)
				{
					shoot = "diag_missile";
					return;
				}
			}
		}
		else
			shoot = "";
	}
};

extern "C" __declspec(dllexport) IMonster* __cdecl create_monster()
{
	return new Monster;
}


