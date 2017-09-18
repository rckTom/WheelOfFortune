#pragma once

class animation
{
public:
	virtual uint32_t update(uint32_t dt);
	virtual ~animation() {};
	//virtual ~animation();
};
