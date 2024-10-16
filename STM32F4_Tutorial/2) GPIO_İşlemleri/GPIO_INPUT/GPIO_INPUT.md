**Butonlar ve anahtarlar mikrodenetleyiciye giriş pini üzerinden lojik 1 veya lojik 0 olarak bilgi girişini sağlayan mekanik elemanlardır.**

## Buton Arkı (DEBUENCE) Oluşumu

* Buton ve anahtarda konum değiştiğinde arktan dolayı mikrodenetleyici girişinde çok sayıda istenmeyen lojik değer oluşur.

## Pull-up / Pull-down Direnç

* **Pull-up** bağlantıda GPIO girişi direnç üzerinden + beslemeye (VCC/VDD) bağlanır.
* Butona basılmadığı durumda GPIO girişinde lojik 1 vardır. Butona basıldığı durumda girişe 0V (lojik 0) uygulanmış olur.
* **Pull-down** bağlantıda, GPIO girişi direnç üzerinden GND ye bağlanır.
* Butona basılmadığı durumda girişte lojik 0 bulunur. Butona basıldığı durumda buton üzerinden lojik 1 uygulanmış olur.

------------------------------------------------------------------------------------------------------------------------------------------------------------

**STM32F4 Mikrodenetleyicilerinde yazılımla port girişleri pull-up veya pull-down olarak seçilebilir. Bunun için GPIOx_PUPDR kaydedicisine ilgili değerleri yazmak yeterlidir.**

**STM32F4 DISCOVERY kiti üzerindeki kullanıcı butonu pull-down bağlanmıştır. Butona basılmadığı durumda girişte 0, basıldığında 1 uygulanır.**

**Atollic'de GPIO portlarından veri okumak için kullanılan bazı fonksiyonlar vardır.**
```c
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);
```

**Biz bu butonun durumunu okuyacağız, okuyacağımız değerinde 0 yada 1 olduğunu biliyoruz. Yani bit seviyesinde bir okuma gerçekleştireceğiz.**
```c
GPIO_ReadInputDataBit(GPIOx, GPIO_Pin);
```
* Birinci parametre Port bilgisi ikinci parametre pin bilgisini içerir.














