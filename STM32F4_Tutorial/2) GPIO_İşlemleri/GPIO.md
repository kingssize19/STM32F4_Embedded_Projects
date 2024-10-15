## 1. GPIO Pinlerinin Yapısı

Her STM32F4 mikrodenetleyicisinde birden fazla GPIO portu bulunur ve bu portlar GPIOA, GPIOB, GPIOC gibi adlandırılır. Her port, mikrodenetleyici modeline bağlı olarak 16 pine kadar destek verebilir (PA0, PA1, PA2, ... PA15 gibi). Her bir pin şu modlarda çalıştırılabilir:
* **Input (Giriş)  :** Pin, dış bir kaynaktan veri alır.
* **Output (Çıkış) :** Pin, dışarıya veri gönderir.
* **Alternate Function (Alternatif Fonksiyon) :** Pin, GPIO harici bir fonksiyon için kullanılır (UART, SPI, I2C, PWM vb.).
* **Analog :** Pin, analog veri okumak veya çıkış vermek için kullanılır (ADC veya DAC).

--------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 2. GPIO Modlarının Yapılandırılması

Her bir pin için çalışma modu, hızı, çekme direnci (pull-up/pull-down) ve çıkış tipi gibi parametreler ayarlanabilir.

**GPIO Modları :**

STM32F4'te her pin, çalıştırılacağı moda göre konfigüre edilir. Bu modlar:
* **Input Mode (Giriş Modu) :** Pin sadece veri almak için kullanılır.
* **Output Mode (Çıkış Modu) :** Pin sadece veri çıkışı yapmak için kullanılır.
* **Alternate Function Mode (Alternatif Fonksiyon Modu) :** Pin bir periferik tarafından kullanılacaksa (örneğin, UART, SPI, I2C gibi) bu mod seçilir.
* **Analog Mode (Analog Modu) :** ADC veya DAC işlemleri için bu mod kullanılır.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------

**Çıkış Modu Türleri :**

* **Push-Pull (Diyot ile Çıkış) :** Pin, aktif olarak lojik 1 veya lojik 0 sinyali üretir.
* **Open-Drain (Açık Dirençli Çıkış) :** Pin, sadece lojik 0 sinyali üretebilir; lojik 1 ise dış bir dirençle sağlanır.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------

**GPIO Pin Hızları :**

GPIO pinlerinin çıkış hızları ayarlanabilir. Farklı hız ayarları şunlardır:
* **Low Speed (Düşük Hız)   :** Düşük hız ve enerji tasarrufu gerketiren uygulamalar için kullanılır.
* **Medium Speed (Orta Hız) :** Genellikle standart uygulamalar için kullanılır.
* **High Speed (Yüksek Hız) :** Yüksek hız gerektiren veri yollarında (SPI, I2C) kullanılır.
* **Very High Speed (Çok Yüksek Hız) :** Kritik zamanlama gerektiren yüksek hızlı veri işlemlerinde kullanılır.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------

**GPIO Çekme Dirençleri (Pull-up/Pull-down) :**

GPIO pinleri, iç dirençlerle lojik seviyeye çekilebilir. Bu dirençler, devrenin dışarıdan belirli bir gerilim kaynağına çekilmesine gerek kalmadan, pinin lojik seviyesini sabitler. Üç ana seçenek vardır:
* **No Pull :** Pinin lojik seviyesi serbest bırakılır.
* **Pull-up :** Pin lojik 1 seviyesine çekilir.
* **Pull-down :** Pin lojik 0 seviyesine çekilir.


