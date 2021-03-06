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
		std::cerr << "Butterfly constructor\n";
		name = "butterfly";
		hp = 20;
		speed = 2;
		weapon = 1;
		ally = 0;
		filename = "r-typesheet5.png";

		addSprite(AnimationType::STATIC, 5, 4, 28, 33, 1, 33, 8);

		addHitbox(4, 0, 24, 25);

		sounds.emplace_back("vroum");
		elements.emplace_back("missile");
		elements.emplace_back("diag_missile");
	}


	~Monster()
	{
		std::cerr << "Butterfly destructor\n";
	}

	void destroy()
	{
		delete this;
	}

	void ai(int frame)
	{
		moveupdown = 3 * sin(frame * 0.07 + random);
		moveleftright = -speed;
		pos.x += moveleftright;
		pos.y += moveupdown;

		if ((frame + random) % 200 == 0)
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


