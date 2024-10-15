## SPL ile Led Yakma Aşamamaları (STM32F407VG DISC1) (D portuna bağlı olan ledleri yakalım.)

1) D portu AHB1 clock hattına bağlıdır. Bu sebeple önce AHB1 clock hattını D portu için aktif etmeliyiz.
* AHB1 hattını aktif etmek için;
  * void RCC_AHB1PeriphClockCmd(uint32_t **RCC_AHB1Periph**, FunctionalState **NewState**); fonksiyonu kullanılır. Bu fonksiyon iki ayrıparametre alır.
  * **1.parametre :** AHB1'e bağlı birim.
  * **2.parametre ise durum. (ENABLE/DISABLE)**

* AHB1 hattını aktif etmek için aşağıdaki gibi yazmamız gerekir.
```c
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
```
Bu ifade ile birlikte D portuna clock sinyali iletmiş oluruz. Ve artık D portunu kullanabiliriz.


