#pragma once

#include <animation.h>
#include <array>
#include <Color.h>
#include <vector>
#include <circularBuffer.h>

template<uint8_t numLed, unsigned int trailLength>
class trailSpinner : public animation
{
	float speed = 1;
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
		if((currentPos - trail.getHead()) >= 1)
		{
			trail.push((int)currentPos);
		}

		for(int i = 0; i<trail.size(); i++)
		{
			Color c = color;
			c.setBrightness(1-i/trailLength);
			frame[trail.getElementAt(i)] = c;
		}

		return 0;
	}
};
