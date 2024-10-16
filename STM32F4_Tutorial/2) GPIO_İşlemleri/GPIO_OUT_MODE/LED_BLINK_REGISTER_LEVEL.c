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
	//GPIOA clock Enable
	RCC->AHB1ENR |= (1 << 0);

	//GPIOA Pin5 output mode config
	GPIOA->MODER |= (1 << 10);
	GPIOA->MODER &= ~(1 << 11);

	//GPIOA Pin5 Fast Speed (10)
	GPIOA->OSPEEDR &= ~(1 << 10);
	GPIOA->OSPEEDR |= (1 << 11);


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
	  GPIOA->ODR |= (1 << 5);			//5. PİN SET
	  delay(16800000);
	  GPIOA->ODR &= ~(1 << 5);			//5. pin reset
	  delay(16800000);
  }
}
