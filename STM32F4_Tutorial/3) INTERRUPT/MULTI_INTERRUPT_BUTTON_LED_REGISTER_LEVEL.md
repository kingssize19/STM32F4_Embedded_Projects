Bu kod, STM32F446 mikrodenetleyicisini kullanarak bir GPIO (LED kontrolü) ve harici kesme (EXTI)(Buton kontrolü) uygulaması yapmaktadır.

# 1. Başlık Dosyaları
```c
#include "stm32f4xx.h"
#include "stm32f446xx.h"
```
* Bu başlık dosyaları, STM32F4 serisi mikrodenetleyici için donanım tanımlarını içerir. STM32F446'ya özel register yapılarını tanımlar.

--------------------------------------------------------------------------------------------------------------------------------------------------------------

# 2. SystemClock_Config() Fonksiyonu 

* Bu fonksiyon sistem saatini yapılandırmak için kullanılır. STM32'de işlemci çekirdeği ve çevresel bileşenlerin saat kaynağını (HSE, HSI, PLL gibi) ayarlamaktan sorumludur.

**2.1 HSI'nın kapatılması :**
```c
RCC->CR &= ~(1 << 0);  // HSI OFF
```
* HSI (Dahili 16 MHz osilatör) kapatılıyor çünkü HSE kullanılacak.

**2.2 HSE'nin açılması :**
```c
RCC->CR |= (1 << 16);  // HSE ON
while (!(RCC->CR & (1 << 17)));  // HSE aktif olana kadar bekle
```
* HSE (Harici osilatör) açılıyor. Bu durumda, harici bir kristal osilatör kaynağı kullanılıyor (genellikle 8 MHz).

**2.3 Flash Bellek Gecikmesi Ayarı :**
```c
FLASH->ACR |= FLASH_ACR_LATENCY_5WS;
```
* 180 MHz hızında çalışan sistem için 5 wait state'li flash bellek ayarı yapılıyor.

**2.4 PLL Ayarları : PLL (Phase Locked Loop) kullanılarak sistem saat frekansını artırıyoruz :**
* **PLL Kaynağı olarak HSE'yi seç :**
```c
RCC->PLLCFGR |= (1 << 22);  // PLL Kaynağı HSE
```
* **PLL_M, PLL_N, PLL_P ayarları :**
```c
RCC->PLLCFGR |= (4 << 0);   // PLL_M = 4 (HSE'yi 2 MHz'e böl)
RCC->PLLCFGR |= (180 << 6); // PLL_N = 180 (2 MHz * 180 = 360 MHz)
RCC->PLLCFGR |= (0 << 16);  // PLL_P = 2 (360 MHz / 2 = 180 MHz)
RCC->PLLCFGR |= (7 << 24);  // PLL_Q = 7 (USB için)
```
* Bu ayarlar, HSE'yi (8 MHz) önce PLL_M ile 2 MHz'e düşürür, ardından PLL_N ile 360 MHz'e çıkarır ve son olarak PLL_P ile 180 MHz'e düşürür. PLL_Q USB iletişim frekansını ayarlamak için kullanılır.

**2.5 PLL'nin açılması :**
```c
RCC->CR |= (1 << 24);  // PLL ON
while (!(RCC->CR & (1 << 25)));  // PLL aktif olana kadar bekle
```

**2.6 Sistem Saati Kaynağı olarak PLL'yi seç :**
```c
RCC->CFGR &= ~(3 << 0);  // Saat kaynağını temizle
RCC->CFGR |= (2 << 0);   // PLL'yi saat kaynağı seç
while ((RCC->CFGR & (3 << 2)) != (2 << 2));  // Doğrula
```
* Sistem saat kaynağı olarak PLL seçilir ve doğrulaması yapılır.

--------------------------------------------------------------------------------------------------------------------------------------------------------------

# 3. GPIO_Config Fonksiyonu
* Bu fonksiyon GPIO'ları (PA5 ve PA7) LED'leri kontrol etmek için yapılandırır.

