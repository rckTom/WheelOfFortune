#pragma once
#include <animation.h>
#include <deque>

template<uint8_t numLeds>
class pong1d : public animation
{
	float speed;
	float pos;

	float player1HarborWidth;
	float player2HarborWidth;
	bool trail;

	Color backGroundColor;
	Color player1Color;
	Color player2Color;

	uint32_t update(uint32_t dt)
	{

	}

	int checkCollision()
	{

	}

public:
	pong1d()
	{

	}
};
