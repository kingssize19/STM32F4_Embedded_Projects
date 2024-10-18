### Bu STM32F4 serisi bir mikrodenetleyicisi için yazılmış bir GPIO yapılandırma ve kontrol programıdır. Program, iki butonu okuyup bir LED'i yakmak veya söndürmek için GPIO pinlerini kullanır. 

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 1. Kütüphaneler
```c
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
```
* **stm32f4xx.h :** STM32F4 serisi mikrodenetleyiciye özgü donanım yapılandırma ve işlevlerini içerir.
* **stm32f4_discovery.h :** STM32F4 Discovery kartı için gerekli kütüphaneyi içerir. Biz bu kütüphaneyi SPL fonksiyonlarını NUCLEO kartı üzerinde çalıştırabilmek için kullandık.

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 2. Global Değişken Tanımı
```c
GPIO_InitTypeDef    GPIO_InitStruct;
```
* Bu yapı, GPIO pinlerinin yapılandırma ayarlarını tutar (mod, hız, direnç durumu vb.).

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 3. GPIO Yapılandırma Fonksiyonu 
```c
void GPIO_Config(void)
{
    // GPIOA ve GPIOB portları için saat sinyali etkinleştiriliyor
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    
    // GPIOB Pin 6 ve Pin 4 giriş olarak ayarlanıyor
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_4;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    // GPIOA Pin 5 çıkış olarak ayarlanıyor (LED kontrolü)
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}
```
**Bu fonksiyon, GPIO pinlerini ayarlar :**
* **GPIOB Pin 6 ve Pin 4 (Butonlar) :** Giriş modunda ayarlanmış. GPIO_PuPd_DOWN ile aşağı çekme direnci etkinleştirilmiş, yani butonlar basılmadığında mantıksal 0 olur.
* **GPIOA Pin 5 (LED) :** Çıkış modunda ayarlanmış, böylece bu pine sinyal gönderilip LED kontrol edilebilir.

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 4. delay Fonksiyonu
```c
void delay(uint32_t time)
{
  while (time--);
}
```
Bu fonksiyon, basit bir yazılım gecikmesi oluşturmak için kullanılır. Butonlarda oluşacak ark'ların önüne geçmek için yazılmış bir gecikme fonksiyonu.

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 5. main Fonksiyonu
```c
int main(void)
{
    GPIO_Config();

    while (1)
    {
        // Eğer GPIOB Pin 6 (Buton 1) basılıysa
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6))
        {
            delay(50000); // Gecikme
            // Tekrar kontrol, eğer hala basılıysa LED yak
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6))
            {
                GPIO_SetBits(GPIOA, GPIO_Pin_5); // LED yak
            }
            // Buton bırakılana kadar bekle
            while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6));
        }

        // Eğer GPIOB Pin 4 (Buton 2) basılıysa
        else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4))
        {
            delay(50000); // Gecikme
            // Tekrar kontrol, eğer hala basılıysa LED söndür
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4))
            {
                GPIO_ResetBits(GPIOA, GPIO_Pin_5); // LED söndür
            }
            // Buton bırakılana kadar bekle
            while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4));
        }
    }
}
```
**Bu ana fonksiyon şunları yapar :**
* **GPIO_Config çağrısı :** GPIO pinleri yapılandırılır.
* **while (1) :**
    * **Buton 1 (Pin 6) :** Buton 1 basıldıktan sonra kısa bir gecikme uygulanır. Eğer buton hala basılıysa, LED (GPIOA Pin5) yakılır ve buton bırakılana kadar beklenir.
    * **Buton 2 (Pin 4) :** Buton 1'e benzer şekilde işlem yapılır, bu sefer LED söndürülür.

  




