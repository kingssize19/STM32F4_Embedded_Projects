## Bu repo'da NUCLEO F446RE KARTI ile FreeRTOS uygulamaları yer almaktadır.

* **FreeRTOS_LED_BLINK :** İki adet led kullanılarak polis flaşör devresi tasarlanmıştır. main.c dosyası klasör içerisinde yer almaktadır. Proje .zip formatında klasör içinde bulunmaktadır. **NOT :** Proje HAL kütüphanesi kullanılarak tasarlanmıştır.


* **FreeRTOS_CAR_PARKING_SYSTEM :** Projede HCSR04 ile mesafe ölçülmüştür. Bu ölçülen mesafe değerine göre;
    * mesafe 20-30 cm arasında ise Buzzer uzun aralıklarla çalıştırılsın ve sadece yeşil led yakılsın (Tehlike Az!).
    * mesafe 10-20 cm arasında ise Buzzer daha hızlı çalıştırılsın ve yeşil led ile birlikte sarı led yakılsın (Tehlike Orta!!).
    * mesafe 0-10 cm arasında ise Buzzer çok hızlı şekilde çalıştırılsın be yeşil sarı kırmızı ledlerin hepsi yakılsın (Tehlike Yüksek!!!).
    * main.c dosyası klasör içerisinde yer almaktadır. Proje .zip formatında klasör içerisinde bulunmaktadır. **NOT :** Proje HAL kütüphanesi kullanılarak tasarlanmıştır.  
