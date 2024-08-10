#include "stm32f4xx.h"
#include "stm32f446xx.h"


#define 	PLL_M		4
#define 	PLL_N		180
#define 	PLL_P		0	//PLLP = 2

#define 	TIMER_PRESCALER		17999
#define		TIMER_ARR		999


void System_Clock_Config(void)
{
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;

	FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;

	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

	RCC->PLLCFGR |= (PLL_M << 0);
	RCC->PLLCFGR |= (PLL_N << 6);
	RCC->PLLCFGR |= (PLL_P << 16);

	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;

	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY));

	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

void GPIO_Config(void)
{
	RCC->AHB1ENR |= (1 << 0);

	GPIOA->MODER |= (1 << 10);
	GPIOA->OTYPER |= (0 << 5);
	GPIOA->OSPEEDR |= (2 << 10);
}

void Timer_Config(void)
{
	// 1. Timer saatini etkinleþtir.
	RCC->APB1ENR |= (1 << 0);

	// 2. Timer ayarlarý
	TIM2->PSC = TIMER_PRESCALER;		//Prescaler
	TIM2->ARR = TIMER_ARR;				//Auto-reload register

	// 3. Timer kesmelerini etkinleþtir
	TIM2->DIER |= TIM_DIER_UIE;			//Güncelleme kesme izni
	NVIC_EnableIRQ(TIM2_IRQn);			//Timer 2 kesmesini NVIC'te etkinleþtir.

	// 4. Timer'ý baþlat
	TIM2->CR1 |= TIM_CR1_CEN;

}

void TIM2_IRQHandler(void)
{
	if (TIM2->SR & TIM_SR_UIF) {		//Güncelleme kesme bayraðýný kontrol et.

		TIM2->SR &= ~TIM_SR_UIF;		// Kesme bayraðýný temizle

		// Led durumunu deðiþtir.
		GPIOA->ODR ^= (1 << 5);		// PA5 pinini deðiþtir.
	}
}
int main(void)
{
	System_Clock_Config();
	GPIO_Config();
	Timer_Config();


  while (1)
  {

  }
}


