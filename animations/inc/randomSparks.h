#pragma once

#include <animation.h>
#include <array>
#include <timer.h>
#include <Color.h>
#include <cstdlib>


template<uint8_t numLeds>
struct randomSparks :  public animation
{
	randomSparks(std::array<Color,numLeds> &frameBuffer,float density, Color color) :
		frequency(density),
		color(color),
		frame(frameBuffer){};
	std::array<Color,numLeds> &frame;
	uint32_t update(uint32_t dt)
	{
		for(auto& el: frame)
		{
			if(std::rand()%(int)frequency == 0)
			{
				el = color;
			}
			else
			{
				el = Color();
			}
		}
		return 0;
	}

public:
	Color color;
	float frequency;
	timer tim;
};
