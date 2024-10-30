## 1. SystemClock_Config Fonksiyonu

Bu fonksiyon sistem saatini HSE (Harici Kristal Osilatör) ve PLL (Phase Locked Loop) kullanarak 180 MHz'e ayarlıyor.

```c
// HSI'yi kapatıyoruz
RCC->CR &= ~(1 << 0);  // HSI OFF
```
* **RCC->CR:** Saat kontrol register’ıdır.
* (1 << 0): HSI (Harici Saat Kaynağı, 16 MHz dahili osilatör) kapatılır.

```c
// HSE'yi aktif ediyoruz
RCC->CR |= (1 << 16);  // HSE ON
while (!(RCC->CR & (1 << 17)));  // HSE aktif olana kadar bekle
```
* HSE (High-Speed External): Harici kristal (8 MHz) açılır. HSE etkin hale gelene kadar beklenir.

```c
// FLASH latency ayarları (5 wait states)
FLASH->ACR |= FLASH_ACR_LATENCY_5WS;
```
* **FLASH->ACR:** Flash gecikme ayarıdır. 180 MHz gibi yüksek hızlarda bellek erişim süresini artırmak için "5 wait states" (bekleme durumu) eklenir.

```c
// PLL ayarları (HSE kaynağına göre)
RCC->PLLCFGR = 0;  // PLLCFGR'yi sıfırlama
```
* **RCC->PLLCFGR:** PLL ayar register'ıdır ve sıfırlanır.

```c
RCC->PLLCFGR |= (1 << 22);  // PLL Kaynağı HSE
RCC->PLLCFGR |= (4 << 0);   // PLL_M = 4 (HSE'yi 2 MHz'e böl)
RCC->PLLCFGR |= (180 << 6); // PLL_N = 180 (2 MHz * 180 = 360 MHz)
RCC->PLLCFGR |= (0 << 16);  // PLL_P = 2 (360 MHz / 2 = 180 MHz)
RCC->PLLCFGR |= (7 << 24);  // PLL_Q = 7 (USB için)
```
* PLL_M, PLL_N, PLL_P ve PLL_Q: PLL'nin çıkış frekansını ayarlayan çarpan ve bölücü parametrelerdir. Bu ayarlara göre, 8 MHz HSE frekansı önce 2 MHz’e düşürülüyor, sonra 180 ile çarpılarak 360 MHz elde ediliyor, son olarak 2’ye bölünüp 180 MHz çıkış frekansı üretiliyor. USB için PLL_Q değeri 7 olarak ayarlanır.

```c
// PLL'yi aktif ediyoruz
RCC->CR |= (1 << 24);  // PLL ON
while (!(RCC->CR & (1 << 25)));  // PLL aktif olana kadar bekle
```
* **PLL Aktivasyonu:** PLL etkinleştirilir ve aktif hale gelene kadar beklenir.

```c
// Sistem saat kaynağını PLL olarak seç
RCC->CFGR &= ~(3 << 0);  // Saat kaynağını temizle
RCC->CFGR |= (2 << 0);   // Sistem saat kaynağı olarak PLL'yi seç
```
* **RCC->CFGR:** Saat konfigürasyon register’ı. Sistem saati olarak PLL seçiliyor.

```c
// Saat kaynağının PLL olduğunu kontrol et
while ((RCC->CFGR & (3 << 2)) != (2 << 2));  // PLL'nin sistem saati olduğunu doğrula
```
* PLL'nin sistem saati olarak seçilip seçilmediği doğrulanır.

--------------------------------------------------------------------------------------------------------------------------------------------

## 2. GPIO_Config Fonksiyonu
GPIO pinlerinin LED'ler ve LDR bağlantısı için ayarlanması.

```c
RCC->AHB1ENR |= (1 << 0);  // GPIOA clock bus active
```
* **RCC->AHB1ENR:** GPIOA için saat kaynağını etkinleştirir.

