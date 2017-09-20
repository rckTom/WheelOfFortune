#pragma once

#include <animation.h>
#include <array>
#include <timer.h>
#include <Color.h>
#include <cstdlib>


template<uint8_t numLeds>
struct randomSparks :  public animation
{
	std::array<Color,numLeds> &frame;
	Color color;
	float frequency;

	randomSparks(std::array<Color,numLeds> &frameBuffer,float density, Color color) :
		frame(frameBuffer),
		color(color),
		frequency(density)
		{};

	uint32_t update(uint32_t)
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


};
