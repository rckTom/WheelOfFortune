#pragma once

#include <animation.h>
#include <array>
#include <Color.h>
#include <vector>
#include <circularBuffer.h>

template<uint8_t numLed, unsigned int trailLength>
class trailSpinner : public animation
{
	float speed = numLed/500.0;
	float currentPos = 0.0;

	std::array<Color,numLed> &frame;
	circularBuffer<int,trailLength> trail;


public:
	Color color{0xFF,0xFF,0xFF};

	void setSpeed(float speed)
	{
		this->speed = speed;
	}

	void setAngle(float angle)
	{
		currentPos = angle;
		trail.clear();
	}

	trailSpinner(std::array<Color,numLed> &frameBuffer,float startAngle) : frame(frameBuffer)
	{
		currentPos = startAngle;
	}

	trailSpinner(std::array<Color,numLed> &frameBuffer) : frame(frameBuffer)
	{
		currentPos = 0;
	}

	uint32_t update(uint32_t dt)
	{
		currentPos += speed*dt;
		currentPos = std::fmod(currentPos,numLed);
		if(std::abs(currentPos - trail[0]) >= 1
				)
		{
			trail.push_back((int)currentPos);
		}

		for(std::size_t i = 0; i<trail.size(); i++)
		{
			Color c = color;
			c.setBrightness(1.0-(float)i/(float)trailLength);
			frame[trail[i]] = c;
		}

		return 0;
	}
};
