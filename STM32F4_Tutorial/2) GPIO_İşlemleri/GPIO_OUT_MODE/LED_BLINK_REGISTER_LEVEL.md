### STM32F4 serisi bir mikrodenetleyici için basit bir GPIO LED yakma uygulamasıdır. Program, harici bir osilatör olan HSE'yi kullanarak sistem saatini 168 MHz olarak ayarlıyor ve ardından GPIOA pin 5'i bir kare dalga sinyal üretmek amacıyla sürekli yakıp söndürüyor.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 1. Kütüphaneler
```c
#include "stm32f4xx.h"
#include "stm32f446xx.h"
```
* Bu iki başlık dosyası STM32F4 serisi için mikrodenetleyiciye özgü tanımları ve fonksiyonları içerir. **stm32f4xx.h** genel bir başlık dosyası olup tüm STM32F4 ailesi için geçerlidir, **stm32f446xx.h** ise STM32F446 çipine özel detayları içerir.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

## RCC_Config() Fonksiyonu

**Bu fonksiyon, sistem saatini HSE üzerinden PLL kullanarak 168 MHz olarak ayarlamak için yazılmıştır.**


**1. HSI'yi Kapatma :**
```c
RCC->CR &= ~(1 << 0);              //HSI OFF
```
* STM32 mikrodenetleyicileri HSI adı verilen dahili bir 16 MHz osilatörle başlar. Bu satır HSI osilatörünü kapatır çünkü bu proje harici bir saat kaynağı (HSE) kullanıyor.


**2. HSE'yi Aktif Etme :** 
```c
RCC->CR |= (1 << 16);                //HSE ON
while (!(RCC->CR & (1 << 17)));      //HSE aktif olana kadar bekle
```
* Harici osilatörün etkinleştirildiği satır. Mikrodenetleyici, HSE'yi etkinleştirmeyi başardı mı diye kontrol eder. **HSE_READY** işareti kontrol edilerek saat kaynağı etkinleştiğinde döngüden çıkılır.


**3. FLASH Latency Ayarı :**
```c
FLASH->ACR |= FLASH_ACR_LATENCY_5WS;
```
* 168 MHz'lik yüksek hızda çalışırken, FLASH bellek erişim süresini (wait state) doğru ayarlamak gerekiyor. 5 wait state kullanılması gerekiyor, çünkü yüsek hızda daha fazla bekleme süresi gerekiyor.


**4. PLL Ayarları :**
```c
RCC->PLLCFGR = 0;                                        //PLLCFGR'yi sıfırlama
RCC->PLLCFGR |= (1 << 22);                               //PLL Kaynağı HSE
RCC->PLLCFGR |= (4 << 0);                                //PLL_M = 4   (HSE'yi 4'e böl 2 MHz)
RCC->PLLCFGR |= (168 << 6);                              //PLL_N = 168 (2 MHz * 168 = 336 MHz)
RCC->PLLCFGR |= (0 << 16);                               //PLL_P = 2   (336 MHz / 2 = 168 MHz)
RCC->PLLCFGR |= (7 << 24);                               //PLL_Q = 7   (USB için)
```
* **PLL (Phase Locked Loop)**, saat sinyalini büyütmek için kullanılan bir devredir. Burada PLL'nin ayarları yapılır.
  * **PLL_M = 4 :** HSE frekansını 4'e böler (Bu sayede 2 MHz elde edilir).
  * **PLL_N = 168 :** 2 MHz'yi 168 ile çarparak 336 MHz elde edilir.
  * **PLL_P = 2 :** 336 MHz, sistem saatini elde etmek için 2'ye bölünür. Sonuç olarak 168 MHz'lik sistem saati elde edilir.
  * **PLL_Q = 7 :** USB saat sinyalini sağlamak için kullanılır. USB 48 MHz gerektirir ve 336 MHz, 7'ye bölünerek 48 MHz elde edilir.


