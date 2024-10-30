### Bu kod, STM32 mikrodenetleyici ile bir potansiyometre ve ışık sensöründen ADC (Analog-Digital Converter) verilerini okuyup, okunan değerleri UART üzerinden 1 saniye aralıklarla bilgisayara iletmek için yazılmıştır.

## 1. Genel Yapı ve Kütüphaneler

* Kod, HAL (Hardware Abstraction Layer) kütüphaneleri ile yazılmıştır. Bu kütüphaneler, donanım katmanını daha kolay kontrol etmek için STMicroelectronics tarafından sağlanır.
* #include <stdio.h> ve #include <string.h> kütüphaneleri, UART üzerinden veri iletimi sırasında kullanılacak mesajların formatlanması için eklenmiştir.

--------------------------------------------------------------------------------------------------------------------------------------------------------

## 2. Değişken Tanımları

* lux ve pot adlı uint16_t tipinde iki değişken tanımlanmış. Bu değişkenler sırasıyla ışık sensörü ve potansiyometre ADC verilerini depolar.
* msg, UART üzerinden gönderilecek mesajın saklandığı char dizisidir.
* raw_values, ADC'den gelen ham verileri almak için kullanılan uint8_t dizisidir.

--------------------------------------------------------------------------------------------------------------------------------------------------------

## 3. Donanım Yapılandırması

* Kodda kullanılan donanımların yapılandırılması (ADC, UART, GPIO, System Clock) için özel fonksiyonlar tanımlanmış ve HAL kütüphanesi kullanılarak başlatılmış.

**3.1 SystemClock_Config**

* Bu fonksiyon, sistem saatinin yapılandırılmasından sorumlu. Mikrodenetleyicinin HSI (High-Speed Internal) osilatörü kullanılarak sistemin çalışması için gerekli saat frekansı yapılandırılmıştır.

**3.2 SystemClock_Config**

Bu fonksiyon, ADC1 yapılandırmasını yapar:
* ADC_CHANNEL_0 (potansiyometre) ve ADC_CHANNEL_1 (ışık sensörü) olmak üzere iki kanal tanımlanmıştır.
* Çözünürlük 12-bit olarak ayarlanmış ve iki kanalın sıralı okuma yapabilmesi için ScanConvMode etkinleştirilmiştir.
* ADC_SAMPLETIME_480CYCLES, ADC'nin örnekleme süresi olarak ayarlanmıştır.


**3.3 SystemClock_Config**

* Bu fonksiyon, UART iletişim modülünü yapılandırır. UART yapılandırması, 115200 baud hızı ve 8N1 (8 veri biti, parite yok, 1 dur bit) protokolüne göre yapılmıştır.

**3.4 SystemClock_Config**

* Bu fonksiyon, GPIO pinlerinin yapılandırılması için kullanılır. Burada GPIO ayarları başlatılmıştır fakat kullanılmamıştır.

--------------------------------------------------------------------------------------------------------------------------------------------------------

## 4. HAL_ADC_ConvCpltCallback Fonksiyonu

Bu callback fonksiyonu, ADC dönüştürme tamamlandığında otomatik olarak çağrılır. Bu fonksiyon sayesinde ADC dönüşümü tamamlandığında, okunan değer uygun sensör değişkenine atanır.
* channel_index, iki sensör arasında dönüşümlü olarak okumalar yapılmasını sağlar.
* channel_index sırasıyla 0 olduğunda potansiyometre, 1 olduğunda ışık sensörü verilerini okur.
* Okuma tamamlandıktan sonra HAL_ADC_Start_IT(hadc) fonksiyonu tekrar çağrılarak kesme modunda ADC başlatılır.

--------------------------------------------------------------------------------------------------------------------------------------------------------

## 5. main Fonksiyonu 

main fonksiyonu, tüm işlemlerin yapıldığı ana döngüyü içerir.
* HAL_ADC_Start_IT(&hadc1); ifadesi, ADC'yi kesme modunda başlatarak ilk dönüşümün başlamasını sağlar.
* while (1) sonsuz döngüsü içinde potansiyometre (pot) ve ışık sensörü (lux) verileri UART üzerinden iletilir.
  * sprintf(msg, "Pot = %hu\r\n", pot); ve sprintf(msg, "Light = %hu\r\n", lux); ile her iki sensör için mesaj formatlanır.
  * HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY); ifadesi, mesajın UART üzerinden bilgisayara iletilmesini sağlar.
  * HAL_Delay(1000); ifadesiyle her veri iletimi arasına 1 saniye gecikme eklenir.

--------------------------------------------------------------------------------------------------------------------------------------------------------

## Özet

Bu kodun amacı, STM32 mikrodenetleyici ile ADC okuma işlemi gerçekleştirip, iki farklı sensörden gelen veriyi UART üzerinden sürekli olarak bilgisayara iletmektir. Sensörlerin ölçtüğü değerler main döngüsünde belirli aralıklarla UART üzerinden gönderilir ve böylece kullanıcı, her iki sensörden alınan verileri bilgisayarda görebilir.
























