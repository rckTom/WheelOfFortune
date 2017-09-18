#include <timer.h>
#include <stm32f30x.h>
#include <diag/Trace.h>

volatile uint32_t timer::value = 0;

timer::timer()
{
	NVIC_EnableIRQ(IRQn::TIM7_IRQn);
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
	TIM7->ARR = 10;
	TIM7->PSC = 7199;
	TIM7->DIER |= TIM_DIER_UIE;
	TIM7->CR1 |= TIM_CR1_CEN;
}

void timer::timer_tick()
{
	timer::value++;
}

uint32_t timer::getEllapsedMs()
{
	return timer::value;
}

void timer::sleep(uint32_t ms)
{
	uint32_t start = getEllapsedMs();
	while(getEllapsedMs()-start < ms)
	{
		__asm__("nop");
	}
}

extern "C"{
	void TIM7_IRQHandler()
	{
		TIM7->SR &= ~TIM_SR_UIF;
		timer::timer_tick();
	}
}
