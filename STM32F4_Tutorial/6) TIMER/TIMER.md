## TIMER

* Timer modülünün temel görevi zamanlama yapmaktır.
* İşlemci frekansına bağlı olarak çalışırlar.
* Dışarıdan gelen pulse (darbe) leri sayarlar.
* İşlemciye tanıtılan bir süre ile, geçen süreyi karşılaştırma ve belli bir süre sonunda kesme üretme gibi işlemlerde kullanılırlar.
* Çoğu mikrodenetleyicide PWM (Pulse Width Modulation) birimleri de Timer ünitelerine bağlı olarak çalışırlar.

* STM32F446RE mikrodenetleyicisinde toplam 17 adet timer bulunur.
* 10 adet genel amaçlı timer.
* 2 adet gelişmiş timer.
* 1 adet IWDG (Independent Watchdog) timer.
* 1 adet WWDG (Window Watchdog) timer.
* 1 adet Systemtick (Sistem Zamanlayıcısı) timer var.

| TIMER | ÖZELLİK |
| -- | -- |
| TIM1 ve TIM8 | 16 Bit Gelişmiş Timer |
| TIM3, TIM4, TIM9..TIM14 | 16 Bit Genel Amaçlı Timer |
| TIM2 ve TIM5 | 32 Bit Genel Amaçlı Timer |
| TIM6 ve TIM7 | 16 Bit Basit Timer |


----------------------------------------------------------------------------------------------------------------------------------------------------------------

### STM32F4 TIMER BİRİMLERİNİN ÖZELLİKLERİ

1 - Genel Amaçlı Timer2, Timer3, Timer4 ve Timer5 Birimleri



