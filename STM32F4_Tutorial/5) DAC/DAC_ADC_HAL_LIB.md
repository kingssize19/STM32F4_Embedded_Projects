**Bu kod, STM32 mikrodenetleyicisinde ADC (Analog-to-Digital Converter) ve DAC (Digital-to-Analog Converter) çevrim işlemlerini gerçekleştiren temel bir uygulama sunuyor. Kod, ADC’den okunan bir analog değeri dijital hale çevirip, bu değeri DAC’e yazarak tekrar analog sinyal olarak üretiyor. Adım adım inceleyelim:**

## 1. Değişken Tanımları
```c
uint16_t adc_value, dac_value;
```
* **adc_value:** ADC’den okunan 16 bitlik dijital değeri tutar.
* **dac_value:** DAC’e yazılacak dijital değeri tutar.

## 2. ADC ve DAC Donanım Yapılandırması

Bu yapılandırma fonksiyonları HAL (Hardware Abstraction Layer) kütüphanesi kullanılarak otomatik olarak oluşturulmuştur:

* ADC Konfigürasyonu (MX_ADC1_Init):
  * ADC, 12-bit çözünürlükte ayarlanmış ve ADC_CHANNEL_0 kanalından okuma yapacak şekilde yapılandırılmış.
  * HAL_ADC_PollForConversion fonksiyonu ile yazma-okuma işlemleri yapılır.

* DAC Konfigürasyonu (MX_DAC_Init):
  * DAC çıkış tamponu (DAC_OUTPUTBUFFER_ENABLE) açık durumda ve DAC_CHANNEL_1 kanalında çalışıyor.

## 3. read_adc() Fonksiyonu
Bu fonksiyon ADC’den veri okumak için tanımlanmıştır:
```c
uint16_t read_adc()
{
    HAL_ADC_Start(&hadc1); // ADC’yi başlat
    if (HAL_ADC_PollForConversion(&hadc1, 100000) == HAL_OK) // Dönüşüm tamamlanana kadar bekle
        adc_value = HAL_ADC_GetValue(&hadc1); // ADC değerini oku
    HAL_ADC_Stop(&hadc1); // ADC’yi durdur
    return adc_value; // Okunan değeri döndür
}
```
* HAL_ADC_Start: ADC’yi başlatır.
* HAL_ADC_PollForConversion: ADC çevriminin tamamlanmasını bekler. Tamamsa, HAL_OK döner.
* HAL_ADC_GetValue: Çevrim sonucu oluşan dijital değeri okur.
* HAL_ADC_Stop: ADC işlemini sonlandırır.

## 4. main Fonksiyonu
Ana fonksiyon işlem sırasını yönetir:
```c
int main(void)
{
    HAL_Init(); // HAL kütüphanesi ve sistem yapılandırmalarını başlatır
    SystemClock_Config(); // Sistem saatini yapılandırır
    MX_GPIO_Init(); // GPIO’ları başlatır
    MX_ADC1_Init(); // ADC’yi başlatır
    MX_DAC_Init(); // DAC’i başlatır

    HAL_DAC_Start(&hdac, DAC_CHANNEL_1); // DAC kanalını başlat

    while (1)
    {
        dac_value = read_adc(); // ADC değerini oku
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_value); // DAC’e yaz
    }
}
```
Bu döngüde read_adc() fonksiyonu ile ADC’den okunan değer dac_value değişkenine atanır, ardından DAC’e yazılır.


## 5. SystemClock_Config Fonksiyonu
Sistem saati yapılandırmasını yapar ve STM32’nin çekirdek çalışma hızını belirler:
* HSI (High-Speed Internal) osilatörü aktif halde kullanır.
* PLL (Phase-Locked Loop) yapılandırması ile sistem saatini 180 MHz hızında çalıştırır.































