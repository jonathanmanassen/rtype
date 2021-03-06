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
		std::cerr << "Missile constructor\n";
		name = "missile";
		hp = 20;
		speed = 8;
		weapon = 0;
		ally = 0;
		filename = "r-typesheet43.png";

		addSprite(AnimationType::STATIC, 134, 4, 10, 11, 1, 15, 8);

		addHitbox(5, 5, 6, 7);
		sounds.emplace_back("piou");
	}

	~Monster()
	{
		std::cerr << "Missile destructor\n";
	}

	void destroy()
	{
		delete this;
	}

	void ai(int frame)
	{
		if (ally == 1)
			moveleftright = speed;
		else
			moveleftright = -speed;
		pos.x += moveleftright;
	}
};

extern "C" __declspec(dllexport) IMonster* __cdecl create_monster()
{
	return new Monster;
}