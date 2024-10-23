Bu kodda STM32F4 mikrokontrolcüsü kullanarak bir LDR (Light Dependent Resistor) sensöründen ADC (Analog-to-Digital Converter) yoluyla analog veriyi okuyoruz ve ardından bu değere göre GPIO pinlerine bağlı LED'leri kontrol ediyoruz. Kod, ışık seviyesini ölçerek belirli bir eşik değerine göre LED'leri yakıp söndürmek için yapılandırılmıştır.

## 1. Kütüphanelerin Dahil Edilmesi
```c
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
```

------------------------------------------------------------------------------------------------------------------------------------------------------------

## 2. Global Değişkenler ve Yapılandırma Yapıları
```c
GPIO_InitTypeDef GPIO_InitStruct;
ADC_InitTypeDef  ADC_InitStruct;
ADC_CommonInitTypeDef ADC_CommonInitStruct;

uint16_t ldr_value = 0;
```
* **GPIO_InitStruct :** GPIO pinlerini yapılandırmak için kullanılır.
* **ADC_InitStruct :** ADC modülünü yapılandırmak için kullanılır.
* **ADC_CommonInitStruct :** ADC’nin ortak ayarları için kullanılır (örneğin, prescaler gibi).
* **ldr_value :** ADC tarafından okunan LDR'nin değeri bu değişkende tutulur.

------------------------------------------------------------------------------------------------------------------------------------------------------------

## 3. GPIO Yapılandırma Fonksiyonu - GPIO_Config
```c
void GPIO_Config(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
```
* GPIOA portunun saat sinyali etkinleştiriliyor. Bu, GPIO pinlerinin kullanılabilmesi için gereklidir.

```c
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStruct);
```
* **GPIOA Pin 0 :** LDR'nin bağlı olduğu pin. Bu pin ADC'den veri alacağımız için Analog Mod (GPIO_Mode_AN) olarak yapılandırılmış.
* **GPIO_PuPd_UP :** Pull-up direnci etkinleştirilmiş. Bu, girişteki sinyalin kararlı bir seviyede tutulmasına yardımcı olur.

```c
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStruct);
}
```
* **GPIOA Pin 5 ve Pin 7 :** LED'lerin bağlı olduğu pinlerdir. Bu pinler output modunda yapılandırılmış, bu sayede LED'leri yakıp söndürebiliriz.

------------------------------------------------------------------------------------------------------------------------------------------------------------

## 4. ADC Yapılandırma Fonksiyonu - ADC_Config
```c
void ADC_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
```
* ADC1 modülünün saat sinyali etkinleştiriliyor.

```c
    ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div8;

    ADC_CommonInit(&ADC_CommonInitStruct);
```
* **ADC_Mode_Independent :** ADC'yi bağımsız modda çalıştırıyoruz, yani ADC1 diğer ADC'lerden bağımsız çalışacak.
* **ADC_Prescaler_Div8 :** ADC modülü için saat frekansını bölüyoruz. Bu işlem, ADC'nin örnekleme hızını kontrol eder.

```c
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_8b;

    ADC_Init(ADC1, &ADC_InitStruct);
    ADC_Cmd(ADC1, ENABLE);
}
```
* **ADC_Resolution_8b :** ADC'nin çözünürlüğü 8 bit olarak ayarlanmış, yani ADC 0 ile 255 arasında bir değer döndürecektir.
* **ADC_Cmd(ADC1, ENABLE) :** ADC1 modülü etkinleştiriliyor.

------------------------------------------------------------------------------------------------------------------------------------------------------------

## 5. Read_ADC Fonksiyonu - Read_ADC
```c
uint16_t Read_ADC()
{
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_56Cycles);

    ADC_SoftwareStartConv(ADC1);

    while (!(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)));

    return ADC_GetConversionValue(ADC1);
}
```
* **ADC_RegularChannelConfig :** ADC'nin Channel 0 (PA0 pininde bağlı olan LDR) üzerinden veri okuması için yapılandırma yapılıyor.
* **ADC_SoftwareStartConv :** Yazılım tarafından ADC dönüşüm işlemi başlatılıyor.
* **ADC_GetFlagStatus :** Dönüşüm tamamlandığında, End Of Conversion (EOC) bayrağı kontrol ediliyor.
* **ADC_GetConversionValue :** Dönüşüm tamamlandıktan sonra ADC'den okunan değeri döndürüyor.

------------------------------------------------------------------------------------------------------------------------------------------------------------

## 6. main Fonksiyonu
```c
int main(void)
{
    GPIO_Config();
    ADC_Config();
    while (1)
    {
        ldr_value = Read_ADC();
```
* GPIO_Config ve ADC_Config fonksiyonları, GPIO ve ADC'yi yapılandırıyor.
* Sonsuz döngü içinde ADC'den LDR'nin değeri okunuyor.

```c
        if (ldr_value < 125)
        {
            GPIO_SetBits(GPIOA, GPIO_Pin_5);
            GPIO_ResetBits(GPIOA, GPIO_Pin_7);
        }
        else if (ldr_value >= 125)
        {
            GPIO_SetBits(GPIOA, GPIO_Pin_7);
            GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        }
```
* Okunan LDR değeri 125'ten küçükse GPIOA Pin 5'e bağlı LED yanıyor, GPIOA Pin 7'deki LED sönüyor.
* Okunan LDR değeri 125'e eşit veya büyükse, bu kez GPIOA Pin 7'deki LED yanıyor, GPIOA Pin 5'teki LED sönüyor.

------------------------------------------------------------------------------------------------------------------------------------------------------------

## ÖZET

* LDR'den analog sinyal okunuyor ve ADC1 modülü ile dijital değere çevriliyor.
* Okunan ADC değeri ışık seviyesine göre GPIOA Pin 5 ve Pin 7'ye bağlı LED'lerin durumunu kontrol etmek için kullanılıyor.
* LDR'ye gelen ışık miktarı arttıkça ldr_value değeri azalıyor ve bu değer 125'ten küçük olduğunda bir LED yanıyor, büyük olduğunda ise diğer LED yanıyor.


