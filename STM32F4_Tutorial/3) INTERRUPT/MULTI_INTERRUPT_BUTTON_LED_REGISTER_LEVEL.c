#include "stm32f4xx.h"
#include "stm32f446xx.h"





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

void GPIO_Config(void)
{
    // GPIOA ve GPIOB saatlerini aktif et
    RCC->AHB1ENR |= (1 << 0) | (1 << 1);

    // PA5 ve PA7 çıkış modunda yapılandırılıyor (LED'ler için)
    GPIOA->MODER &= ~(0xF << 10);  // PA5 ve PA7 pinlerini temizle
    GPIOA->MODER |= (1 << 10) | (1 << 14);  // PA5 ve PA7 output mode

    // Hız ve pull-up/pull-down yapılandırması (opsiyonel)
    GPIOA->OTYPER &= ~(1 << 5) & ~(1 << 7); // Push-pull
    GPIOA->OSPEEDR |= (1 << 11) | (1 << 15);  // Yüksek hız
    GPIOA->PUPDR &= ~(1 << 10) & ~(1 << 14); // No pull-up/pull-down
}

void EXTI_Config(void)
{
    // SYSCFG ve EXTI saatlerini aktif et
    RCC->APB2ENR |= (1 << 14);  // SYSCFG saatini aç

    // EXTI3 ve EXTI4 için NVIC ayarları
    NVIC_EnableIRQ(EXTI3_IRQn);
    NVIC_EnableIRQ(EXTI4_IRQn);

    // EXTI3 ve EXTI4 önceliklerini ayarla
    NVIC_SetPriority(EXTI3_IRQn, 1);
    NVIC_SetPriority(EXTI4_IRQn, 0);

    // EXTI3 ve EXTI4 için SYSCFG ayarları (PA3 ve PA4)
    SYSCFG->EXTICR[0] = (1 << 12); // PB3 için EXTI3
    SYSCFG->EXTICR[1] = (1 << 0);  // PB4 için EXTI4

    // EXTI mask ve tetikleme ayarları
    EXTI->IMR |= (1 << 3) | (1 << 4);  // EXTI3 ve EXTI4 mask aktif
    EXTI->RTSR |= (1 << 3) | (1 << 4); // Yükselen kenar tetikleme
}


void delay(uint32_t time)
{
    while (time--);
}

void EXTI3_IRQHandler()
{
    if (EXTI->PR & (1 << 3))
    {

        for (int i = 0; i < 5; ++i)
        {
            GPIOA->ODR |= (1 << 5); // PA5 LED ON
            delay(3600000);
            GPIOA->ODR &= ~(1 << 5); // PA5 LED OFF
            delay(3600000);
        }
        EXTI->PR |= (1 << 3); // EXTI3 bayrağını temizle
    }
}

void EXTI4_IRQHandler()
{
    if (EXTI->PR & (1 << 4))
    {


        for (int i = 0; i < 3; ++i)
        {
            GPIOA->ODR |= (1 << 7); // PA7 LED ON
            delay(1800000);
            GPIOA->ODR &= ~(1 << 7); // PA7 LED OFF
            delay(1800000);
        }
       EXTI->PR |= (1 << 4); // EXTI4 bayrağını temizle
    }
}

int main(void)
{
    SystemClock_Config();
    SystemCoreClockUpdate();
    GPIO_Config();
    EXTI_Config();

    while (1)
    {
        // Sistemi düşük güç moduna alabilir veya bekleyebilirsiniz
          // Wait For Interrupt

    }
}

