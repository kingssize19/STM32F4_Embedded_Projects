#include "stm32f4xx.h"
#include "stm32f446xx.h"

uint8_t ldr_value;

void SystemClock_Config(void)
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

void GPIO_Config()
{
	RCC->AHB1ENR |= (1 << 0);				//GPIOA clock bus active

	//PA5 CONF LED1

	//MODE OUT
	GPIOA->MODER |= (1 << 10);
	GPIOA->MODER &= ~(1 << 11);

	//PUSH PULL
	GPIOA->OTYPER &= ~(1 << 5);

	//NO PULL
	GPIOA->PUPDR &= ~(1 << 10);
	GPIOA->PUPDR &= ~(1 << 11);

	//MEDİUM SPEED
	GPIOA->OSPEEDR |= (1 << 10);
	GPIOA->OSPEEDR &= ~(1 << 11);

	//PA7 CONF LED

	//MODE OUT
	GPIOA->MODER |= (1 << 14);
	GPIOA->MODER &= ~(1 << 15);

	//PUSH PULL
	GPIOA->OTYPER &= ~(1 << 7);

	//NO PULL
	GPIOA->PUPDR &= ~(1 << 14);
	GPIOA->PUPDR &= ~(1 << 15);

	//MEDİUM SPEED
	GPIOA->OSPEEDR |= (1 << 14);
	GPIOA->OSPEEDR &= ~(1 << 15);


	//LDR CONFİG

	//ANALOG MODE
	GPIOA->MODER |= (1 << 0);
	GPIOA->MODER |= (1 << 1);

	//PUSH PULL
	GPIOA->OTYPER &= ~(1 << 0);

	//PULL DOWN
	GPIOA->PUPDR &= ~(1 << 0);
	GPIOA->PUPDR |= (1 << 1);

	//MEDİUM SPEED
	GPIOA->OSPEEDR |= (1 << 0);
	GPIOA->OSPEEDR &= ~(1 << 1);
}

void ADC_Config()
{
	//APB2 enable
	RCC->APB2ENR |= (1 << 8);

	//Resolution 8 bit
	ADC1->CR1 &= ~(1 << 24);
	ADC1->CR1 |= (1 << 25);

	//ADC1 ON
	ADC1->CR2 |= (1 << 0);

	//SAMPLE TİME
	ADC1->SMPR2 |= 0x00000003;

	//ADCPRE PRESCALER CLOCK DİV 8
	ADC->CCR |= (1 << 16);
	ADC->CCR |= (1 << 17);
}

uint8_t Read_ADC()
{
	uint8_t value;

	ADC1->CR2 |= (1 << 30); //START SOFTWARE

	while (!(ADC1->SR & (1 << 1)));				//wait EOC

	value = ADC1->DR;

	return value;
}

int main(void)
{
  SystemClock_Config();
  SystemCoreClockUpdate();
  GPIO_Config();
  ADC_Config();

  while (1)
  {
	  ldr_value = Read_ADC();

	  if (ldr_value > 150)
	  {
		  GPIOA->ODR |= (1 << 5);
		  GPIOA->ODR &= ~(1 << 7);

	  }
	  else
	  {
		  GPIOA->ODR |= (1 << 7);
		  GPIOA->ODR &= ~(1 << 5);
	  }
  }
}
