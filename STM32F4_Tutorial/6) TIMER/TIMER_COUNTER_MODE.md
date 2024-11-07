Bu kodda, STM32F4 Discovery kartı üzerinde Timer 2 kullanılarak GPIO pinlerinden birini belirli aralıklarla açıp kapatma işlemi yapılmıştır.
Kod, temel olarak bir timer sayacı kullanarak belirli bir süre dolduğunda LED yakma veya söndürme gibi işlemler yapmaya yönelik bir yapılandırmaya sahiptir.

--------------------------------------------------------------------------------------------------------------------------------------------------------------

## 1. GLOBAL DEĞİŞKEN TANIMLAMALARI

```c
uint32_t count;
TIM_TimeBaseInitTypeDef TIM_InitStruct;
GPIO_InitTypeDef GPIO_InitStruct;
```
* **count :** Timer'ın sayaç değerini saklamak için kullanılan bir değişkendir.
* **TIM_InitStruct :** Timer yapılandırması için kullanılan bir yapı. Bu yapı ile timer'ın çalışma özellikleri ayarlanır.
* **GPIO_InitStruct :** GPIO yapılandırması için kullanılan bir yapı. Bu yapı ile GPIO pinlerinin çıkış veya giriş modları gibi özellikleri ayarlanır.

--------------------------------------------------------------------------------------------------------------------------------------------------------------

## 2. GPIO_Config() Fonksiyonu 

```c
void GPIO_Config()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStruct);
}
```

Bu fonksiyon, GPIO yapılandırmasını yaparak pin modlarını ve özelliklerini belirler. 

* **RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE):** GPIOA portu için saat sinyali aktif edilir. STM32'de çevresel birimlerin çalışabilmesi için saat sinyalinin aktif edilmesi gereklidir.
* **GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT:** GPIOA üzerindeki pin 5'in çıkış modu olarak ayarlanmasını sağlar.
* **GPIO_InitStruct.GPIO_OType = GPIO_OType_PP:** Pin çıkış tipi "Push-Pull" olarak ayarlanır. Bu sayede pin, hem yüksek hem düşük sinyal çıkışı verebilir.
* **GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5:** GPIOA portunda 5 numaralı pini seçer. STM32F446re nucleo kartında GPIOA pin 5, LED bağlı pinlerden biridir.
* **GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL:** Pin için pull-up veya pull-down direnci kullanılmaz.
* **GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz:** Pin hızını 50 MHz olarak ayarlar. Bu, pinin sinyal frekansı için önemli bir ayardır.
* **GPIO_Init(GPIOA, &GPIO_InitStruct):** Bu yapılandırmaların GPIOA portuna uygulanmasını sağlar.

--------------------------------------------------------------------------------------------------------------------------------------------------------------

## 3. Timer_Config() Fonksiyonu

```c
void Timer_Config()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStruct.TIM_Period = 7999;
    TIM_InitStruct.TIM_Prescaler = 44999;
    TIM_InitStruct.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
    TIM_Cmd(TIM2, ENABLE);
}
```

Bu fonksiyon, Timer 2'yi yapılandırır.

* **RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE):** Timer 2 için saat sinyali aktif edilir.
* **TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1:** Saat bölme (Clock Division) değeri 1 olarak ayarlanır. Bu, timer'ın giriş saat sinyalini bölmeden kullanacağı anlamına gelir.
* **TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up:** Timer’ın yukarı doğru sayma modunda çalışmasını sağlar. Yani, sayaç değeri sıfırdan başlayarak TIM_Period değeri olan 7999’a kadar sayar.
* **TIM_InitStruct.TIM_Period = 7999:** Timer’ın period değeri 7999 olarak ayarlanır. Bu değer sayaçın ulaştığında sıfırlanacağı değeri belirler.
* **TIM_InitStruct.TIM_Prescaler = 44999:** Timer’ın prescaler değeri olarak 44999 seçilir. Bu değer, timer'ın çalışma frekansını yavaşlatmak için kullanılır. STM32F4’te sistem saat frekansı 84 MHz’dir, bu frekans prescaler ile bölünür.
* **TIM_InitStruct.TIM_RepetitionCounter = 0:** Bu parametre, yalnızca gelişmiş zamanlayıcılar için kullanılır, burada etkisi yoktur.
* **TIM_TimeBaseInit(TIM2, &TIM_InitStruct):** Timer yapılandırmalarının Timer 2’ye uygulanmasını sağlar.
* **TIM_Cmd(TIM2, ENABLE):** Timer 2’yi başlatır.

--------------------------------------------------------------------------------------------------------------------------------------------------------------

## main() Fonksiyonu

```c
int main(void)
{
    Timer_Config();  // Timer 2 yapılandırmasını başlatır
    GPIO_Config();   // GPIOA yapılandırmasını başlatır
    while (1)
    {
        count = TIM_GetCounter(TIM2);  // Timer 2’nin sayaç değerini alır

        if (count == 3999)
        {
            GPIO_SetBits(GPIOA, GPIO_Pin_5);  // GPIOA pin 5’i (LED) yüksek yap
        }
        else if (count == 7999)
        {
            GPIO_ResetBits(GPIOA, GPIO_Pin_5);  // GPIOA pin 5’i düşük yap
        }
    }
}
```
Bu fonksiyon, ana program döngüsünü içerir.

* Timer_Config() ve GPIO_Config() fonksiyonları çağrılarak Timer ve GPIO yapılandırmaları yapılır.
* while (1) döngüsünde sürekli olarak count değişkeni TIM_GetCounter(TIM2) ile güncellenir. Bu fonksiyon, Timer 2’nin CNT register’ındaki (Counter register) mevcut sayaç değerini döndürür.
* if (count == 3999): Sayaç değeri 3999 olduğunda, GPIO_SetBits(GPIOA, GPIO_Pin_5) fonksiyonu ile GPIOA pin 5 (LED) yüksek yapılır.
* else if (count == 7999): Sayaç değeri 7999 olduğunda, GPIO_ResetBits(GPIOA, GPIO_Pin_5) ile pin düşük yapılır.

Bu sayede LED, Timer 2’nin sayacına bağlı olarak belirli aralıklarla yanıp sönmektedir


![Uploading image.png…]()































