## SPL ile Led Yakma Aşamamaları (STM32F407VG DISC1) (D portuna bağlı olan ledleri yakalım.)

## 1) Clock Hattını Aktif Edelim

* D portu AHB1 clock hattına bağlıdır. Bu sebeple önce AHB1 clock hattını D portu için aktif etmeliyiz.
* AHB1 hattını aktif etmek için;
  * void RCC_AHB1PeriphClockCmd(uint32_t **RCC_AHB1Periph**, FunctionalState **NewState**); fonksiyonu kullanılır. Bu fonksiyon iki ayrı parametre alır.
  * **1.parametre :** AHB1'e bağlı birim.
  * **2.parametre ise durum. (ENABLE/DISABLE)**

* AHB1 hattını aktif etmek için aşağıdaki gibi yazmamız gerekir.
```c
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
```
Bu ifade ile birlikte D portuna clock sinyali iletmiş oluruz. Ve artık D portunu kullanabiliriz.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## 2) GPIO Structure'larını Kullanalım 

* GPIO'ların structure'ları GPIO_InitTypeDef altında tanımlanmıştır. Bu structure'ları kullanabilmek için GPIO_InitTypeDef türünden bir nesne oluşturmamız gerekir.
```c
GPIO_InitTypeDef     GPIO_InitStruct;
```
* GPIO'nun konfigürasyonları artık GPIO_InitStruct nesnesi üzerinden yapılabilir.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## 3) Konfigürasyon Ayarlarını Yapalım

Konfigürasyonlarda 5 farklı ifade mevcut.
* **GPIO_Pin :** Kullanacağımız pinleri seçeriz.
* **GPIO_Mode :** Kullanacağımız pinlerin hangi modda kullanılacağını seçeriz.
* **GPIO_OType :** Kullanacağımız pinlerin Push-pull mu yoksa Open Drain olarak mı çalışacağını seçeriz.
* **GPIO_PuPd :** Pull up / Pull down ya da no pull olarak seçim yapmamızı sağlar.
* **GPIO_Speed :** Kullanacağımız pinlerin hızlarını ve enerji tüketimlerini belirleriz.

**Bu tanımlama işlemlerinden sonra konfigürasyonlarımızı D portuna yüklememiz gereklidir. Yükleme işlemi aşağıdaki gibi yapılır.**
```c
GPIO_Init(GPIOD, &GPIO_InitStruct);
```
* Bu aşamadan sonra konfigürasyon ayarlarımız biter artık ledlerimizi kontrol etmeye başlayabiliriz.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## 4) Ledleri Yakalım/Söndürelim

* Bir pini lojik 1 yapmak istiyorsak GPIO_SetBits(), bir pini lojik 0 yapmak istiyorsak GPIO_ResetBits() fonksiyonlarını kullanmalıyız.

* Örneğin D portunun 12. pinindeki ledi yakmak istiyorsak;
```c
GPIO_SetBits(GPIOD, GPIO_Pin_12);
```
yazmamız yeterlidir.

* Aynı şekilde lojik 1 durumundaki D portunun 12. pinini lojik 0'a çekmek istiyorsak yani ledi söndürmek istiyorsak;
```c
GPIO_ResetBits(GPIOD, GPIO_Pin_12);
```
yazmamız yeterlidir.






















