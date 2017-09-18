#pragma once

#include <array>
#include <cstdlib>
#include <cmath>

template<class T>
class perlinNoiseGenerator
{
public:
	int x;
	int amplitude;
	int wavelength;

	int a,b;

	perlinNoiseGenerator(int amp, int wl)
	{
		x = 0;
		amplitude = amp;
		wavelength = wl;
		a = std::rand()%0xFF;
		b = std::rand()%0xFF;
	}

	T update()
	{
		int y = 0;
		if(x%wavelength == 0)
		{
			a = b;
			b = std::rand()%0xFF;
			y=interpolate(a,b,(x%wavelength)/(float)wavelength)*amplitude;
		}
		return y;
	}

	float interpolate(float pa, float pb, float px)
	{
		float ft = px*3.1415;
		float f = (1-std::cos(ft))*0.5;
		return (pa*(1-f)+pb*f);
	}
};
