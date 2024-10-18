#include "stm32f4xx.h"
#include "stm32f446xx.h"

void RCC_Config(void)
{
    // HSI'yi kapatıyoruz
    RCC->CR &= ~(1 << 0);                    // HSI OFF

    // HSE'yi aktif ediyoruz
    RCC->CR |= (1 << 16);                    // HSE ON
    while (!(RCC->CR & (1 << 17)));          // HSE aktif olana kadar bekle

    // FLASH latency ayarları (5 wait states)
    FLASH->ACR |= FLASH_ACR_LATENCY_5WS;

    // PLL ayarları (HSE kaynağına göre)
    RCC->PLLCFGR = 0;                        // PLLCFGR'yi sıfırlama

    // PLL ayarları
    RCC->PLLCFGR |= (1 << 22);               // PLL Kaynağı HSE
    RCC->PLLCFGR |= (4 << 0);                // PLL_M = 4 (HSE'yi 2 MHz'e böl)
    RCC->PLLCFGR |= (168 << 6);              // PLL_N = 168 (2 MHz * 168 = 336 MHz)
    RCC->PLLCFGR |= (0 << 16);               // PLL_P = 2 (336 MHz / 2 = 168 MHz)
    RCC->PLLCFGR |= (7 << 24);               // PLL_Q = 7 (USB için)

    // PLL'yi aktif ediyoruz
    RCC->CR |= (1 << 24);                    // PLL ON
    while (!(RCC->CR & (1 << 25)));          // PLL aktif olana kadar bekle

    // Sistem saat kaynağını PLL olarak seç
    RCC->CFGR &= ~(3 << 0);                  // Saat kaynağını temizle
    RCC->CFGR |= (2 << 0);                   // Sistem saat kaynağı olarak PLL'yi seç

    // Saat kaynağının PLL olduğunu kontrol et
    while ((RCC->CFGR & (3 << 2)) != (2 << 2));  // PLL'nin sistem saati olduğunu doğrula
}

void GPIO_Config(void)
{
	RCC->AHB1ENR |= (3 << 0);   //GPIOA and GPIOB enable

	//Input Mode (reset state) PB 6
	GPIOB->MODER &= ~(1 << 12);
	GPIOB->MODER &= ~(1 << 13);

	//Input Mode (reset state) PB4
	GPIOB->MODER &= ~(1 << 8);
	GPIOB->MODER &= ~(1 << 9);


	//PUSH PULL PB4, PB6
	GPIOB->OTYPER &= ~(1 << 4);
	GPIOB->OTYPER &= ~(1 << 6);

	//SPEED PB4, PB6
	GPIOB->OSPEEDR &= ~(1 << 9);
	GPIOB->OSPEEDR |= (1 << 8);

	GPIOB->OSPEEDR &= ~(1 << 13);
	GPIOB->OSPEEDR |= (1 << 12);

	//PUPDR PB4, PB6
	GPIOB->PUPDR &= ~(1 << 8);
	GPIOB->PUPDR |= (1 << 9);

	GPIOB->PUPDR &= ~(1 << 12);
	GPIOB->PUPDR |= (1 << 13);

	//LED CONF

	//MODE OUTPUT PA5
	GPIOA->MODER &= ~(1 << 11);
	GPIOA->MODER |= (1 << 10);

	//PUSH PULL PA5
	GPIOA->OTYPER &= ~(1 << 5);

	//PA5 MEDİUM MODE
	GPIOA->OSPEEDR &= ~(1 << 11);
	GPIOA->OSPEEDR |= (1 << 10);

	//NO PULL PA5
	GPIOA->PUPDR &= ~(1 << 10);
	GPIOA->PUPDR &= ~(1 << 11);


}

void delay(uint32_t time)
{
	while (time--);
}


int main(void)
{
	RCC_Config();
	GPIO_Config();

  while (1)
  {

	 if (GPIOB->IDR & (1 << 4))
	 {
		 while (GPIOB->IDR & (1 << 4));
		 delay(168000);

		 GPIOA->ODR |= (1 << 5);
		 delay(50000000);
		 GPIOA->ODR &= ~(1 << 5);


	 }

	 else if (GPIOB->IDR & (1 << 6))
	 {
		 while (GPIOB->IDR & (1 << 6));
		 delay(168000);

		 GPIOA->ODR |= (1 << 5);
		 delay(5000000);
		 GPIOA->ODR &= ~(1 << 5);

	 }
  }
}
