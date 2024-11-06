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


-----------------------------------------------------------------------------------------------------------------------------------------------------

### STM32F4 TIMER BİRİMLERİNİN ÖZELLİKLERİ

**1. Genel Amaçlı Timer2, Timer3, Timer4 ve Timer5 Birimleri**
* Timer 2, 3, 4 ve Timer 5 birimleri, düşük hızlı APB1 (42 MHz) veri yolu üzerinde bulunmaktadır. Eğer APB1 prescaler (bölücü) değeri 1 den farklı ise bu timerların clock frekansları beslendikleri frekansların 2 katına çıkar. Yani 84 MHz clock frekansına sahip olur.
* Timer3 (TIM3) ve Timer4 (TIM4) 16-bit'lik sayıcıya, Timer2 (TIM2) ve Timer5 (TIM5) 32-bit'lik sayıcıya sahiptirler.
* Bu sayıcılar yukarı (up), aşağı (down) ve hizalanmış/merkezlenmiş (auto-reload) modlarda sayma yapabilirler.
* Ayrıca bu sayıcıların otomatik yükleme özellikleri de vardır.
* 16-bit genişliğinde kontrol edilebilir ön bölücü değeri (prescaler) vardır.
* Bu timer biriminde 4x16 adet yüksek çözünürlükte capture/compare kanalı bulunur. Bu kanallar;
  * Giriş Yakalama (Input Capture)
  * Çıkış Karşılaştırma (Output Compare)
  * Darbe Genişlik Modülasyonu (PWM)
  * Tek Darbe Çıkışı (One-Pulse)
* Dahili diğer Timer birimleri ile senkronize şekilde çalışır.
* Interrupt (Kesme) ve DMA üretimi
* Clock kaynağı seçimi

-----------------------------------------------------------------------------------------------------------------------------------------------------

### TEMEL ÇALIŞMA MANTIĞI 

STM32F446RE gibi STM32 serisi mikrodenetleyicilerde, TIMx_CNT, TIMx_PSC ve TIMx_ARR gibi timer (zamanlayıcı) register' ları, belirli zamanlama veya PWM (Pulse Width Modulation) gibi uygulamalar için kullanılır. Her biri farklı bir işlevi yerine getirir. Aşağıda her birinin işlevini detaylı bir şekilde açıklayalım:

**1. TIMx_CNT (Counter Register)**

TIMx_CNT, zamanlayıcının mevcut sayma değerini tutan kayıttır. Bu değer, timer çalışırken sürekli olarak artar veya azalır (moduna göre). TIMx_CNT, belirlenen başlangıç değeri ile AutoReload Register (ARR) arasında çalışır. Bu register'in temel işlevi, sayım işlemini gerçekleştirmektir.

* Çalışma Prensibi :
  * Timer çalışmaya başladığında **CNT** register'ı sıfırdan başlayarak **ARR** değerine kadar (veya tersine) sayar.
  * CNT, ARR değerine ulaştığında sıfırlanır (veya yeniden başlatılır), bu bir overflow (taşma) olarak adlandırılır.
  * Taşma meydana geldiğinde bir interrupt (kesme) oluşturulabilir, bu sayede belirli periyotlarda bir işlem gerçekleştirilir.

* Örnek Kullanım:
  * ```c
    uint32_t current_count = TIM2->CNT; // TIM2'nin mevcut sayım değerini al
    ```
    Bu satır, TIM2 zamanlayıcısının o anki sayım değerini elde eder.





