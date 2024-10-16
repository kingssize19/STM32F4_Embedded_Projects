##  Bu kod, bir mikrodenetleyicide sistem saatini yapılandırmak ve sistem çevre birimlerini başlatmak için yazılmıştır. Kodun amacı, mikrodenetleyicinin Saat Kaynağını (Clock Source) değiştirerek sistem saatini HSE (Harici Kristal) üzerinden çalıştırmak, PLL (Phase-Locked Loop) ile çarparak daha yüksek bir frekans elde etmek ve sistem saatini bu yeni frekansta çalışacak şekilde ayarlamaktır.

```c
#include "main.h"

extern uint32_t SystemCoreClock;
uint32_t systemClock;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void RCC_Config(void);


void RCC_Config(void)
{
  RCC->CR &= ~(1 << 0);                // HSI OFF
  RCC->CR |= (1 << 16);               // HSE ON
  while (!(RCC->CR & (1 << 17)));     // wait HSE active
  RCC->CR |= (1 << 19);               // CSS ON

  RCC->PLLCFGR = 0x00000000;          // PLLCFGR Reset and PLL_P = 2
  RCC->PLLCFGR |= (1 << 22);          // PLL Source HSE
  RCC->PLLCFGR |= (4 << 0);           // PLL_M = 4
  RCC->PLLCFGR |= (168 << 6);         // PLL_N = 168

  RCC->CR |= (1 << 24);               //PLL ON
  while (!(RCC->CR) & (1 << 25));     // wait PLL active

  // System clock is PLL
  RCC->CFGR &= ~(1 << 0);
  RCC->CFGR |= (1 << 1);

  while (!(RCC->CFGR & (1 << 1)));  //Select system clock is PLL clock
}

int main (void)
{
  HAL_Init();                          // Tüm çevre birimlerini sıfırlar ve Systick'i başlatır.
  SystemClock_Config();                // Sistem saatini yapılandırır.
  MX_GPIO_Init();                      // GPIO'ları başlatır.
  systemClock = SystemCoreClock;      // Sistem saatini okuyup değişkende tutarız.    (180.000.000 Hz)

  HAL_RCC_DeInit();                  // HSI aktif eder ve PLL'yi kapatır.
  SystemCoreClockUpdate();           // Güncel sistem saatini getirir.
  systemClock = SystemCoreClock;    // Sistemin güncel saatini değişkende saklıyoruz.     (16.000.000 Hz)

  RCC_Config();
  SystemCoreClockUpdate();
  systemClock = SystemCoreClock;    // 168.000.000 Hz

  while (1)
  {

  }

}

```

--------------------------------------------------------------------------------------------------------------------------------------------------------------

## Code Analysis 

**1. extern uint32_t SystemCoreClock;**
* **SystemCoreClock :** Sistemin mevcut saat frekansını saklayan bir değişkendir. Bu değişken, saat konfigürasyonları yapıldıktan sonra güncellenir.

**2. uint32_t systemClock;**
* Bu değişken, SystemCoreClock'un bir kopyasını alarak sistmein saatinin frekansını saklamak için kullanılır.

**3. void RCC_Config(void);**
* Bu fonksiyon, saat kaynağını yapılandırmak için yazılmıştır. RCC (Reset and Clock Control) register'ları kullanarak sistem saati ayarlanır.

--------------------------------------------------------------------------------------------------------------------------------------------------------------

## void RCC_Config(void) Function Analysis

**1. RCC->CR &= (1 << 0);**
* HSI (High-Speed Internal) saat kaynağını kapatır.

**2. RCC->CR |= (1 << 16);**
* HSE (High Speed External) aktif hale getirilir. HSE genellikle bir kristal osilatörle harici olarak sağlanan saat sinyalidir. (8 MHz)

**3. while (!(RCC->CR & (1 << 17)));**
* HSE'nin aktif hale gelmesi beklenir. RCC->CR register'ındaki 17.bit HSE Ready'yi ifade eder. HSE hazır olana kadar döngüde beklenir.

