Bu kod, STM32F446 mikrodenetleyicisini kullanarak bir GPIO (LED kontrolü) ve harici kesme (EXTI)(Buton kontrolü) uygulaması yapmaktadır.

# 1. Başlık Dosyaları
```c
#include "stm32f4xx.h"
#include "stm32f446xx.h"
```
Bu başlık dosyaları, STM32F4 serisi mikrodenetleyici için donanım tanımlarını içerir. STM32F446'ya özel register yapılarını tanımlar.

--------------------------------------------------------------------------------------------------------------------------------------------------------------

# 2. SystemClock_Config() Fonksiyonu 

Bu fonksiyon sistem saatini yapılandırmak için kullanılır. STM32'de işlemci çekirdeği ve çevresel bileşenlerin saat kaynağını (HSE, HSI, PLL gibi) ayarlamaktan sorumludur.

**1. HSI'nın kapatılması :**
```c
RCC->CR &= ~(1 << 0);  // HSI OFF
```
* HSI (Dahili 16 MHz osilatör) kapatılıyor çünkü HSE kullanılacak.

**2. HSE'nin açılması :**
```c
RCC->CR |= (1 << 16);  // HSE ON
while (!(RCC->CR & (1 << 17)));  // HSE aktif olana kadar bekle
```
* HSE (Harici osilatör) açılıyor. Bu durumda, harici bir kristal osilatör kaynağı kullanılıyor (genellikle 8 MHz).

**3. Flash Bellek Gecikmesi Ayarı :**
```c
FLASH->ACR |= FLASH_ACR_LATENCY_5WS;
```
* 180 MHz hızında çalışan sistem için 5 wait state'li flash bellek ayarı yapılıyor.

**4. PLL Ayarları : PLL (Phase Locked Loop) kullanılarak sistem saat frekansını artırıyoruz :**
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

**5. PLL'nin açılması :**
```c
RCC->CR |= (1 << 24);  // PLL ON
while (!(RCC->CR & (1 << 25)));  // PLL aktif olana kadar bekle
```

**6. Sistem Saati Kaynağı olarak PLL'yi seç :**
```c
RCC->CFGR &= ~(3 << 0);  // Saat kaynağını temizle
RCC->CFGR |= (2 << 0);   // PLL'yi saat kaynağı seç
while ((RCC->CFGR & (3 << 2)) != (2 << 2));  // Doğrula
```
* Sistem saat kaynağı olarak PLL seçilir ve doğrulaması yapılır.

--------------------------------------------------------------------------------------------------------------------------------------------------------------

```c

```
```c

```
```c

```
```c

```
```c

```
```c

```
```c

```
```c

```
```c

```
```c

```
```c

```


























