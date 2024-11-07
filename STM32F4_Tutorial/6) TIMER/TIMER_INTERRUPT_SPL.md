Bu kod, STM32F4 serisi mikrodenetleyici kullanarak basit bir GPIO ve Timer konfigürasypnu gerçekleştiren bir örnek uygulamadır. Kodun amacı, bir timer kullanarak belirli bir zaman aralığında GPIO pininin durumunu değiştirmektir.

## 1. GPIO Konfigürasyonu 

GPIO, mikrodenetleyicinin dijital pinlerinin giriş veya çıkış olarak kullanılmasını sağlar. Bu kodda, bir GPIO pinini çıkış olarak yapılandırıyoruz.

```c
void GPIO_Config()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  // GPIOA'nın saatini etkinleştir

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;              // Pin çıkış olarak ayarlanır
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;             // Push-pull çıkışı seçilir
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;                  // Pin 5 seçilir (STM32F4 Discovery kartında genellikle LED1'e bağlıdır)
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;           // Pin için pull-up/pull-down direnç kullanılmaz
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;          // Pin hızı 50 MHz olarak ayarlanır

    GPIO_Init(GPIOA, &GPIO_InitStruct);                     // GPIOA pin 5 yapılandırılır
}
```

* **RCC_AHB1PeriphClockCmd:** GPIOA'nın saat kaynağını etkinleştirir. Bu fonksiyon, GPIOA portunun kullanılması için gerekli olan saat sinyalini aktifleştirir.
* **GPIO_InitStruct.GPIO_Mode:** GPIO pininin çalışma modunu belirtir. GPIO_Mode_OUT olarak ayarlandığında pin çıkış olarak çalışacaktır.
* **GPIO_InitStruct.GPIO_OType:** Çıkış tipini seçer. GPIO_OType_PP (Push-Pull) kullanıldığında, pin yüksek veya düşük seviyeye çekilebilir.
* **GPIO_InitStruct.GPIO_Pin:** Hangi pinin kullanılacağını belirtir. Burada GPIO_Pin_5 seçilmiştir, yani PA5 pini (Discovery kartında genellikle bir LED'e bağlıdır).
* **GPIO_InitStruct.GPIO_PuPd:** Pull-up veya pull-down dirençlerini ayarlar. GPIO_PuPd_NOPULL ile direnç kullanılmaz.
* **GPIO_InitStruct.GPIO_Speed:** Pin hızını belirler. Bu parametre, pinin ne kadar hızlı şekilde yüksek/ düşük olabileceğini ayarlar. GPIO_Speed_50MHz genellikle yeterli hızdır.

--------------------------------------------------------------------------------------------------------------------------------------------------------

## 2. Timer Konfigürasyonu

Timer, zamanlayıcı işlevi görerek belirli aralıklarla kesmeler oluşturabilir. Bu kodda, bir zamanlayıcı (Timer 2) belirli bir süre sonra kesme oluşturacak şekilde yapıladırılır.

```c
void Timer_Config()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // Timer 2'nin saatini etkinleştir

    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        // Saat bölme seçeneği
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;     // Sayıcı sayma yönü (yukarıya doğru)
    TIM_InitStruct.TIM_Period = 1999;                        // Timer süresi (1999, 2000'den bir önceki değeri belirtir)
    TIM_InitStruct.TIM_Prescaler = 41999;                    // Prescaler (önbölge) değeri
    TIM_InitStruct.TIM_RepetitionCounter = 0;                // Tekrarlama sayacı sıfırdır

    TIM_TimeBaseInit(TIM2, &TIM_InitStruct);                 // Timer 2'yi yapılandır
    TIM_Cmd(TIM2, ENABLE);                                   // Timer 2'yi başlat
}
```

* **RCC_APB1PeriphClockCmd:** Timer 2'nin saat kaynağını etkinleştirir.
* **TIM_InitStruct.TIM_ClockDivision:** Saat bölme oranını belirler. Burada TIM_CKD_DIV1 ile hiçbir bölme yapılmaz.
* **TIM_InitStruct.TIM_CounterMode:** Sayıcının yönünü ayarlar. TIM_CounterMode_Up ile sayıcı yukarıya doğru çalışır (0'dan belirtilen periyoda kadar sayar).
* **TIM_InitStruct.TIM_Period:** Timer'ın bitiş değeri. Bu değere ulaşıldığında timer sıfırlanır ve kesme (interrupt) oluşturulur.
* **TIM_InitStruct.TIM_Prescaler:** Timer'ın saat frekansını bölme faktörüdür. Bu, timer'ın daha düşük hızda çalışmasını sağlar.
* **TIM_TimeBaseInit:** Timer'ı temel yapılandırma ile başlatır.
* **TIM_Cmd:** Timer'ı etkinleştirir.

--------------------------------------------------------------------------------------------------------------------------------------------------------

## 3. NVIC Konfigürasyonu (Kesme Yönetimi)

NVIC(Nested Vectored Interrupt Controller), mikrodenetleyicinin kesme yöntemini sağlar. Bu fonksiyon, Timer 2'nin kesme isteklerini etkinleştirir.

```c
void NVIC_Config()
{
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;                   // Kesme kanalını belirtir (Timer 2)
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;                     // Kanalı etkinleştirir
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;       // Öncelik seviyesi
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;              // Alt öncelik seviyesi

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);                       // Timer 2 için kesme isteği etkinleştirilir
    NVIC_Init(&NVIC_InitStruct);                                     // NVIC yapılandırması başlatılır
}
```
* **NVIC_InitStruct.NVIC_IRQChannel:** Hangi kesme kanalının kullanılacağını belirtir (burada TIM2_IRQn ile Timer 2'nin kesmesi kullanılır).
* **NVIC_InitStruct.NVIC_IRQChannelCmd:** Kesme kanalının etkinleştirilip etkinleştirilmeyeceğini belirtir.
* **NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority:** Kesme öncelik seviyesidir. 0x00 ile en yüksek öncelik verilmiştir.
* **NVIC_InitStruct.NVIC_IRQChannelSubPriority:** Kesme alt önceliğidir.
* **TIM_ITConfig:** Timer için kesme etkinleştirilir. TIM_IT_Update ile periyodik kesme etkinleştirilmiştir.

--------------------------------------------------------------------------------------------------------------------------------------------------------

## 4. Timer 2 Kesme Handler

```c
void TIM2_IRQHandler()
{
    GPIO_ToggleBits(GPIOA, GPIO_Pin_5);  // PA5 pinini tersten değiştir (LED'in yanıp sönmesini sağlar)
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  // Kesme bayrağını temizler
}
```

* **GPIO_ToggleBits:** GPIO pininin değerini tersine çevirir. Eğer pin yüksekse, düşük yapılır ve tam tersi.
* **TIM_ClearITPendingBit:** Kesme bayrağını temizler, böylece bir sonraki kesme için hazır hale gelir.

--------------------------------------------------------------------------------------------------------------------------------------------------------

## main fonksiyonu

```c
int main(void)
{
    GPIO_Config();  // GPIO yapılandırmasını yap
    Timer_Config(); // Timer yapılandırmasını yap
    NVIC_Config();  // NVIC yapılandırmasını yap

    while (1)
    {
        // Ana döngüde sürekli olarak işlem yapılmaz, kesme tabanlı bir yapı vardır.
    }
}
```

--------------------------------------------------------------------------------------------------------------------------------------------------------

## ÖZET

* **GPIO_Config:** GPIO pinini çıkış olarak yapılandırır (PA5, genellikle bir LED pinidir).
* **Timer_Config:** Timer'ı yapılandırarak, belirli bir süre sonra kesme oluşturmasını sağlar.
* **NVIC_Config:** Kesme yönetimini yapılandırarak, Timer 2'nin kesmelerini etkinleştirir.
* **TIM2_IRQHandler:** Kesme geldiğinde çalışacak fonksiyon, PA5 pinini değiştirir.












