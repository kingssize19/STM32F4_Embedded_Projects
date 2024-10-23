Bu uygulama, STM32F446RE Nucleo kartı kullanarak potansiyometre gibi bir analog sensörden okunan değeri ADC (Analog to Digital Converter) ile dijital bir sinyale çeviriyor. Daha sonra bu sinyale bağlı olarak GPIO çıkışı kontrol ediliyor.

## 1. Başlık Dosyaları ve Yapılandırma
```c
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
```
* **stm32f4xx.h :** Bu başlık dosyası, STM32F4 serisi mikrodenetleyicilerin tüm donanım yapılandırmalarını içerir. Bu dosya, mikrodenetleyici ailesine ait register tanımları ve sistem clock yapılandırmaları gibi temel fonksiyonları sağlar.
* **stm32f4_discovery.h :** Discovery kartları için özel yapılandırmaları içerir. Nucleo kartı ile de kullanılabilir çünkü Nucleo ve Discovery kartları STM32F4 serisi işlemcileri kullanır ve ortak donanım yapılandırmalarına sahiptir. Nucleo kartlarının işlemci yapıları, Discovery kartlarıyla uyumludur. Ancak, Nucleo kartında farklı periferal bağlantıları olabileceği için, bazı özel fonksiyonlar (örneğin LED, butonlar gibi) farklılık gösterebilir.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## 2. Değişkenler ve Yapılandırma Yapıları
```c
GPIO_InitTypeDef	   		GPIO_InitStruct;
ADC_InitTypeDef		    	ADC_InitStruct;
ADC_CommonInitTypeDef		ADC_CommonInitStruct;

uint16_t adc_value = 0;
float adc_voltage = 0;
```
* **GPIO_InitTypeDef :** GPIO pinlerinin yapılandırılması için kullanılır.
* **ADC_InitTypeDef :** ADC yapılandırmasını tutar.
* **ADC_CommonInitTypeDef :** ADC'nin ortak yapılandırmaları için kullanılır (örneğin, ADC modları ve prescaler ayarları).
* **adc_value :** ADC'den okunan dijital değeri tutar.
* **adc_voltage :** Bu değer, ADC’den okunan değerin voltaja dönüştürülmüş halini saklar.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## 3. GPIO Yapılandırması
```c
void GPIO_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
```
* **RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE) :** GPIOA portu için AHB1 saat sinyali etkinleştiriliyor. GPIO kullanmadan önce saat sinyalini açmak gerekir.

**Pin PA0 (ADC Kanalı)**
```c
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
```
* **GPIO_Mode_AN :** Pin, analog giriş olarak ayarlanıyor (ADC kullanımı için gerekli).
* **GPIO_Pin_0 :** PA0 pini seçiliyor.
* **GPIO_PuPd_NOPULL :** Giriş pini için pull-up/pull-down direnci uygulanmıyor.
* **GPIO_Speed_50MHz :** GPIO hızını belirler (bu hız, pinin çevresel birimlere veri iletme hızıdır; analog modda pek önemli değildir).

**Pin PA5 (LED veya Dijital Çıkış)**
```c
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
```
* **GPIO_Mode_OUT :** PA5 pini çıkış pini olarak ayarlanıyor.
* **GPIO_Pin_5 :** Bu pin genellikle kart üzerindeki dahili LED ile ilişkilidir (Nucleo kartlarında da bu yapılandırma mevcuttur).

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## 4. ADC Yapılandırması
```c
void ADC_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
```
* **RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE) :** ADC1 çevresel birimi için saat sinyali etkinleştiriliyor.

**ADC Ortak Yapılandırması**
```c
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div8;
	ADC_CommonInit(&ADC_CommonInitStruct);
```
* **ADC_Mode_Independent :** ADC modunu bağımsız olarak ayarlar (tek başına çalışacak).
* **ADC_Prescaler_Div8 :** ADC’nin saat sinyali 8'e bölünüyor, bu da ADC hızını kontrol etmek için kullanılır. Hız çok yüksek olduğunda ADC performansı etkilenebilir.

**ADC1 Yapılandırması**
* **ADC_Resolution_12b :** ADC'nin çözünürlüğü 12 bit olarak ayarlanıyor. 12 bit çözünürlük, 0 ile 4095 arasında bir dijital değer döndürür.
* **ADC_Cmd(ADC1, ENABLE) :** ADC1 modülü etkinleştiriliyor.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## 5. Read_ADC Fonksiyonu
```c
uint16_t Read_ADC()
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_56Cycles);
	ADC_SoftwareStartConv(ADC1);
	while (!(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)));
	return ADC_GetConversionValue(ADC1);
}
```
* **ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_56Cycles) :** ADC1'in PA0 (ADC_Channel_0) kanalından 56 döngüde bir örnek alınacak şekilde yapılandırıyor.
* **ADC_SoftwareStartConv(ADC1) :** Yazılımsal olarak ADC dönüşümünü başlatır.
* **while (!(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))) :** Dönüşüm tamamlanana kadar bekler (ADC_FLAG_EOC = End of Conversion).
* **ADC_GetConversionValue(ADC1) :** Dönüşüm tamamlandıktan sonra ADC'den dijital değeri okur ve geri döner.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## 6. main Fonksiyonu
```c
int main(void)
{
  GPIO_Config();
  ADC_Config();
  while (1)
  {
	  adc_value = Read_ADC();
	  adc_voltage = (0.000805) * adc_value;
	  if (adc_value > 1500)
	  {
		  GPIO_SetBits(GPIOA, GPIO_Pin_5);
	  }
	  else if (adc_value <= 1500)
	  {
		  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	  }
  }
}
```
* **adc_value = Read_ADC() :** ADC'den dijital değer okunur.
* **adc_voltage = (0.000805) * adc_value; :** Dijital değer voltaja çevrilir. 0.000805 faktörü, 12 bit çözünürlük ve 3.3V referans gerilimine dayalıdır (3.3V / 4096 ≈ 0.000805 V).
* **GPIO_SetBits(GPIOA, GPIO_Pin_5) :** Eğer ADC değeri 1500'den büyükse, PA5 pini HIGH yapılır (LED yanar).
* **GPIO_ResetBits(GPIOA, GPIO_Pin_5) :** Eğer ADC değeri 1500'den küçük veya eşitse, PA5 pini LOW yapılır (LED söner).

---------------------------------------------------------------------------------------------------------------------------------------------------------------

## SONUÇ

* Bu kod, ADC kullanarak potansiyometreden bir değer okur ve bu değere göre bir LED'in yanmasını veya sönmesini sağlar. Potansiyometreye uygulanan gerilim, ADC tarafından dijital değere çevrilir ve bu değer belirli bir eşik değerinin üzerindeyse LED yanar.























