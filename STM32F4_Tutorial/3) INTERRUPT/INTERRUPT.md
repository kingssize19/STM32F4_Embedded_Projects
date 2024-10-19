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

* Harici bir kaynaktan oluşan olaylardan dolayı meydana gelen kesmelere, harici kesmeler denir.
* STM32F446RE Nucleo kartı 23 adet harici kaynaktan kesme kabul etmektedir.
* Harici kaynak olarak, dış ortamdan pinler vasıtasıyla gelecek kesme ve kendi içindeki donanımlardan gelen kesmeleri anlayabiliriz.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------

**NOT :** Bütün portların x. pinleri EXTIx hattına bağlıdır. Yani A portunun 0'ıncı pini ile B portunun 0'ıncı pinini aynı anda kesme olarak kullanamam.
EXTI0, EXTI1, EXTI2, ........ , EXTI15.  

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

# NVIC (NESTED VECTOR INTERRUPT CONTROL)

* Karmaşık kesme isteklerinin işlemciye sürekli yük getirmemesi için işlemci içerisinde özel bir donanım bloğu oluşturulmuştur. Bu donanıma kesme kontrolörü (interrupt controller) adı verilir.
  
* Kesme kontrolörü haklı bir sebeple gelen kesme isteği neticesinde düzgün işleyen programı askıya alarak kesme fonksiyonu (interrupt function) olarak adlandırılan özel kod parçasını işlemeye başlar.

* Kesme fonksiyonunun işletilmesinin bitiminde program kaldığı yerden çalışmaya devam eder.

* STM32F4 serisi mikroişlemcilerde kesme kontrolörü "Nested Vector Interrupt Controller" olarak isimlendirilir.

* NVIC kontrolör mikroişlemci içerisindeki önemli donanım kesmelerini (DMA istekleri, USART, CAN, I2C, ve Timer gibi donanım kesmeleri) ve ayrıca External Interrupt/Event Controller (EXTI) adı verilen donanım vasıtasıyla portlardan gelen kesmeleri kontrol eder.

 
I2C   IRQ ------------------NVIC------------------PROCESSOR
DMA   IRQ ------------------NVIC------------------PROCESSOR
USART IRQ ------------------NVIC------------------PROCESSOR
CAN   IRQ ------------------NVIC------------------PROCESSOR
TIMER IRQ ------------------NVIC------------------PROCESSOR

Port A ---------EXTI--------NVIC------------------PROCESSOR
Port B ---------EXTI--------NVIC------------------PROCESSOR
...... ---------EXTI--------NVIC------------------PROCESSOR
...... ---------EXTI--------NVIC------------------PROCESSOR
...... ---------EXTI--------NVIC------------------PROCESSOR
Port I ---------EXTI--------NVIC------------------PROCESSOR

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

* SYSCFG_EXTICRx registerleri ile interrupt olarak algılanacak Port-Pin (bit) seçilir.

* Kesmenin yükselen/düşen (rising/falling) kenar veya her ikisinden gelen kesme seçilir. Durumda bir değişiklik olduğunda bunu kesme olarak algılamasını sağlayan EXTI_IMR registerinin biti 1 yapılır.

* EXTI_PR kaydedicisi "1" yapılarak kesme isteğinin oluştuğu bildirilir.

* NVIC kontroller ilgili interrupt rutinini yönlendirir.

* Rutin içerisindeki komutlar işlendikten sonra tekrar ana programa dönülür.

* Event olarak isimlendirilen kaydedici, mikrodenetleyiciyi standby (bekleme) durumundan çıkarak (uyandırma) işlemi de yapabilmektedir. EXTI_EMR kaydedicisi ile bu sağlanmaktadır.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

**Priority :** Öncelik seviyesi. Bu değer ne kadar düşükse öncelik o kadar yüksektir.







































