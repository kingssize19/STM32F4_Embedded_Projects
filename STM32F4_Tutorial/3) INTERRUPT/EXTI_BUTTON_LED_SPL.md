Bu kod, bir STM32F4 mikrodenetleyicisi üzerinde harici kesme (EXTI) kullanarak bir düğmeye basma olayı ile LED'leri yakıp söndüren bir uygulamadır. Kesme ve giriş-çıkış (GPIO) yapılandırmaları kullanılmıştır.

## 1. Başlık Dosyalarının Dahil Edilmesi
```c
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
```
* **stm32f4xx.h :** STM32F4 serisi mikrodenetleyicilerin çekirdek yapılandırma ve donanım başlık dosyası.
* **stm32f4_discovery.h :** STM32F4 Discovery kartı için çeşitli sürücü ve yardımcı fonksiyonlar sağlar. (STM32F446RE Nucleo için kullandık.)

------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 2. Global Değişkenler
```c
GPIO_InitTypeDef GPIO_InitStruct;
EXTI_InitTypeDef EXTI_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;
```
* GPIO_InitTypeDef, EXTI_InitTypeDef ve NVIC_InitTypeDef yapılandırma yapıları, GPIO, EXTI (kesme) ve NVIC (kesme denetleyici) için kullanılan yapılandırma parametrelerini içerir.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 3. GPIO Yapılandırma Fonksiyonu
```c
void GPIO_Config(void)
{
    // Saat sinyallerini etkinleştir
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	// GPIOA için saat sinyalini etkinleştir
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	// GPIOB için saat sinyalini etkinleştir
```
* RCC_AHB1PeriphClockCmd ile GPIOA ve GPIOB portları için saat sinyalleri etkinleştirilir. Bu olmadan portlar çalışmaz.

## 3.1 LED Yapılandırması (GPIOA)
```c
    // LED konfigürasyonu (PA5 ve PA6 pinleri)
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;            // Çıkış modu
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;   // PA5 ve PA6 pinleri
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;           // Push-pull tipi
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;         // Pull-up/pull-down yok
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;        // Hız: 50 MHz

    GPIO_Init(GPIOA, &GPIO_InitStruct);                   // Ayarları GPIOA portuna uygula
```
* PA5 ve PA6 pinleri, LED çıkışı olarak ayarlanmıştır. Bu pinler push-pull çıkış tipi ve 50 MHz hızla çalışır.

## 3.2 Buton Yapılandırması (GPIOB)
```c
    // Buton konfigürasyonu (PB4 pini)
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;             // Giriş modu
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;                // PB4 pini
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;           // Push-pull tipi
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;           // Pull-down aktif (buton basılı olmadığında '0' seviyesinde)
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;        // Hız: 50 MHz

    GPIO_Init(GPIOB, &GPIO_InitStruct);                   // Ayarları GPIOB portuna uygula
}
```
* PB4 pini bir buton için giriş olarak ayarlanmıştır. Pull-down ile pin, butona basılmadığında '0' seviyesinde kalır.

------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 4. Kesme (EXTI) Yapılandırma Fonksiyonu
```c
void EXTI_Config(void)
{
    // SYSCFG saat sinyali etkinleştir
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);	// EXTI için gerekli olan SYSCFG'nin saat sinyali

    // EXTI konfigürasyonu (PB4 pini)
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);    // PB4 pinini EXTI hattına bağla

```
* SYSCFG_EXTILineConfig, GPIOB'nin 4. pinini EXTI hattına bağlar.


## 4.1 EXTI Hat Yapılandırması
```c
    EXTI_InitStruct.EXTI_Line = EXTI_Line4;               // EXTI hattı 4 (PB4 pini için)
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;                // EXTI hattını etkinleştir
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;      // Mod: kesme (interrupt)
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;   // Tetikleme: Yükselen kenar (butona basıldığında)
    
    EXTI_Init(&EXTI_InitStruct);                          // Yapılandırmayı uygula
```
* EXTI hattı PB4 için etkinleştirilmiştir ve yükselen kenar tetiklemesi (butona basıldığında) ayarlanmıştır.


## 4.2 NVIC Yapılandırması (Kesme Denetleyici)
```c
    NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;         // EXTI hattı 4 için kesme kanalı
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;          // Kanalı etkinleştir
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0; // Öncelik seviyesi: 0
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;       // Alt öncelik seviyesi: 0

    NVIC_Init(&NVIC_InitStruct);                          // NVIC yapılandırmasını uygula
}
```
* NVIC konfigürasyonu, EXTI4 kesmesini (PB4 butonu) kullanacak şekilde yapılmıştır. Kesme önceliği ve alt öncelik 0 olarak ayarlanmıştır, yani en yüksek önceliğe sahiptir.

------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 5. Delay Fonksiyonu
```c
void delay(uint32_t time)
{
    while (time--);
}
```
* Basit bir delay fonksiyonu, parametre olarak verilen süre kadar döngü yapar ve bekler.

------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 6. EXTI Kesme Servis Rutini (ISR)
```c
void EXTI4_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line4) != RESET) {          // EXTI hattı kesmesi tetiklenmiş mi?
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);                // PA5 (LED) kapat

        for (int i = 0; i < 3; ++i) {
            GPIO_SetBits(GPIOA, GPIO_Pin_6);              // PA6 (LED) yak
            delay(16800000);                              // Gecikme
            GPIO_ResetBits(GPIOA, GPIO_Pin_6);            // PA6 (LED) söndür
            delay(16800000);                              // Gecikme
        }

        EXTI_ClearITPendingBit(EXTI_Line4);               // EXTI kesme bayrağını temizle
    }
}
```
* EXTI_IRQHandler fonksiyonu, PB4 pinine basıldığında tetiklenen kesme servis rutinidir. Kesme tetiklendiğinde :
  * PA5 pini kapatılır (LED söndürülür.)
  * PA6 pini 3 kez yakıp söndürülür.
  * Kesme bayrağı temizlenir (bir sonraki kesmeye hazırlanılır).

------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 7. main Fonskiyonu
```c
int main(void)
{
    GPIO_Config();    // GPIO'ları yapılandır
    EXTI_Config();    // EXTI (kesme) yapılandır

    while (1)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_5);  // Ana döngüde PA5 pini sürekli olarak açık tutulur (LED yanar)
    }
}
```
* GPIO_Config ve EXTI_Config çağrılarak gerekli yapılandırmalar yapılır.
* while(1) içerisinde PA5 pini sürekli olarak açık tutulur (LED sürekli yanar).

------------------------------------------------------------------------------------------------------------------------------------------------------------------

## ÖZET

* Bu programda bir butona basıldığında tetiklenen bir kesme (EXTI) kullanılıyor.
* PB4 pinine bağlı butona basıldığında PA6 pini (LED) 3 kez yakıp söndürülüyor.
* Ana döngüde ise PA5 pini sürekli olarak açık tutuluyor (LED sürekli yanıyor).
* EXTI yapılandırması NVIC ile kesme denetleyicisine bağlanmış durumda.





