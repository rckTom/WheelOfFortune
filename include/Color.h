#pragma once

#include <cstdint>

struct hsvValue
{
	float h,s,v;
};

struct rgbValue
{
	uint8_t r,g,b;
};

class Color
{


public:
	uint8_t r,g,b;
	Color()
	{
		r = 0;
		g = 0;
		b = 0;
	}

	Color(uint8_t r, uint8_t g, uint8_t b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	void setBrightness(float b);

	static Color fromHsv(float h, float s, float v);
	static hsvValue toHSV(const Color &color);
	static rgbValue toRGB(const Color &color);
	static rgbValue toRGB(const hsvValue &hsv);

	uint32_t getAsGRBValue()
	{
		return (g<<16 | r<<8 | b);
	}

	uint32_t getAsBGValue()
	{
		return (b<<16 | r<<8 | g);
	}

	uint32_t getAsBGRValue()
	{
		return (b<<16 | g<<8 | r);
	}

	uint32_t getAsRGBValue()
	{
		return (r<<16 | g<<8 | b);
	}
};

class ColorMap
{
public:
	static Color fire(uint8_t value);
};
