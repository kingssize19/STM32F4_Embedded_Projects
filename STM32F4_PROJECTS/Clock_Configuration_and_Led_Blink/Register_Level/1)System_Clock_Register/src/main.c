#include "stm32f4xx.h"
#include "stm32f446xx.h"


#define PLL_M	4
#define PLL_N	180
#define PLL_P	0	//PLLP = 2


void SysClockConfig(void)
{
	// 1. ENABLE HSE and wait for the HSE to become Ready
	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));

	// 2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR	|= PWR_CR_VOS;

	// 3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;

	// 4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	// AHB PR
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	// APB1 PR
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

	// APB2 PR
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

	// 5. Configure the MAIN PLL
	// M = 4
	RCC->PLLCFGR |= (PLL_M << 0);

	// N = 180
	RCC->PLLCFGR |= (PLL_N << 6);

	// P = 2
	RCC->PLLCFGR |= (PLL_P << 16);

	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;

	// 6. Enable the PLL and wait for it to become ready
	RCC->CR |= (1 << 24);
	while (!(RCC->CR & RCC_CR_PLLRDY));

	// 7. Select the Clock Source and wait for it to be set
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

}

void GPIO_Config()
{
	// 1. Enable the GPIO CLOCK
	RCC->AHB1ENR |= (1 << 0);

	// 2. set the Pin as OUTPUT
	GPIOA->MODER |= (1<<10);  //pin PA5(bits 11:10) as output (01)

	// 3. Configure the OUTPUT MODE
	GPIOA->OTYPER = 0;
	GPIOA->OSPEEDR = 0;

}

void delay(uint32_t time)
{
	while(time--);
}


int main(void)
{
	SysClockConfig();
	GPIO_Config();

  while (1)
  {
	 GPIOA->BSRR |= (1<<5);		//set the pin PA5
	 delay(5000000);
	 GPIOA->BSRR |= (1<<21);	//reset the pin PA5
	 delay(5000000);
  }
}
