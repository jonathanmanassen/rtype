#pragma once
#include "Protocol.h"

#ifndef HITBOX_H_
#define HITBOX_H_

class Hitbox
{
public:
	Hitbox() {}
	Hitbox(int _x, int _y, int _height = 0, int _width = 0, int _loop = 0, int _inc_x = 0, int _nb_frame = 0) {
		x = _x;
		y = _y;
		height = _height;
		width = _width;
		loop = _loop;
		inc_x = _inc_x;
		nb_frame = _nb_frame;
	}
	void	change(int _x, int _y, int _height = 0, int _width = 0, int _loop = 0, int _inc_x = 0, int _nb_frame = 0) {
		x = _x;
		y = _y;
		height = _height;
		width = _width;
		loop = _loop;
		inc_x = _inc_x;
		nb_frame = _nb_frame;
	}
	int x;
	int y;
	int height;
	int width;
	int	loop;
	int inc_x;
	int nb_frame;
	AnimationType type;
};

#endif // HITBOX_H_