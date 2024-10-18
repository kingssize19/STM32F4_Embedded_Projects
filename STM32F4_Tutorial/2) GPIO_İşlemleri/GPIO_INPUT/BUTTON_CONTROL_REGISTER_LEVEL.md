## STM32F446RE Nucleo - Button Control
```c
#include "stm32f4xx.h"
#include "stm32f446xx.h"
```
* STM32F4xx ve STM32F446xx için gerekli başlık dosyaları dahil edilmiştir. Bu dosyalar, STM32 mikrodenetleyici donanımına erişim sağlayan yapılandırmaları içerir.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## RCC_Config() Fonksiyonu
Bu fonksiyon sistem saati ve PLL yapılandırması için kullanılır. HSE kullanılarak sistem saatinin ayarlanması sağlanır. 

**1. HSI'yı Kapatma :**
```c
RCC->CR &= ~(1 << 0);  // HSI OFF
```
HSI (High-Speed Internal oscillator) kapatılır.

**2. HSE'yi Açma :**
```c
RCC->CR |= (1 << 16);  // HSE ON
while (!(RCC->CR & (1 << 17)));  // HSE aktif olana kadar bekle
```
HSE (High-Speed External oscillator) açılır ve HSE'nin aktif olmasını bekler.

**3. Flash Gecikme Ayarı :**
```c
FLASH->ACR |= FLASH_ACR_LATENCY_5WS;
```
FLASH belleği için gecikme ayarları yapılır (5 wait state).

**4. PLL Ayarları :**
```c
RCC->PLLCFGR = 0;  // PLLCFGR'yi sıfırlama
RCC->PLLCFGR |= (1 << 22);  // PLL Kaynağı HSE
RCC->PLLCFGR |= (4 << 0);   // PLL_M = 4
RCC->PLLCFGR |= (168 << 6); // PLL_N = 168
RCC->PLLCFGR |= (0 << 16);  // PLL_P = 2
RCC->PLLCFGR |= (7 << 24);  // PLL_Q = 7
```
PLL ayarları yapılarak HSE kaynağı 2 MHz'e bölünür ve 168 MHz saat sinyali elde edilir.

**5. PLL'yi Aktif Etme :**
```c
RCC->CR |= (1 << 24);  // PLL ON
while (!(RCC->CR & (1 << 25)));  // PLL aktif olana kadar bekle
```

**6. Sistem Saat Kaynağını PLL Olarak Seçme :**
```c
RCC->CFGR &= ~(3 << 0);  // Saat kaynağını temizle
RCC->CFGR |= (2 << 0);   // PLL'yi saat kaynağı olarak seç
```

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## GPIO_Config() Fonksiyonu

GPIO portları ve pinler yapılandırılır. Aşağıda yapılan işlemler özetlenmiştir:

**1. GPIOA ve GPIOB portlarını aktif etme :**
```c
RCC->AHB1ENR |= (3 << 0);  // GPIOA ve GPIOB aktif
```

**2. PB6 ve PB4 için giriş modunu ayarlama :**
```c
GPIOB->MODER &= ~(1 << 12);
GPIOB->MODER &= ~(1 << 13);  // PB6 giriş modu
GPIOB->MODER &= ~(1 << 8);
GPIOB->MODER &= ~(1 << 9);   // PB4 giriş modu
```

**3. Çıkış modu ve diğer ayarları yapılandırma :**
  * PB4 ve PB6 için push-pull modu ve hız ayarları yapılır.
  * PA5 pini için çıkış modu, push-pull modu, ve hız ayarları yapılır.
PA5 pinini çıkış olarak yapılandırarak LED kontrolüne uygun hale getirilir.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## delay() Fonksiyonu (Buton Ark'ını yok etmek için)

```c
void delay(uint32_t time)
{
   while (time--);
}
```

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## main() Fonksiyonu

Ana fonksiyon, mikrodenetleyicinin çalıştığı koddur.

**1. Saat ve GPIO yapılandırma fonksiyonları çağrılır :**
```c
RCC_Config();
GPIO_Config();
```

**2. Buton kontrolü ve LED yakma işlemleri:**
```c
if (GPIOB->IDR & (1 << 4))
```
* PB4 pini okunur. Eğer butona basılmışsa, LED (PA5) yanar ve belirli bir süre sonra söner.

```c
else if (GPIOB->IDR & (1 << 6))
```
* PB6 pini kontrol edilir ve aynı şekilde LED yanma-sönme işlemi yapılır.










