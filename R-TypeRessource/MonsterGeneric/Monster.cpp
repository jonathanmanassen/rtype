#include "stdafx.h"
#include "IMonster.h"
#include <iostream>
#include <windows.h>
#include <list>
#include <vector>
#include <cmath>
#include "Hitbox.h"

class Monster : public IMonster {
public:
	Monster()
	{
		std::cerr << "Monster constructor\n";
		name = "MonsterGeneric";
		hp = 100;
		speed = 5;
		weapon = 1;
		ally = 0;
		filename = "r-typesheet42.png";

		addSprite(AnimationType::STATIC, 0, 66, 33, 18, 0, 0, 1);
		


		addHitbox(1, 4, 32, 14);
		sounds.emplace_back("piou");
		sounds.emplace_back("shprout");
		elements.emplace_back("missile1");
		
	}

	~Monster()
	{
		std::cerr << "Monster destructor\n";
	}

	void destroy()
	{
		delete this;
	}

	void ai(int frame)
	{
		moveupdown = static_cast<int>(1 * sin(frame));
		moveleftright = -5;

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