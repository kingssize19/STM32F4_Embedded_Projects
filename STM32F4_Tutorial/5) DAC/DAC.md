STM32F446RE Nucleo kartında yer alan DAC (Digital-to-Analog Converter), sayısal (dijital) sinyalleri analog sinyallere dönüştüren bir modüldür. Bu modül, dijital verilerin analog olarak ses, sinyal veya voltaj gibi çıktılar olarak kullanılması gereken uygulamalarda önemlidir. DAC modülü, özellikle ses üretimi, kontrol devreleri ve analog sinyal simülasyonları gibi işlemlerde faydalıdır.

## STM32F446RE Nucleo DAC Özellikleri
STM32F446RE mikrodenetleyici, 12-bit çözünürlüğe sahip bir adet DAC modülüne sahiptir. İşte DAC özelliklerinin ayrıntıları:

**1. 12-bit Çözünürlük :**
* 12-bit çözünürlük, 0 ile 4095 arasında bir değer aralığında çıkış verebilir. Bu, daha hassas analog sinyaller üretebileceğiniz anlamına gelir.
* 12-bitlik çözünürlük sayesinde, DAC çıkışında analog sinyaller 4096 farklı seviyede ayarlanabilir.

**2. 2 Kanal Destekli :**
* STM32F446RE DAC modülü, DAC1 ve DAC2 olarak bilinen iki bağımsız kanala sahiptir. Bu kanallar aracılığıyla iki farklı analog sinyal oluşturulabilir.
* Her iki kanal da bağımsız olarak yapılandırılabilir ve farklı dijital verilere göre çalıştırılabilir.

**3. Çıkış Modları :**
* DAC modülü, çıkış sinyalleri için çeşitli modlar sunar:
  * **Normal Çıkış Modu :** Dijital veriyi doğrudan analog sinyale dönüştürür.
  * **Üçgen Dalga Üretimi :** Kullanıcı tarafından belirlenen genlik ve frekansta üçgen dalga çıkışı oluşturabilir.
  * **Gürültü Üretimi :** Rastgele gürültü sinyalleri üretmek için kullanılır; test ve simülasyon gibi uygulamalarda yararlıdır.

**4. Örnekleme Frekansı :**
* DAC modülü, zamanlayıcı veya DMA ile birlikte kullanılarak belirli bir örnekleme hızında çalışabilir. Bu, özellikle sinyal üretiminde sürekli veri akışı sağlamak için kullanılır.

**5. Tamponlama Modu :**
* Tamponlama (buffer) modu, çıkış empedansını düşürerek DAC çıkışının yük etkilerine karşı daha stabil çalışmasını sağlar. Bu mod, düşük empedanslı yüklerde daha kararlı bir sinyal sağlamak için kullanılabilir.
* Tamponlama modunu açıp kapatma seçeneği vardır, böylece çıkış empedansı uygulamaya göre optimize edilebilir.

**6. Trigger Özellikleri :**
* DAC modülü, belirli olaylarla tetiklenerek çalıştırılabilir. Bu tetikleyiciler arasında yazılım tetikleme, harici sinyal tetikleme ve çeşitli zamanlayıcı tetiklemeleri bulunur.
* Tetikleyici sinyaller, sinyal üretimini belirli bir zamana veya duruma göre senkronize etmek için kullanılır.

**7. DMA Destekli Veri Aktarımı :**
* STM32F446RE’nin DAC modülü, DMA (Direct Memory Access) ile birlikte çalışarak yüksek hızda ve kesintisiz veri aktarımı sağlar. Bu özellik, sürekli veri akışının gerekli olduğu uygulamalarda CPU yükünü azaltır.

**8. Çıkış Gerilim Aralığı :**
* DAC modülü, referans voltajına (Vref+) bağlı olarak 0V ile Vref arasında bir analog çıkış sinyali üretir. Genellikle Vref+, STM32 kartlarının besleme voltajı olan 3.3V olarak ayarlanır.

-------------------------------------------------------------------------------------------------------------------------------------------------

## DAC Kullanım Alanları
STM32F446RE üzerindeki DAC modülü, aşağıdaki uygulamalarda kullanılabilir:

* **Ses Üretimi:** Dijital ses verilerini analog ses sinyallerine çevirme
* **Analog Sinyal Üretimi:** Test ve simülasyon uygulamalarında belirli bir sinyal üretimi
* **Kontrol Sistemleri:** Motor sürücüleri ve kontrol devrelerinde analog kontrol sinyalleri üretme
* **Dalga Üretimi:** Üçgen veya sinüs dalgaları gibi sinyallerin üretilmesi
* **Sensör Simülasyonu:** Analog sensör verilerinin simüle edilmesi, böylece cihazlar gerçek sensörler bağlıymış gibi davranabilir

DAC modülü, STM32F446RE gibi mikrodenetleyicilerde dijital verilerin analog sinyallere dönüştürülmesi gereken birçok uygulamada kullanılabilecek güçlü bir araçtır. Bu modülü kullanarak, mikrodenetleyicinin analog dünyayla daha uyumlu çalışmasını sağlayabilirsiniz.

-------------------------------------------------------------------------------------------------------------------------------------------------

* DAC birimleri APB1 veri yoluna bağlıdır, kullanılmak için aktif etmek gereklidir.
* 12 bitlik çözünürlüğe sahip bir DAC biriminin referans gerilimleri Vssa = 0 V, Vdda = +3 V ele alınır ise, adım başına üreteceği voltaj şu şekilde hesaplanır:
  * DACout = Vref / 4095

* Buradan adım başına düşen voltaj:
  * DACout = 3V / 4095 = 732,600732... mV

* Buradan istenilen voltajı elde etmek için voltaj değeri adım başına üretilen voltaj değerine bölünerek elde edilir.
  * Örneğin 1V elde elde etmek isteniyorsa:
    * 1 / 0,000732600... = 1365 değeri elde edilir.

