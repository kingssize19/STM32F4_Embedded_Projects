## ADC - \[ANALOG DGITAL CONVERTER\]

* Doğada var olan bütün fiziksel büyüklükler (ısı, ışık, ses, zaman vs.) analog büyüklük kavramına girer.
* Dünyadaki herhangi birşeyi dijital sistemlerimiz ile ölçmek, değerlendirmek, işlemek ve bu değerlere göre işlem yapabilmek için ADC'ye (Analog Digital Converter) ihtiyaç vardır.
* ADC, analog sinyali kullandığımız dijital sistemimizin algılayabileceği dijital veri haline dönüştürür.

--------------------------------------------------------------------------------------------------------------------------------------------------------------

* Analog büyüklüklerde iki nokta arasında sonsuz tane değer bulunur.
* Örneğin; 1 derece ile 2 derece arasında sonsuz değer vardır.
* Dijital büyüklükte ise; matematiksel değerinde aralık bulunmaz.
* Dijital bilgi 1 ve 0 dan oluşan bilgidir.
* Analogtan dijitale çevrilen her bilgi bu sebepten dolayı belli bir miktar kayba uğrar.
* Sonuçta dijital bilgi, analog aslına göre çok kısıtlı aralıklarda değerler alır ve örneklediği analog sinyale göre daha az bilgi içerir. Bu sebeple;
  * Analog kayıtla, dijital kayıtlara göre daha kalitelidir.
  * Analog kayıtlar : Sinema film, plak vs.
  * Dijital kayıtlar : mp3, wav, dvd, vcd vs.

--------------------------------------------------------------------------------------------------------------------------------------------------------------

* ADC işlemleri için ADC modülleri vardır. Birçok mikroişlemcide dahili olarak bulunur.
* ADC modülleri gerek harici, gerek dahili olsun hepsi bir referans voltaja ihtiyaç duyarlar.
* Genellikle mikroişlemcilerde referans voltajı işlemcinin besleme gerilimidir. (AVR -> 5V, PIC -> 5V, STM32 -> 3.3V)
* Bu değer aynı zamanda ayarlar yapılarak harici olarak verilebilir.

--------------------------------------------------------------------------------------------------------------------------------------------------------------

## REFERANS VOLTAJI

* ADC'ler 10, 12, 16, 24 vb. bit çözünürlükte bulunurlar.
* STM32F4 discovery kartı üzerinde bulunan ADC'ler 6, 8, 10 ve 12 bit çözünürlükte çalışabilirler.
* STM32F4 discovery kartında referans voltajı 3.3V dur.
* ADC modülünün 10 bit olduğunu düşünelim. 2^10 = 1024 değeri okunacak maksimum değerdir.
* Yani ADC okuması yaparken 0V = 0, 3.3V = 1023 değeri ile bize döner.
* Buradan her bit bir değerinin alacağı voltaj değerini (çözünürlüğü) 3.3 / 1024 = 0,00322265625 olarak buluruz.
* Buradan da biz ADC modülünden okuduğumuz değeri bu ifade ile çarparsak voltaj değerini buluruz. Örneğin 640 okuduysak 640 * 0,00322265625 = 2,0625 V olduğunu buluruz.
* Gerçek hayatta bu değerler tutmayabilir. (Referans geriliminden dolayı)

--------------------------------------------------------------------------------------------------------------------------------------------------------------  

## ADC ÖZELLİKLERİ

**1. ADC Sayısı**
* STM32F446RE mikrodenetleyicisinde 3 adet bağımsız ADC bulunmaktadır. (ADC1, ADC2, ADC3).

**2. Çözünürlük**
* **12-bit çözünürlük :** Bu, giriş sinyalini 0-4095 arasında bir dijital değere dönüştürebileceği anlamına gelir.
* **Ayarlanabilir çözünürlük :** 12, 10, 8 veya 6 bit olarak ayarlanabilir. Çözünürlük ayarını yaparak örnekleme süresini optimize edebilirsiniz.

**3. Kanal Sayısı**
* STM32F446RE'nin her bir ADC modülü için 16 adede kadar analog giriş kanalı bulunmaktadır. Bu sayede çok sayıda analog sinyali aynı anda izleyebilirsiniz.

**4. Giriş Voltajı Aralığı**
* ADC giriş voltajı, referans voltajına bağlıdır. Giriş voltajı 0V ile VREF+ arasında olmalıdır. Genellikle VREF+ değeri 3.3V'tur, fakat bu mikrodenetleyicide kullanılan VDD'ye bağlıdır.

**5. Örnekleme Süresi (Sampling Time)**
* Her bir kanal için farklı örnekleme süreleri (sampling time) ayarlanabilir. Örnekleme süresi, ADC'nin girişteki sinyali ne kadar süre örnekleyeceğini belirler. STM32F446RE ADC, 3 çevrimden (clock cycles) 480 çevrime kadar örnekleme sürelerine sahiptir.

**6. Dönüşüm Hızı (Conversion Rate)**
* STM32F446RE ADC'nin maksimum dönüştürme hızı 2.4 MSPS'dir (Mega Samples Per Second), yani saniyede 2.4 milyon örnek alınabilir. Bu hız yüksek performanslı uygulamalarda ADC'yi kullanmayı mümkün kılar.

**7. ADC Modları**
* **Tekli dönüşüm modu (Single conversion mode):** Tek bir analog giriş için tek bir örnekleme ve dijital dönüşüm yapılır.
* **Sürekli dönüşüm modu (Continuous conversion mode):** Bir kez başlatıldıktan sonra ADC sürekli olarak çalışır ve yeni girişleri sürekli olarak dönüştürür.
* **Scan modu (Scan mode):** Birden fazla kanaldan veri toplamak için kullanılır. Bu modda seçilen tüm kanallardan sırayla veri toplanır.
* **Kesme modları (Interrupt mode):** ADC'nin tamamladığı dönüşümleri kesme olarak işleyebilir.
* **DMA destekli mod (Direct Memory Access):** ADC dönüşümlerini doğrudan belleğe yazmak için DMA kullanılır.

**8. Dış Tetikleme (External Triggering)**
* ADC'yi tetiklemek için dış tetikleme (external trigger) kaynakları kullanılabilir. Timer'lar veya GPIO pinleri dış tetikleme kaynağı olarak kullanılabilir.

**9. Veri Hizalama**
* ADC'den elde edilen dijital veriler sağa veya sola hizalanmış olarak depolanabilir (right-aligned or left-aligned).

**10. Sıcaklık Sensörü ve Vref Dahili Girişler**
* Dahili sıcaklık sensörü ve dahili referans voltajı (Vref) girişleri de mevcuttur ve bunlar ADC aracılığıyla ölçülebilir.

**11. DMA (Direct Memory Access) Desteği**
* ADC, veriyi CPU'yu kullanmadan doğrudan belleğe yazmak için DMA ile entegre edilebilir. Bu, hızlı ve yüksek verimli veri toplama işlemleri için gereklidir.

**12. Analog İzleme (Analog Watchdog)**
* ADC, belirli bir aralıkta izlenen analog sinyalin, belirlenen sınırların dışına çıkıp çıkmadığını kontrol etmek için Analog Watchdog özelliğine sahiptir.

**13. Çalışma Frekansı**
* ADC'nin çalışma frekansı, APB2 bus frekansı üzerinden bir bölme faktörü (prescaler) ile ayarlanır. Bu faktör 2, 4, 6 veya 8 olabilir.


**NOT :** Çözünürlük arttıkça ADC biriminin ölçüm hızı düşmektedir.

