**4. RCC->CR |= (1 << 19);**
* Clock Security System (CSS) etkinleştirilir. Bu sistem, saat sinyalinde bir hata tespit edilirse HSE' yi kapatarak HSI'yi devreye alır.

**5. RCC->PLLCFGR = 0x00000000;**
* PLL yapılandırma register'ı sıfırlanır ve varsayılan değerler atanır. Aynı zamanda ilgili PLLP bitleri 00 yapılarak (yani PLLP değeri 2 olarak ayarlanır) PLLP değeri belirlenmiş olur.

**6. RCC->PLLCFGR |= (1 << 22);**
* PLL'nin kaynak saati HSE olarak ayarlanır.

**7. RCC->PLLCFGR |= (4 << 0);**
* PLL'nin giriş bölme faktörü PLLM 4 olarak ayarlanır.

**8. RCC->PLLCFGR |= (168 << 6);**
* PLL'nin çarpan faktörü PLLN 168 olarak ayarlanır. Bu değer, HSE'den gelen saati 168 katına çıkarır.

**9. RCC->CR |= (1 << 24);**
* PLL aktif hale getirilir.

**10. while (!(RCC->CR & (1 << 25)));**
* PLL'nin aktif hale gelmesi beklenir. RCC->CR register'ındaki 25. bit, PLL Ready sinyalini ifade eder.

**11. Sistem Saati PLL Olarak Seçme:**
* RCC->CFGR &= ~(1 << 0); ve RCC->CFGR |= (1 << 1);
* Bu iki adım, sistem saatini PLL kaynağından çalıştırmak için kullanılır.

**12. while (!(RCC->CFGR & (1 << 1)));**
* Sistem saatinin **PLL Clock** olduğunu doğrulamak için bir bekleme döngüsü oluşturulur.

--------------------------------------------------------------------------------------------------------------------------------------------------------------

## int main(void) Function Analysis

**1. HAL_Init();**
* Bu fonksiyon, HAL (Hardware Abstraction Layer) kütüphanesinin başlatılmasını sağlar. Çevre birimleri sıfırlanır ve Systick Timer'ı başlatır.

**2. SystemClock_Config();**
* Sistem saatini yapılandırmak için bir fonksiyon. Bu fonksiyon sayesinde sistem saat ayarı 180 MHz olarak ayarlanmıştır.

**3. MX_GPIO_Init();**
* Giriş/Çıkış (GPIO) pinlerini başlatmak için kullanılır.

**4. systemClock = SystemCoreClock;**
* Sistemin mevcut saat frekansı SystemCoreClock'tan okunarak systemClock değişkenine atanır. Başlangıçta bu değişken 180 MHz değerini tutar.

**5. HAL_RCC_DeInit();**
* Bu fonksiyon, sistemi varsayılan duruma döndürür ve HSI saatini etkinleştirip PLL'yi devre dışı bırakır.

**6. SystemCoreClockUpdate();**
* Sistem saatinin güncel frekansını SystemCoreClock değişkenine günceller. Şuan HSI saatini kullanarak 16 MHz olarak ayarlanacaktır.

**7. RCC_Config();**
* Bu noktada saat yapılandırması yapılır (HSE üzerinden PLL kullanılarak). **RCC_Config()** fonksiyonu çağrılır ve sistem saati 168 MHz olarak yapılandırılır.

**8. SystemCoreClockUpdate();**
* Bu adımda, yeni yapılandırmaya göre SystemCoreClock güncellenir.

**9. systemClock = SystemCoreClock;**
* Sistem saatinin son hali 168 MHz, systemClock değişkenine atanır.

--------------------------------------------------------------------------------------------------------------------------------------------------------------

## ÖZET
* Kod, başlangıçta sistemi HSI (dahili 16 MHz saat) ile başlatır, ardından HSE (harici kristal) ve PLL'yi kullanarak sistmein saatini 168 MHz'e çıkarır.



