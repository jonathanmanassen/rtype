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
		std::cerr << "BigBlueMissile constructor\n";
		name = "bigbluemissile";
		hp = 50;
		speed = 8;
		weapon = 0;
		ally = 0;
		filename = "r-typesheet1.png";

		addSprite(AnimationType::STATIC, 103, 170, 16, 81, 1, 0, 2);

		addHitbox(0, 0, 12, 17);
		sounds.emplace_back("piou");
	}

	~Monster()
	{
		std::cerr << "BigBlueMissile destructor\n";
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