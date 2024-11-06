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

----------------------------------------------------------------------------------------------------------------------------------------------------

## TIMER ÇALIŞMA MODLARI

**1. Upcounter Mode (Yukarı Sayma Modu)**

Bu modda timer, sıfırdan başlar ve AutoReload Register (ARR) değerine kadar sayar. CNT register'ı ARR değerine ulaştığında sıfırlanır ve bir güncelleme (update) olayı tetiklenir. Bu mod periyodik olaylar veya kesmeler oluşturmak için kullanılır. 
* **Kullanım Alanları :** Zamanlayıcı, periyodik kesme oluşturma.

----------------------------------------------------------------------------------------------------------------------------------------------------

**2. Downcounter Mode (Aşağı Sayma Modu)**

Aşağı sayma modunda timer, ARR register'da tanımlı değerden başlayarak sıfıra doğru sayar. CNT sıfıra ulaştığında sıfırlanır ve yine bir güncelleme (update) olayı tetiklenir.
* **Kullanım Alanları:** Geri sayım uygulamaları veya periyodik işlemler.

----------------------------------------------------------------------------------------------------------------------------------------------------

**3. Center-Aligned Mode (Merkez Hizalamalı Mod)**

Bu modda timer, ARR değerine kadar yukarı doğru sayar, ardından sıfıra doğru geri sayar. Yani bir yukarı ve bir aşağı sayma döngüsü oluşturur. Bu sayede sinyaller merkezde hizalanır ve daha düzgün bir PWM sinyali elde edilebilir. Timer yukarı ve aşağı sayarken Compare Match olayları tetiklenir.

* **Kullanım Alanları:** PWM sinyal üretimi, motor sürücüleri gibi hassas uygulamalarda simetrik sinyal üretimi.

----------------------------------------------------------------------------------------------------------------------------------------------------

**4. One-Pulse Mode (Tek Darbe Modu)**

Bu mod, timer’ın yalnızca bir defa belirli bir pulse (darbe) üretmesi için kullanılır. One-Pulse Mode (OPM) aktifleştirildiğinde, timer bir olayla tetiklenir ve belirli bir darbe üretir. Bu darbeyi ürettikten sonra timer otomatik olarak durur. Tek darbe üretiminde tetikleyici bir giriş veya program üzerinden bir olay kullanılabilir.

* **Kullanım Alanları:** Sensör tetikleme, darbe üretimi, belirli aralıkta bir işlem gerçekleştirme.

----------------------------------------------------------------------------------------------------------------------------------------------------

**5. Output Compare Mode (Çıkış Karşılaştırma Modu)**

Bu modda timer, belirli bir değerle (örneğin ARR değeri) çıkış değerini karşılaştırır ve belirli bir koşul sağlandığında çıkışı değiştirir. Bu, zamanlama kontrolü ve periyodik işlemler için kullanılır. Output Compare (OC) modları ile farklı çıkış davranışları (toggle, set, reset) oluşturabilirsiniz.

* **Kullanım Alanları:** Kesme tetikleme, periyodik görevler veya zamanlayıcı bazlı işlemler.

----------------------------------------------------------------------------------------------------------------------------------------------------

**6. Input Capture Mode (Giriş Yakalama Modu)**

Input Capture Mode, belirli bir giriş sinyalinin zamanını yakalamak için kullanılır. Timer, giriş pinine uygulanan sinyalin yükselen veya alçalan kenarında CNT değerini kaydeder. Bu mod, darbe genişliği veya frekans ölçümü gibi zamanlama işlemlerinde kullanılır.

* **Kullanım Alanları:** Frekans ölçümü, darbe genişliği ölçümü, olayların zamanını tespit etme.

----------------------------------------------------------------------------------------------------------------------------------------------------

**7. PWM Mode (Pulse Width Modulation - Darbe Genişlik Modülasyonu)**

PWM modunda timer, PWM sinyalleri üretmek için kullanılır. PWM sinyalinin periyodu ARR değeri ile belirlenirken, duty cycle (görev döngüsü) çıkış karşılaştırma (Output Compare) registerları ile ayarlanır. Duty cycle ayarları sayesinde, sinyalin ne kadar süre boyunca açık veya kapalı kalacağı belirlenebilir.

* **Kullanım Alanları:** Motor sürme, LED parlaklık kontrolü, güç kontrol uygulamaları.

----------------------------------------------------------------------------------------------------------------------------------------------------

**8. Encoder Interface Mode (Enkoder Arabirimi Modu)**

Encoder Mode, enkoderlerden okuma yapmak için özel olarak tasarlanmıştır. Timer, enkoderin hareket yönünü ve konumunu belirlemek için A ve B fazlarını okur. Bu mod, motor hareketini veya belirli bir eksendeki hareketi ölçmek için kullanılır.

* **Kullanım Alanları:** Motor geri bildirimi, pozisyon ölçümü, hız ölçümü.

----------------------------------------------------------------------------------------------------------------------------------------------------

**9. Slave Mode (Köle Modu)**

Timer’lar bazen slave mode (köle modu) olarak yapılandırılabilir ve bir başka timer veya tetikleyici sinyal ile senkronize çalışabilir. Bu mod, çoklu timer senkronizasyonu veya başka bir olayın tetiklemesiyle işlemleri başlatmak için kullanılır.

* **Kullanım Alanları:** Çoklu timer senkronizasyonu, olay bazlı tetiklemeler.

----------------------------------------------------------------------------------------------------------------------------------------------------

## Özet Tablo

| Mod | Açıklama | Kullanım Alanları |
|--|--|--|
| Upcounter Mode | 0’dan yukarı doğru sayar, ARR’ye ulaşınca sıfırlanır. | Periyodik işlemler |
| Downcounter Mode	| ARR’den aşağı doğru sayar, sıfıra ulaştığında sıfırlanır.	| Geri sayım işlemler | 
| Center-Aligned Mode	| Yukarı ve aşağı doğru sayarak simetrik sinyal oluşturur.	| PWM üretimi |
| One-Pulse Mode	| Tek seferlik darbe üretir, tetikleme ile çalışır ve ardından durur. |	Darbe üretimi |
| Output Compare Mode |	Timer ile belirli bir değeri karşılaştırarak kesme veya çıkış sinyali üretir. |	Zamanlayıcı tabanlı işlemler |
| Input Capture Mode |	Giriş sinyalinin zamanını yakalar, sinyalin genişliğini veya frekansını ölçer. |	Frekans ve darbe genişliği ölçümü |
| PWM Mode	| PWM sinyali üretir, duty cycle ve periyod kontrol edilir. |	Motor sürme, LED parlaklık kontrolü |
| Encoder Interface Mode |	Enkoder sinyallerini okuyarak yön ve konum bilgisi sağlar. |	Pozisyon ve hız ölçümü |
| Slave Mode |	Başka bir timer veya olayla senkronize çalışır. |	Çoklu timer senkronizasyonu |

STM32 timer modları, çok çeşitli zamanlama ve sinyal işleme uygulamalarında kullanılır ve her mod belirli bir ihtiyaca göre optimize edilmiştir. Bu modları doğru şekilde yapılandırarak, mikrodenetleyici tabanlı projelerinizde hassas zamanlama ve kontrol sağlayabilirsiniz.









