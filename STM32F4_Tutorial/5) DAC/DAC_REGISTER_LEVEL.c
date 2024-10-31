#include "stm32f4xx.h"
#include "stm32f446xx.h"

uint16_t count = 0;

void delay(uint32_t time)
{
	while (time--);
}


void RCC_Config()
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
	    RCC->PLLCFGR |= (180 << 6);              // PLL_N = 180 (2 MHz * 180 = 360 MHz)
	    RCC->PLLCFGR |= (0 << 16);               // PLL_P = 2 (360 MHz / 2 = 180 MHz)
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

void DAC1_Config()
{

	RCC->AHB1ENR |= (1 << 0);
	RCC->APB1ENR |= (1 << 29);					// DAC enable

	DAC->CR |= (1 << 0);						//DAC channel 1 active
	DAC->SWTRIGR |= 0x00000000;					//DAC channel software trigger disable
	DAC->DHR12R1 |= 0x00000000;					//DAC channel 1 12 bit right-align data



}



int main(void)
{
	RCC_Config();
	DAC1_Config();

  while (1)
  {
	  for (count = 0; count < 4096; count++)
	  {
		DAC->DHR12R1 = count;
		delay(10000);
		if (count == 4095)
		{
			while (count > 0)
			{
				count--;
				DAC->DHR12R1 = count;
				delay(10000);
			}
		}

	  }


  }
}
