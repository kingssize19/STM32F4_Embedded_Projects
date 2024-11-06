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

-----------------------------------------------------------------------------------------------------------------------------------------------------

**2. TIMx_PSC (Prescaler Register)**

TIMx_PSC, zamanlayıcının giriş saat sinyalini bölen (bölme oranını ayarlayan) register'dır. Bu, timer'ın sayma hızını ayarlamak için kullanılır. Prescaler değeri sayesinde, timer daha yavaş bir frekansta sayar, bu da daha uzun periyotlar elde etmemizi sağlar.

* Çalışma Prensibi:
  * Timer'a giren saat sinyali (APB1 veya APB2 saat frekansı) PSC değeri ile bölünür.
  * PSC 'ye yazılan değere göre giriş frekansı bölünür. Örneğin, PSC = 7 olarak ayarlanırsa, saat sinyali 8'e bölünmüş olur.
  * Timer artık giriş frekansının 1/8 hızında çalışır.

* Örnek Kullanım:
  * ```c
    TIM2->PSC = 79; // TIM2 için prescaler değerini 79 yap
    ```
    Bu kod ile, eğer sistem saat frekansı 80 MHz ise, 80 MHz / (79 + 1) = 1 MHz'lik bir sayım frekansı elde edilir.

-----------------------------------------------------------------------------------------------------------------------------------------------------

**3. TIMx_ARR (AutoReload Register)**

TIMx_ARR, timer'ın ulaşacağı maksimum sayma değerini belirleyen kayıttır. Timer, CNT register'ı ARR değerine ulaştığında sıfırlanır (veya tersine sayıyorsa başlangıç değerine döner). Bu özellik, timer'ın belirli periyotlarda taşma yapmasını sağlar, bu da bir PWM sinyali oluşturmak veya belirli zaman periyotlarında kesme oluşturmak için kullanılır.

* Çalışma Prensibi:
  * CNT register'ı ARR değerine ulaştığında sıfırlanır ve update interrupt (güncelleme kesmesi) oluşur.
  * ARR'ye yazılan değer, timer'ın sayım süresini belirler. Bu değeri ayarlayarak timer'ın çalışma periyodunu kontrol edebiliriz.

* Örnek Kullanım:
  * ```c
    TIM2->ARR = 999; // TIM2'nin taşma (overflow) değeri 999 olarak ayarla
    ```
    Bu kod, TIM2'nin CNT değerinin 0'dan 999'a kadar saymasını sağlar. CNT 999'a ulaştığında sıfırlanır ve bu döngü tekrarlanır.

-----------------------------------------------------------------------------------------------------------------------------------------------------

**Birlikte Kullanımı**

Bu üç register birlikte çalışarak zamanlayıcıyı kontrol eder. Örneğin, bir saniyelik bir zamanlayıcı ayarlamak istiyorsanız:
* Prescaler değeri ile giriş frekansını düşürün.
* ARR değerini giriş frekansına göre ayarlayın.

Örneğin, sistem frekansınız 80 MHz ise, PSC’yi 7999 yaparak 80 MHz’lik frekansı 10 kHz’e düşürebilirsiniz. Bu durumda, ARR değerini 10.000 yaparsanız, timer her 1 saniyede bir taşma yapacaktır.

**ÖZET TABLO**
| Register | Açıklama |
|--|--|
| CNT | Mevcut sayım değeri, timer çalıştıkça artar veya azalır. |
| PSC | Timer giriş frekansını belirli bir oranda böler. |
| ARR | Timer'ın ulaşacağı maksimum değer; bu değere ulaştığında CNT sıfırlanır ve periyot tamamlanır. |

Bu üç register kullanılarak, mikrodenetleyici üzerinde belirli zaman aralıklarında işlemler yaptırmak veya PWM gibi uygulamalar gerçekleştirmek mümkündür.

-----------------------------------------------------------------------------------------------------------------------------------------------------
