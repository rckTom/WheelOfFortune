#pragma once

#include <cstdint>

class Particle
{
	float force;
	float vel;
	float pos;

public:
	void setForce(float force)
	{
		this->force = force;
	}

	void setVel(float vel)
	{
		this->vel = vel;
	}

	void update(float dt)
	{
		vel = vel+force*dt;
		pos = pos+vel*dt;
	}

	void reset()
	{
		vel = 0;
		pos = 0;
	}

	float getPos()
	{
		return pos;
	}

	float getVel()
	{
		return vel;
	}

};