**3.1 GPIOA ve GPIOB saatlerini aktif et :**
```c
RCC->AHB1ENR |= (1 << 0) | (1 << 1);  // GPIOA ve GPIOB saatlerini aç
```
**3.2 PA5 ve PA7 pinlerini çıkış modunda ayarla :**
```c
GPIOA->MODER &= ~(0xF << 10);  // PA5 ve PA7 pinlerini temizle
GPIOA->MODER |= (1 << 10) | (1 << 14);  // PA5 ve PA7 output mode
```

**3.3 Pinlerin hız ve pull-up/pull-down ayarları :**
```c
GPIOA->OTYPER &= ~(1 << 5) & ~(1 << 7); // Push-pull
GPIOA->OSPEEDR |= (1 << 11) | (1 << 15);  // Yüksek hız
GPIOA->PUPDR &= ~(1 << 10) & ~(1 << 14);  // No pull-up/pull-down
```

--------------------------------------------------------------------------------------------------------------------------------------------------------------

# 4. EXTI_Config Fonksiyonu
* Bu fonksiyon, PA3 ve PA4 için harici kesmeleri (EXTI) yapılandırır.

**4.1 SYSCFG saatini aktif et :**
```c
RCC->APB2ENR |= (1 << 14);  // SYSCFG saatini aç
```

**4.2 NVIC ayarları (EXTI3 ve EXTI4 kesmeleri için) :**
```c
NVIC_EnableIRQ(EXTI3_IRQn);
NVIC_EnableIRQ(EXTI4_IRQn);
NVIC_SetPriority(EXTI3_IRQn, 1);
NVIC_SetPriority(EXTI4_IRQn, 0);
```

**4.3 SYSCFG yapılandırması :**
```c
SYSCFG->EXTICR[0] = (1 << 12); // PB3 için EXTI3
SYSCFG->EXTICR[1] = (1 << 0);  // PB4 için EXTI4
```

**4.4 EXTI mask ve tetikleme ayarları :**
```c
EXTI->IMR |= (1 << 3) | (1 << 4);  // EXTI3 ve EXTI4 mask aktif
EXTI->RTSR |= (1 << 3) | (1 << 4); // Yükselen kenar tetikleme
```

--------------------------------------------------------------------------------------------------------------------------------------------------------------

# 5. delay Fonksiyonu 
```c
void delay(uint32_t time) {
    while (time--);
}
```
* Bu basit fonksiyon, time değeri kadar bekleme sağlar.

--------------------------------------------------------------------------------------------------------------------------------------------------------------

# 6. EXTI3_IRQHandler ve EXTI4_IRQHandler Fonksiyonları
* Her iki fonksiyon da kesme meydana geldiğinde PA5 ve PA7 LED'lerini yakıp söndürür.

**6.1 EXTI3 kesmesi (PA5 LED kontrolü) :**
```c
for (int i = 0; i < 5; ++i) { ... }  // PA5 LED'i 5 kez yakıp söndür
EXTI->PR |= (1 << 3);  // EXTI3 kesme bayrağını temizle
```

**6.2 EXTI4 kesmesi (PA7 LED kontrolü) :**
```c
for (int i = 0; i < 3; ++i) { ... }  // PA7 LED'i 3 kez yakıp söndür
EXTI->PR |= (1 << 4);  // EXTI4 kesme bayrağını temizle
```

--------------------------------------------------------------------------------------------------------------------------------------------------------------

# 7. main Fonksiyonu
```c
int main(void)
{
    SystemClock_Config();
    SystemCoreClockUpdate();
    GPIO_Config();
    EXTI_Config();

    while (1) {
        // Sistemi düşük güç moduna alabilir veya bekleyebilirsiniz
    }
}
```
* Bu fonksiyon, sistem saatini, GPIO'ları ve EXTI'yi yapılandırır. Daha sonra sonsuz döngüde çalışır. Sistemin düşük güç moduna alınabileceği bir yorum satırı var (bekleme moduna geçebilir).

























