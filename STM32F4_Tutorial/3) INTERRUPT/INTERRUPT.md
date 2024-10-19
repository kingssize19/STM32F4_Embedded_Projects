# INTERRUPT (KESME)

* Önceliği yüksek işlerin mikrodenetleyici tarafından ana program akışını keserek yapılmasına interrupt (kesme) denir.
* Eğer bir kesme kaynağından mikrodenetleyiciye uyarı gelirse mikrodenetleyici yapmakta olduğu işi bekletir, kesme alt programına gider, o programı icra eder, daha sonra ana programda kaldığı yerden devam eder.
* Kesmeleri genellikle çok hızlı yapılması gereken işlemlerde, anlık tepki verilmesi gereken yerlerde kullanırız.
* Düşünün ki araç içerisinde gidiyorsunuz ve frene bastınız. Anlık tepki almanız ve anlık olarak yerine gelmesi gerekiyor, kesmeye gidilmeli...

--------------------------------------------------------------------------------------------------------------------------------------------------------------------

## STM32F4 INTERRUPT ÖZELLİKLERİ

1. STM32F446RE Nucleo kartındaki mikrodenetleyicide 82 adet maskelenebilir kesme kanalı vardır.
   * STM32F446RE mikrodenetleyicisi de toplamda 82 adet maskelenebilir kesme (interrupt) kanalı sunar. Bu kanallar, hem dahili çevresel birimler (USART, TIMER, ADC gibi) hem de harici kesme kaynakları (GPIO pinleri) için kullanılır.

2. Programlanabilir 16 öncelik seviyesi verilebilir.
   * NVIC, 4-bit öncelik alanı ile birlikte gelir, yani kesmeler için 16 adet programlanabilir öncelik seviyesi belirlenebilir. Her bir kesme kanalı için, bu öncelik seviyeleri tanımlanarak daha kritik olan kesmelere öncelik verilebilir.

3. Kısa gecikme süreli kesme erişimine sahiptir.
   * STM32F446RE, düşük gecikmeli kesme erişimi sağlar. Bu, NVIC mimarisi sayesinde kesme isteklerinin hızlı bir şekilde işlenebilmesi anlamına gelir. Kritik kesmeler, daha düşük öncelikli kesmelerle çakıştığında dahi hızlı bir şekilde işlenebilir.

4. Güç kontrol kesmelerine sahiptir.
   * STM32F4 ailesindeki tüm mikrodenetleyiciler gibi, STM32F446RE de güç yönetimi ile ilgili kesmeleri destekler. Mikrodenetleyici, çeşitli güç modlarına geçiş sırasında veya düşük güç modlarında bazı olaylara kesmelerle yanıt verebilir. Örneğin, uyku moduna geçtikten sonra bir kesme olayıyla (örn. harici bir sinyal) yeniden aktif hale gelebilir.

5. Sistem kontrol kesmelerine sahiptir.
  * Sistem kontrol kesmeleri, mikrodenetleyicinin önemli olaylara (örneğin hatalar, reset olayları, Systick timer kesmesi) yanıt verebilmesi için kullanılır. STM32F446RE de bu kesmeleri destekler:
    * **NMI (Non-Maskable Interrupt) :** Maskelenemeyen kesmeler için kullanılır.
    * **HardFault :** Ciddi hata durumlarında tetiklenen bir kesme.
    * **Memory Management Fault :** Bellek yönetimi ile ilgili hatalar için kullanılır.
    * **BusFault :** Veri yolu hatası olduğunda tetiklenir.
    * **UsageFault :** Yanlış komut veya geçersiz işlem gerçekleştirildiğinde tetiklenen kesme.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------

# HARİCİ KESMELER (EXTERNAL INTERRUPT) \[ EXTI \]









