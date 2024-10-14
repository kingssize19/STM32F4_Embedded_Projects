## Frekans Nedir?
* Bir saniyede oluşan dalga sayısına **frekans** denir.
* Gömülü sistemlerde frekans, genellikle bir mikrodenetleyicinin veya işlemcinin çalıştığı saat hızını (clock speed) ifade eder.
* Bu saat frekansı, işlemcinin bir saniyede kaç işlem gerçekleştirebileceğini belirler ve Hertz (Hz) cinsinden ölçülür.
* Örneğin, bir mikrodenetleyici 16 MHz hızında çalışıyorsa, saniyede 16 milyon işlem gerçekleştirebilir.

-------------------------------------------------------------------------------------------------------------------------------------------------------------

## HSI & HSE
* STM32F446RE Nucleo kartında HSE ve HSI olmak üzere iki farklı ana saat kaynağı bulunur :

**1. HSI (High Speed Internal)**
   * STM32F446RE için dahili bir osilatördür.
   * HSI Frekansı : 16 MHz
   * Bu osilatör harici bir kristale gerek duymadan sistemin çalışabilmesini sağlar. Dahili kristal verimlilik olarak kesin sonuçlar vermeyebilir. Bu yüzden daha çok harici osilatör tercih edilmektedir.

**2. HSE (High Speed External)**
   * Harici bir kristal osilatör yada osilatör modülü olarak kullanılır.
   * HSE Frekansı : 8 MHz (Ancak harici olarak farklı kristalde kullanılabilir. Örneğin, 4 MHz ila 26 MHz arasında olabilir.)
   * HSE, daha hassas ve daha stabil bir saat kaynağı sağlamak amacıyla kullanılır.

**NOT :** Bu iki saat kaynağı, sistemin ana frekansını oluşturan PLL (Phase-Locked Loop) devresine kaynak olarak seçilebilir ve daha yüksek çalışma frekanslarına ölçeklenebilir. STM32F446RE'de sistemin maksimum çalışma frekansı, PLL kullanılarak 180 MHz'e kadar çıkarılabilir.

-------------------------------------------------------------------------------------------------------------------------------------------------------------

## PLL (Phase Locked Loop)

* Clockları daha verimli kullanmak için PLL kullanılır.
* PLL bir clock değildir. HSE değerini kullanarak farklı frekans değerlerine çevirmemizi sağlar.
* 3 parametresi vardır.
    * PLL_M, PLL_N, PLL_P
* **SİSTEM CLOCK (SYSCLK) = ((HSE / PLL_M) * PLL_N) / PLL_P** formülü ile sistem clock değeri hesaplanır.
* STM32F407VG için;
    * HSE = 8 MHz  ---PLL---> 168 MHz (max)
* STM32F446RE Nucleo için;
    * HSE = 8 MHz  ---PLL---> 168 MHz (max)

**Örnek :** PLL_M = 4, PLL_N = 168, PLL_P = 4 olsun. Sistem clock hızı yani SYSCLK nedir?

**Cevap :** SYSCLK = ((8 MHz / 4) * 168) / 4 = 84 MHz'dir.

-------------------------------------------------------------------------------------------------------------------------------------------------------------










