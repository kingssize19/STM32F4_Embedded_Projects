## Kod Analizi
Kod STM32 mikrodenetleyicisi için yazılmış ve temel olarak DAC modülünü kullanarak bir sayısal değeri analog çıkışa dönüştürmeyi amaçlıyor. Örnek olarak 12 bit çözünürlükte bir değer üretip, DAC kanalına gönderiyor. 

--------------------------------------------------------------------------------------------------------------------------------------------------------

## Kullanılan DAC Fonksiyonları 

**1. HAL_DAC_Init(&hdac) :**
* Bu fonksiyon, hdac DAC yapılandırma değişkenini kullanarak DAC periferini başlatır.
* Eğer işlem başarısız olursa, Error_Handler() çağrılır.
* Başlatılan bu DAC, daha sonra analog çıkış elde etmek için kullanılabilir.

**2. HAL_DAC_Start(&hdac, DAC_CHANNEL_1) :**
* Bu komut, belirtilen DAC_CHANNEL_1 kanalı üzerinden DAC'i aktif hale getirir.
* hdac yapısı ve kanal numarası fonksiyona parametre olarak iletilir. Bu şekilde DAC çıkışında belirlenen kanal üzerinden veri üretilecektir.
* Bu örnekte DAC_CHANNEL_1 kanalını etkinleştirir ve sürekli bir sinyal üretmek için hazır hale getirir.

**3. HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i) :**
* Bu fonksiyon DAC'in çıkış voltajını ayarlamak için kullanılır.
* hadc DAC yapısı, DAC_CHANNEL_1 kanalı ve i değişkenindeki 0 ile 4095 arası sayısal değeri kullanır.
* DAC_ALIGN_12B_R, 12 bit sağa dayalı olarak veri gönderileceğini belirtir. 12 bit çözünürlükte değerler 0-4095 arasında olmalıdır, bu da DAC çıkışının 0V ile referans voltajı (örneğin 3.3V) arasında değişmesini sağlar.

**4. HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) :**
* Bu fonksiyon, DAC kanalının konfigürasyonunu yapılandırmak için kullanılır.
* sConfig yapısı, DAC ayarlarını tanımlar : DAC_TRIGGER_NONE tetikleme olmaksızın sürekli çalışma modunda DAC'i ayarlar ve DAC_OUTPUTBUFFER_ENABLE ile çıkış tamponunu etkinleştirir, bu da kararlı bir çıkış sağlar.
* Bu konfigürasyonla, kanal sürekli bir sinyal üretmeye hazır hale getirilir ve bu sinyal main() fonksiyonundaki döngüde HAL_DAC_SetValue() fonksiyonuyla güncellenir.




