**LED1 Konfigürasyonu (PA5)**
```c
GPIOA->MODER |= (1 << 10);  // MODE OUT
GPIOA->MODER &= ~(1 << 11);
GPIOA->OTYPER &= ~(1 << 5); // PUSH PULL
GPIOA->PUPDR &= ~(1 << 10); // NO PULL
GPIOA->OSPEEDR |= (1 << 10); // MEDIUM SPEED
```
* **MODER:** PA5 pinini çıkış modu olarak ayarlar.
* **OTYPER:** Çıkış tipi push-pull olarak belirlenir.
* **PUPDR:** Çekme direnci yok (No Pull).
* **OSPEEDR:** Hız seviyesi orta hızda ayarlanır.

**LED2 Konfigürasyonu (PA7)**
Benzer işlemler PA7 pini için yapılır. Bu pin de çıkış ve push-pull olarak ayarlanır.

**LDR Konfigürasyonu (PA0)**
```c
GPIOA->MODER |= (1 << 0); // ANALOG MODE
GPIOA->OTYPER &= ~(1 << 0); // PUSH PULL
GPIOA->PUPDR &= ~(1 << 0); // PULL DOWN
GPIOA->PUPDR |= (1 << 1);
```
* **MODER:** PA0 pinini analog moda alır.
* **PUPDR:** Pull-down direnci eklenir.

--------------------------------------------------------------------------------------------------------------------------------------------

## 3. ADC_Config Fonksiyonu
ADC1 konfigürasyonunun yapıldığı fonksiyon.
```c
RCC->APB2ENR |= (1 << 8);  // APB2 enable
```
* **APB2ENR:** ADC1 biriminin saat kaynağı etkinleştirilir.

```c
// Resolution 8 bit
ADC1->CR1 &= ~(1 << 24);
ADC1->CR1 |= (1 << 25);
```
* **ADC1->CR1:** Çözünürlüğü 8 bit olarak ayarlar.

```c
ADC1->CR2 |= (1 << 0); // ADC1 ON
```
* ADC1->CR2: ADC'yi etkinleştirir.

```c
ADC1->SMPR2 |= 0x00000003; // SAMPLE TIME
```
* **SMPR2:** Örnekleme süresi ayarları. Bu süre, ADC'nin örnekleme hızını belirler.

```c
// ADCPRE PRESCALER CLOCK DIV 8
ADC->CCR |= (1 << 16);
ADC->CCR |= (1 << 17);
```
* **ADC->CCR:** ADC saat frekansını 8'e bölerek ADC’ye uygulanan frekansı azaltır.

--------------------------------------------------------------------------------------------------------------------------------------------

## 4. Read_ADC Fonksiyonu 

ADC'den veri okumak için kullanılan fonksiyon.
```c
ADC1->CR2 |= (1 << 30); // START SOFTWARE
```
* Yazılım aracılığıyla ADC dönüşümünü başlatır.

```c
while (!(ADC1->SR & (1 << 1)));  // wait EOC
```
* **ADC1->SR:** EOC (End of Conversion) bayrağı bitene kadar bekler.

```c
value = ADC1->DR;
```
* **ADC1->DR:** ADC dönüşüm sonucunu okur.

--------------------------------------------------------------------------------------------------------------------------------------------

## 5. main Fonksiyonu
Sistem saatini yapılandırır, GPIO ve ADC'yi konfigüre eder ve sonsuz döngüde LDR değerine göre LED'leri kontrol eder.

**LED Kontrolü**
```c
if (ldr_value > 150)
{
    GPIOA->ODR |= (1 << 5);   // PA5 LED ON
    GPIOA->ODR &= ~(1 << 7);  // PA7 LED OFF
}
else
{
    GPIOA->ODR |= (1 << 7);   // PA7 LED ON
    GPIOA->ODR &= ~(1 << 5);  // PA5 LED OFF
}
```
* **GPIOA->ODR:** LED'lerin LDR değerine göre yanmasını veya sönmesini sağlar.

