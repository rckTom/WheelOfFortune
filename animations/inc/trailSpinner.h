#pragma once

#include <animation.h>
#include <array>
#include <Color.h>
#include <vector>

template<uint8_t numLed>
class trailSpinner : public animation
{
	std::array<Color,numLed> &frame;


public:
	Color color = Color{0,0,0xFF};
	float speed = numLed/10000.0;
	float startAngle = 0;
	float trailLength = 10.0;
	float angle;

	trailSpinner(std::array<Color,numLed> &frameBuffer) :
		frame(frameBuffer){
		angle = 0;
		startAngle = 0;
		speed = numLed/500.0;
	};

	uint32_t update(uint32_t dt)
	{
		angle += speed*dt;
		angle = std::fmod(angle,numLed);
		for(int i = 0; i<numLed; i++)
		{
			if(i<angle & i>angle-trailLength)
			{
				float b = (angle-i)/trailLength;
				Color c = Color::fromHsv(150,1,1-b);
				frame[i] = c;
			}
			if((angle-trailLength) < 0)
			{
				if(i > angle-trailLength+numLed)
				{
					float b = (angle-i+numLed)/trailLength;
					Color c = Color::fromHsv(150,1,1-b);
					frame[i] = c;
				}
			}
			else
			{
				//frame[i] = Color{};
			}
		}
	}

	void reset()
	{
		angle = 0;
	}
};
