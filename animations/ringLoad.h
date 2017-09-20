#pragma once

#include <animation.h>
#include <array>
#include <Color.h>
#include <vector>

template<uint8_t numLed>
class ringLoad : public animation
{
	std::array<Color,numLed> &frame;


public:
	Color color = Color{0,0,0xFF};
	float speed = numLed/10000.0;
	float endAngle = numLed;
	float startAngle = 0;
	float angle;
	bool enableDithering = true;

	ringLoad(std::array<Color,numLed> &frameBuffer) :
		frame(frameBuffer){
		angle = 0;
		enableDithering = true;
		startAngle = 0;
		endAngle = numLed;
		speed = numLed/500.0;
	};

	uint32_t update(uint32_t dt)
	{
		if(angle > numLed)
			reset();

		angle += speed*dt;
		for(int i = 0; i<numLed; i++)
		{
			if(i < angle)
			{
					frame[i] = color;
			}
			else
			{
				if(i-angle < 1)
				{
					float brightness = 1-(i-angle);
					Color b = color;
					b.setBrightness(brightness);
					frame[i] = b;
				}
				else
				{
					//frame[i] = Color{};
				}
			}
		}
		return 0;
	}

	void reset()
	{
		angle = 0;
	}
};
