#pragma once

#include <cstdint>
#include <stm32f30x.h>
#include <array>
#include <Color.h>
#include <iterator>

template<size_t numLeds>
struct ws2811Driver
{
	constexpr static uint8_t highBitHighTime = 122;
	constexpr static uint8_t lowBitHighTime = 58;
	constexpr static uint8_t syncTime = 100;
	ws2811Driver()
	{
		//Configure Data Out Pin
		RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

		GPIO_InitTypeDef gpioInitStruct;
		gpioInitStruct.GPIO_Mode = GPIO_Mode_AF;
		gpioInitStruct.GPIO_Pin = GPIO_Pin_7;
		gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		gpioInitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
		gpioInitStruct.GPIO_OType = GPIO_OType_PP;

		GPIO_Init(GPIOC,&gpioInitStruct);
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_2);

		//Configure Timer3 in PWM Mode
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
		RCC->CFGR3 |= (1<<25);
		TIM3->ARR = (uint16_t)179;

		//enable dma request on update event
		TIM3->DIER |= TIM_DIER_UDE;

		//setup capture compare channel 2
		TIM3->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2FE;
		TIM3->CCER |= TIM_CCER_CC2E;

		//Configure DMA
		RCC->AHBENR |= RCC_AHBENR_DMA1EN;
		DMA1_Channel3->CCR |= DMA_CCR_PL | DMA_CCR_PSIZE_0 | DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE;
		DMA1_Channel3->CNDTR = (uint16_t)(numLeds*24+syncTime);
		DMA1_Channel3->CMAR = (uint32_t)framePixelBuffer.data();
		DMA1_Channel3->CPAR = (uint32_t)&TIM3->CCR2;

		clearFrame();
	}

	void setLED(int n, Color color)
	{
		volatile uint32_t colorVal = color.getAsRGBValue();

		for(unsigned int i = 0; i<24; i++)
		{
			framePixelBuffer[n*24+i] = (colorVal & (1<<(23-i)))?highBitHighTime:lowBitHighTime;
		}
	}

	void setFrame(const std::array<Color,numLeds> &frame, const std::array<uint8_t,numLeds> &mapping)
	{
		for(unsigned int i = 0; i<numLeds; i++)
		{
			setLED(i,frame[mapping[i]]);
		}
	}

	template<class Callable>
	void setFrame(const Callable colormap, const std::array<uint8_t,numLeds> &mapping)
	{
		for(unsigned int i = 0; i<numLeds; i++)
		{
			setLED(i,colormap(mapping[i]));
		}
	}

	void setFrame(const std::array<Color, numLeds> frame)
	{
		for(unsigned int i = 0; i<numLeds;i++)
		{
			setLED(i,frame[i]);
		}
	}



	void clearFrame()
	{
		for(int i = 0; i< (int)(numLeds*24);i++)
		{
			framePixelBuffer[i] = lowBitHighTime;
		}
		for(int i = (int)(numLeds*24); i<(int)(numLeds*24+syncTime);i++)
		{
			framePixelBuffer[i] = 0;
		}
	}

	void show()
	{
		//enable timer and dma
		TIM3->CR1 &= ~TIM_CR1_CEN;
		TIM3->CNT = 0;
		DMA1_Channel3->CCR &= ~DMA_CCR_EN;
		DMA1_Channel3->CNDTR = (uint16_t)numLeds*24+syncTime;
		DMA1_Channel3->CMAR = (uint32_t)framePixelBuffer.data();
		DMA1_Channel3->CCR |= DMA_CCR_EN;
		TIM3->CR1 |= TIM_CR1_CEN;
	}

	void sync()
	{
		while((!(DMA1->ISR)) & (DMA_ISR_TCIF3))
		{
			__asm__("nop");
		}
	}

private:
	std::array<uint8_t,numLeds*24+syncTime> framePixelBuffer;
};


