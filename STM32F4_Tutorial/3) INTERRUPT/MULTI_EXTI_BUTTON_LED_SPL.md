Bu STM32F4 mikrokontrolcüsü için yazılmış bir C programı, GPIO pinlerini ve harici kesme (EXTI) yapılandırmasını kullanarak iki farklı butonla LED'lerin kontrolünü sağlıyor.

## 1. Sistem Kütüphaneleri
```c
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
```
* **stm32f4xx.h :** STM32F4 serisi mikrodenetleyiciler için temel yapılandırma ve donanım başlık dosyalarını içerir.
* **stm32f4_discovery.h :** STM32F4 Discovery kartları için spesifik yapılandırmaları ve bazı ek işlevleri içerir.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## 2. Yapılandırma Tip Tanımları
```c
GPIO_InitTypeDef GPIO_InitStruct;
EXTI_InitTypeDef EXTI_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;
```
Bu yapılar GPIO, EXTI ve NVIC (kesme denetleyicisi) yapılandırmaları için kullanılır. Her biri ilgili donanım birimini yapılandırmak için gerekli parametreleri içerir.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## 3. GPIO Yapılandırma Fonksiyonu (GPIO_Config)
```c
void GPIO_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//led
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	//button
```
* Burada, GPIOA ve GPIOB portlarının saatleri etkinleştirilir. STM32’de, bir portu kullanabilmek için o portun saatini açmak gereklidir.

## 3.1 LED Konfigürasyonu :
```c
	//led config
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
```
* GPIO_Pin_5 ve GPIO_Pin_7 pinleri LED olarak ayarlanmıştır.
* Bu pinler çıkış modunda (GPIO_Mode_OUT), push-pull tipi (GPIO_OType_PP) olarak ayarlanmış, herhangi bir pull-up ya da pull-down direnci bağlanmamış (GPIO_PuPd_NOPULL) ve hızları 50 MHz (GPIO_Speed_50MHz) olarak belirlenmiştir.

## 3.2 Buton Konfigürasyonu
```c
	//button config
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStruct);
```
* GPIO_Pin_4 ve GPIO_Pin_3 pinleri buton olarak giriş modunda (GPIO_Mode_IN) yapılandırılmış.
* Bu pinlere pull-down direnci bağlanmış (GPIO_PuPd_DOWN) ve hızları yine 50 MHz olarak ayarlanmış. pull-down ile buton basılı olmadığında pinin LOW (0) seviyesinde olması sağlanır.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## 4. EXTI Yapılandırma Fonksiyonu (EXTI_Config)
```c
void EXTI_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);	//exti
```
* EXTI kesme hattı için gerekli olan SYSCFG biriminin saat sinyali etkinleştirilir.

## 4.1 EXTI3 Konfigürasyonu
```c
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource3);

	EXTI_InitStruct.EXTI_Line = EXTI_Line3;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;

	EXTI_Init(&EXTI_InitStruct);
```
* GPIOB'deki 3 numaralı pin (PB3), EXTI kesmesi ile ilişkilendirilir. Butona basıldığında bu kesme tetiklenir.
* EXTI3 kesme hattı (EXTI_Line3), Rising Edge (düşükten yükseğe geçiş) tetikleyici ile etkinleştirilir.

## 4.2 EXTI4 Konfigürasyonu
```c
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);

	EXTI_InitStruct.EXTI_Line = EXTI_Line4;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;

	EXTI_Init(&EXTI_InitStruct);
```
* Benzer şekilde, GPIOB'deki 4 numaralı pin (PB4) de EXTI kesmesi ile ilişkilendirilir.

## 4.3 NVIC Yapılandırması
```c
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;

	NVIC_Init(&NVIC_InitStruct);
```
* NVIC_PriorityGroupConfig ile kesmelerin öncelik grubu ayarlanır.
* EXTI3 için öncelik ayarlanır ve kesme etkinleştirilir. Öncelik PreemptionPriority = 0 ve SubPriority = 0 olarak ayarlanmıştır.

```c
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;

	NVIC_Init(&NVIC_InitStruct);
```
* EXTI4 için de benzer şekilde yapılandırma yapılır, ancak bu kesme PreemptionPriority = 1 ile daha düşük bir önceliğe sahiptir.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## 5. Kesme Rutini (IRQ Handlers)

**EXTI3_IRQHandler() fonksiyonu :**
```c
void EXTI3_IRQHandler()
{
	if (EXTI_GetITStatus(EXTI_Line3) != RESET) {
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		for (int i = 0; i < 3; ++i){
			GPIO_SetBits(GPIOA, GPIO_Pin_7);
			delay(5000000);
			GPIO_ResetBits(GPIOA, GPIO_Pin_7);
			delay(5000000);
		}
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}
```
* PB3 butonuna basıldığında kesme tetiklenir.
* PA5 pini kapatılır (GPIO_ResetBits), ardından PA7 pini 3 kez yakılıp söndürülür. Bu işlem bir gecikme (delay) ile gerçekleştirilir.

**EXTI4_IRQHandler() fonksiyonu :**
```c
void EXTI4_IRQHandler()
{
	if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		for (int i = 0; i < 3; ++i){
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
			delay(16800000);
			GPIO_ResetBits(GPIOA, GPIO_Pin_5);
			delay(16800000);
		}
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}
```
* PB4 butonuna basıldığında benzer bir işlem yapılır, fakat PA5 pini bu kez 3 kez yakıp söndürülür. delay süreleri farklıdır.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## 6. main FOnksiyonu
```c
int main(void)
{
	GPIO_Config();
	EXTI_Config();
	while (1)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	}
}
```
* Ana döngüde sürekli olarak PA5 pini kapalı tutulur.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## 7. Delay Fonksiyonu
```c
void delay(uint32_t time)
{
	while (time--);
}
```
* Gecikme oluşturmak için basit bir döngü kullanılır.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## 8. SONUÇ

Bu programda iki buton kullanılarak farklı LED'ler kontrol edilmektedir. Butonlara basıldığında ilgili kesmeler tetiklenir ve LED'ler yanıp söner. Kodun geri kalanı basit konfigürasyon ve kesme yapıları ile ilgilidir.































