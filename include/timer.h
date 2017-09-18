#pragma once
#include <cstdint>

class timer
{

	volatile static uint32_t value;

public:
	timer();
	void sleep(uint32_t ms);
	uint32_t getEllapsedMs();
	static void timer_tick();
};
