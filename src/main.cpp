/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stm32f30x.h>
//#include "diag/Trace.h"
#include <ws2811Driver.h>
#include <Color.h>
#include <timer.h>
#include <cmath>
#include <array>
#include <algorithm>
#include <Particle.h>
#include <noise.h>
#include <circularBuffer.h>

#include "../animations/randomSparks.h"
#include "../animations/ringLoad.h"
#include "../animations/trailSpinner.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F3 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.

static constexpr uint8_t numLeds = 48;

void setupClock()
{
	FLASH->ACR |= FLASH_ACR_LATENCY_0;
	RCC->CR |= RCC_CR_HSION;

	while(!(RCC->CR & RCC_CR_HSIRDY))
	{
		__asm__("nop");
	}

	//Setup PLL
	RCC->CFGR |= RCC_CFGR_PLLMULL9;
	RCC->CFGR |= (uint32_t)0x80000; //PLL Source
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_HPRE_DIV1;

	//Switch on pll and wait for ready flag
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY))
	{
		__asm__("nop");
	}

	//select pll as system clock
	RCC->CFGR |= RCC_CFGR_SW_PLL;
}

void
main()
{
 	setupClock();
	timer tim;
	ws2811Driver<48> ledChain;

	circularBuffer<int,10> circBuff{};

	for(int i = 0; i<100; i++)
	{
		circBuff.push(i);
	}

	for(int i = 0; i<10; i++)
	{
		circBuff.pop();
	}


	auto frameBuffer = std::array<Color,numLeds>{};
	auto sparklingAnim = randomSparks<numLeds>{frameBuffer,16,Color{0xFF,0xFF,0xFF}};
	auto ringLoadAnim = ringLoad<numLeds>{frameBuffer};
	auto trailSpinnerAnim =trailSpinner<numLeds,6>{frameBuffer};
	auto trailSpinnerAnim2 =trailSpinner<numLeds,6>{frameBuffer,16};
	auto trailSpinnerAnim3 = trailSpinner<numLeds,6>{frameBuffer,32};
	ledChain.setFrame(frameBuffer);
	ledChain.show();
	int mode = 0;
	auto start = tim.getEllapsedMs();
	float pos = 0;
	while(true)
	{
		if(tim.getEllapsedMs()-start >= 5000)
		{
			start = tim.getEllapsedMs();
			mode++;
			mode = mode%4;
		}
		tim.sleep(15);
		switch(mode)
		{
		case 0:
			sparklingAnim.update(15);
			break;
		case 1:
			ringLoadAnim.update(5);
			//change color after every completed cycle
			if(ringLoadAnim.angle >= numLeds)
			{
				ringLoadAnim.color.g = std::rand()%0xFF;
				ringLoadAnim.color.b = std::rand()%0xFF;
				ringLoadAnim.color.r = std::rand()%0xFF;
			}
			break;
		case 2:
			std::fill(frameBuffer.begin(),frameBuffer.end(),Color{0x02,0x02,0x02});
			trailSpinnerAnim.update(5);
			trailSpinnerAnim2.update(5);
			trailSpinnerAnim3.update(5);
			break;
		case 3:
			for(int i = 0; i<numLeds; i++)
			{
				pos += 0.05;
				frameBuffer[i] = Color::fromHsv(std::fmod(i*360.0/numLeds+pos,360),1,1);
			}
		}
		ledChain.sync();
		ledChain.setFrame(frameBuffer);
		ledChain.show();
	}
}
//{
//	setupClock();
//	timer tim;
//	ws2811Driver<48> ledChain;
//
//	std::array<Color,numLeds> colors;
//	std::array<uint8_t,numLeds> mapping;
//
//	//Default mapping
//	for(int i = 0; i<numLeds;i++)
//	{
//		mapping[i] = i;
//	}
//
//	//HSV Colorwheel
//	for(int i = 0; i<48; i++)
//	{
//		colors[i] = Color::fromHsv(i*360/48.0,1,1);
//	}
//
//	ledChain.setFrame(colors,mapping);
//	ledChain.show();
//	ledChain.sync();
//	tim.sleep(1500);
//	uint32_t start = tim.getEllapsedMs();
//	int mode = 2;
//	auto p = Particle{};
//	p.setVel(4);
//
//	std::array<Color,numLeds> colors2;
//	colors2[0] = Color{0x00,0x00,0xFF};
//	uint32_t offset;
//	uint32_t s,e;
//	int x = 0;
//
//
//
//
//	while(true)
//	{
//		switch(mode)
//		{
//		case 0:
//			s = tim.getEllapsedMs();
//			tim.sleep(15);
//			for(int i = 0; i<48; i++)
//			{
//				colors[i] = Color::fromHsv((std::fmod(i*360/48.0+(float)tim.getEllapsedMs()*0.01,360)),1,1);
//			}
//			ledChain.setFrame(colors);
//			ledChain.sync();
//			ledChain.show();
//			break;
//		case 1:
////			tim.sleep(15);
////			p.setForce(-0.05);
////			p.update(0.15);
////
////			//uint8_t pos = std::round(std::fmod(p.getPos(),24));
////			std::fill(colors.begin(),colors.end(),Color{0xFF,0,0});
////			for(int i = 0; i<24;i++)
////			{
////				if(i == pos)
////				{
////					colors[i*2] = Color{0,0,0xFF};
////					colors[(i*2+1)%48] = Color{0,0,0xFF};
////					int right = i*2-1;
////					if(right < 0)
////					{
////						right = 48-right;
////					}
////					colors[right] = Color{0,0,0xFF};
////				}
////			}
////
////			if(p.getVel() <= 0)
////			{
////				while(true)
////				{
////
////				}
////			}
////
////			ledChain.sync();
////			ledChain.setFrame(colors);
////			ledChain.show();
////			break;
//		case 2:
//			auto pg = perlinNoiseGenerator<uint8_t>{0xFF,6000};
//			auto map = std::array<uint8_t,numLeds>{};
//
//			std::fill(map.begin(),map.end(),0);
//			while(true)
//			{
//				map[0] = pg.update();
//				std::rotate(map.begin(),map.begin()+1,map.end());
//				tim.sleep(250);
//				ledChain.sync();
//				ledChain.setFrame(ColorMap::fire,map);
//				ledChain.show();
//			}
//			break;
//		}
//	}
//}


// ----------------------------------------------------------------------------
