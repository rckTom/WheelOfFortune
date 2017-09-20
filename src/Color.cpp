#include <color.h>
#include <algorithm>

Color Color::fromHsv(float h, float s, float v)
{
	int hi = h/60;
	float f = h/60.0-hi;
	float p = v*(1-s);
	float q = v*(1-s*f);
	float t = v*(1-s*(1-f));

	float r, g, b;

	switch(hi)
	{
	case 1:
		r = q;
		g = v;
		b = p;
		break;
	case 2:
		r = p;
		g = v;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = v;
		break;
	case 4:
		r = t;
		g = p;
		b = v;
		break;
	case 5:
		r = v;
		g = p;
		b = q;
		break;
	default:
		r = v;
		g = t;
		b = p;
		break;
	}

	return Color(r*0xFF,g*0xFF,b*0xFF);
}

void Color::setBrightness(float bright)
{
	auto hsvValues = toHSV(*this);
	hsvValues.v = bright;
	auto rgb = toRGB(hsvValues);
	r = rgb.r;
	g = rgb.g;
	b = rgb.b;
}

hsvValue Color::toHSV(const Color& color)
{
	uint8_t max,min;
	max = std::max(std::max(color.r,color.g),color.b);
	min = std::min(std::min(color.r,color.g),color.b);

	auto ret = hsvValue{};
	if(max == min)
	{
		ret.h = 0;
	}
	else if(max == color.r)
	{
		ret.h = 60.0*((color.g-color.b)/((float)(max-min)));
	}
	else if(max == color.g)
	{
		ret.h = 60.0*(2.0+(color.b-color.r)/((float)(max-min)));
	}
	else if(max == color.b)
	{
		ret.h = 60.0*(4.0+(color.r-color.g)/((float)(max-min)));
	}

	ret.h = ret.h<0?ret.h+360:ret.h;

	if(max == 0)
	{
		ret.s = 0;
	}
	else
	{
		ret.s = (max-min)/(float)max;
	}

	ret.v = max/256.0;

	return ret;
}

rgbValue Color::toRGB(const hsvValue& hsv)
{
	int hi = hsv.h/60;
	float f = hsv.h/60.0-hi;
	float p = hsv.v*(1-hsv.s);
	float q = hsv.v*(1-hsv.s*f);
	float t = hsv.v*(1-hsv.s*(1-f));

	float r, g, b;

	switch(hi)
	{
	case 1:
		r = q;
		g = hsv.v;
		b = p;
		break;
	case 2:
		r = p;
		g = hsv.v;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = hsv.v;
		break;
	case 4:
		r = t;
		g = p;
		b = hsv.v;
		break;
	case 5:
		r = hsv.v;
		g = p;
		b = q;
		break;
	default:
		r = hsv.v;
		g = t;
		b = p;
		break;
	}

	auto ret = rgbValue{};
	ret.r = r;
	ret.g = g;
	ret.b = b;

	return ret;
}

Color ColorMap::fire(uint8_t value)
{
	  uint8_t R = std::min(value,(uint8_t)85) * 3;
	  value = std::max(value-85,0);
	  uint8_t G = std::min(value,(uint8_t)85) * 3;
	  value = std::max(value-85,0);
	  uint8_t B = std::min(value,(uint8_t)85) * 3;
	  return Color{R,G,B};
}