**5. PLL'yi Aktif Etme :**
```c
RCC->CR |= (1 << 24);                                //PLL ON
while (!(RCC->CR & (1 << 25)));                      //PLL aktif olana kadar bekle
```
* PLL devresinin etkinleştirilmesi ve devrenin hazır olup olmadığını kontrol eder.


**6. PLL'yi Sistem Saati Olarak Ayarlama :**
```c
RCC->CFGR &= ~(3 << 0);                            //Saat kaynağını temizle
RCC->CFGR |= (2 << 0);                             //Sistem saat kaynağı olarak PLL'yi seç
while ((RCC->CFGR & (3 << 2) != (2 << 2));         //PLL'nin sistem saati olduğunu doğrula
```
* Sistem saat kaynağını PLL olarak ayarlar. Saat kaynağı olarak PLL'nin seçilip seçilmediğini doğrular.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 3. GPIO_Config() Fonksiyonu

**Bu fonksiyon, GPIOA'nın pin 5'ini çıkış pini olarak yapılandırır.**

**1. GPIOA Clock Enable :**
```c
RCC->AHB1ENR |= (1 << 0);
```
* **GPIOA** portunun saat sinyalini etkinleştirir. GPIOA'ya erişim sağlamak için önce saat sinyali aktif edilmelidir.


**2. GPIOA Pin 5 Output Mode :**
```c
GPIOA->MODER |= (1 << 10);
GPIOA->MODER &= ~(1 << 11);
```
* GPIOA'nın pin 5'i çıkış modu olarak ayarlanır. Bu pin STM32F446RE Nucleo kartı üzerinde bulunan LED'e bağlıdır.


**3. GPIOA Pin 5 Fast Speed Ayarı :**
```c
GPIOA->OSPEEDR &= ~(1 << 10);
GPIOA->OSPEEDR |= (1 << 11);
```
* Pin hız ayarları yapılır. Fast speed olarak ayarlanır (orta hızdan daha hızlı bir sinyal değişimi).

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 4. delay() Fonksiyonu 

```c
void delay(uint32_t time)
{
  while (time--);
}
```
Bu fonksiyon basit bir bloklayıcı döngü ile gecikme sağlar. Zamanlama doğruluğu çok yüksek değildir, ancak bu tip uygulamalarda yeterlidir. Parametre olarak aldığı **time** kadar işlemciyi meşgul eder.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 5. main() Fonksiyonu 
Bu fonksiyon programın ana döngüsünü içerir.

**1. RCC ve GPIO Yapılandırmaları :**
```c
RCC_Config();
GPIO_Config();
```
* İlk önce saat kaynağı HSE + PLL ile yapılandırılır ve sistem saatini 168 MHz yapar. Ardından GPIOA'nın pi 5'i çıkış olarak ayarlanır.


**2. LED Yakıp Söndürme :**
```c
while (1)
{
    GPIOA->ODR |= (1 << 5);   // Pin 5'i HIGH yap (LED'i yak)
    delay(16800000);          // Bekle
    GPIOA->ODR &= ~(1 << 5);  // Pin 5'i LOW yap (LED'i söndür)
    delay(16800000);          // Bekle
}
```
* Sonsuz döngü içinde, GPIOA pin 5'i HIGH yapılarak LED yakılır, ardından bir süre beklenir. Sonrasında pin LOW yapılarak LED söndürülür ve tekrar beklenir.
* Gecikme süresi 16800000 olarak belirlenmiş. Bu, işlemci frekansına göre yaklaşık bir saniyelik gecikme yaratır (168 MHz frekansında).

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 6. Sonuç

Bu program, STM32F4 serisi bir mikrodenetleyici üzerinde HSE'yi kullanarak sistemi 168 MHz hızında çalıştırıyor. GPIOA'nın pin 5'ini kontrol ederek bir LED'i belirli bir gecikmeyle yakıp söndürüyor. Sistemin saat kaynağı olarak HSE + PLL kullanılıyor ve delay() fonksiyonu işlemciyi meşgul ederek basit bir zamanlama sağlıyor.





















