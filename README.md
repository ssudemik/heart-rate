# heart-rate
Arduino kullanımıyla, Esp32 ve Pulse Sensörü ile nabız ölçer 

Bu proje, bir ESP32 mikrodenetleyici kullanarak nabız ölçümü yapmayı amaçlamaktadır. Projede bir nabız sensörü, ESP32 geliştirme kartı ve Blynk platformu kullanılarak gerçek zamanlı olarak nabız verileri toplanıp izlenmiştir.
PROJEDE KULLANILAN ARAÇALAR
•	ESP32 Geliştirme Kartı 
•	Pulse Sensor (Nabız Sensörü) 
•	Breadboard
•	Jumper Kabloları
•	USB Kablosu
•	Blynk Uygulaması
        
![image](https://github.com/ssudemik/heart-rate/assets/77544171/b23e8583-e746-49d3-a52d-31f695386808)
![image](https://github.com/ssudemik/heart-rate/assets/77544171/d4567db6-4692-4f52-b60a-e9db5c179bed)

PROJENİN AMACI
Projenin temel amacı, bireylerin kalp atış hızını (nabız) ölçen ve bu veriyi gerçek zamanlı olarak izlemelerini sağlayan bir sistem geliştirmektir. Bu sistem, taşınabilir ve düşük maliyetli bir sağlık izleme cihazı olarak kullanılabilir. Özellikle, kalp sağlığına dikkat eden bireyler ve sporcular için pratik bir çözüm sunmayı hedeflemektedir. IoT (Nesnelerin İnterneti) teknolojilerini kullanarak geleneksel nabız ölçer cihazlarının ötesine geçmektir. Bunu sağlamak için, nabız verilerinin Wi-Fi aracılığıyla Blynk platformuna gönderilmesi ve kullanıcıların bu verileri akıllı telefonları üzerinden kolayca izleyebilmesi sağlanmıştır. Böylece, kullanıcılar herhangi bir yerde ve zamanda nabız verilerine erişebilir ve geçmiş verilerini analiz edebilir.
ESP32 Mikrodenetleyicisinin Kullanımı: ESP32'nin Wi-Fi özelliklerini kullanarak verilerin kablosuz olarak iletilmesi ve işlenmesi.
Nabız Sensörü ile Veri Toplama: Nabız sensöründen alınan verilerin doğru bir şekilde okunması ve işlenmesi.
Gerçek Zamanlı İzleme: Blynk platformu üzerinden nabız verilerinin gerçek zamanlı olarak izlenmesi.
Veri Görselleştirme: Nabız verilerinin grafiksel olarak görselleştirilmesi ve geçmiş verilere dayalı analiz yapılabilmesi.
Projede kullanılan bağlantılar şu şekildedir:
•	Pulse Sensor sinyal kablosu (sarı) -> ESP32'nin GPIO 32 pinine
•	Pulse Sensor güç kablosu (kırmızı) -> ESP32'nin 5V pinine
•	Pulse Sensor toprak kablosu (siyah) -> ESP32'nin GND pinine

PROJENİN ARDUINO KODU
// Blynk tanımlamaları
#define BLYNK_TEMPLATE_ID "TMPL6Xen9kX1Q" // Blynk uygulaması için şablon ID'si
#define BLYNK_TEMPLATE_NAME "Nabız Ölçer" // Blynk uygulaması için şablon adı
#define BLYNK_AUTH_TOKEN "lqKNYdzgBSmMfcU1N9Jz0no0p5s1kWx_" // Blynk uygulaması için kimlik doğrulama token'ı

// Gerekli kütüphanelerin dahil edilmesi
#include <WiFi.h> // WiFi bağlantısı için kütüphane
#include <WiFiClient.h> // WiFi istemci kütüphanesi
#include <Wire.h> // I2C iletişimi için kütüphane
#include <BlynkSimpleEsp32.h> // Blynk kütüphanesi
#include <PulseSensorPlayground.h> // Pulse sensör kütüphanesi

// Blynk ayarları
char auth[] = BLYNK_AUTH_TOKEN; // Blynk uygulamasından aldığınız Auth Token
char ssid[] = "Redmi Note 8 Pro"; // WiFi SSID
char password[] = "1234.1234"; // WiFi Parolası

#define REPORTING_PERIOD_MS 510 // Raporlama periyodu (milisaniye cinsinden)

int BPM; // Nabız değerini tutacak değişken
const int PulseWire = 32; // Pulse sensörünü bağladığımız Analog pin
int Threshold = 510; // Eşik değeri, belirli bir nabız algılama eşiği

PulseSensorPlayground pulseSensor; // Pulse sensörü için nesne oluşturma

void setup() {
  Serial.begin(115200); // Seri iletişimi başlat
  pulseSensor.analogInput(PulseWire); // Pulse sensörünün bağlı olduğu pini ayarla
  pulseSensor.setThreshold(Threshold); // Eşik değerini ayarla
  
  // Pulse sensörünü başlat
  pulseSensor.begin();

  // WiFi bağlantısını başlat
  WiFi.begin(ssid, password);
  
  // WiFi bağlanma işlemi tamamlanana kadar bekle
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WiFi bağlantısı kuruluyor...");
  }
  
  Serial.println("WiFi bağlandı"); // WiFi bağlantısı başarılı mesajı
  Serial.print("IP Adresi: ");
  Serial.println(WiFi.localIP()); // Cihazın IP adresini yazdır

  // Blynk ile bağlantı kur
  Blynk.begin(auth, ssid, password);

  Serial.println("Blynk ile bağlantı kuruldu"); // Blynk bağlantısı başarılı mesajı
}

void loop() {
  Blynk.run(); // Blynk işlevlerini çalıştır
  
  // Pulse sensöründen veri al
  if (pulseSensor.sawStartOfBeat()) {
    BPM = pulseSensor.getBeatsPerMinute(); // Nabız değerini al
  }

  // Belirli bir periyotta bilgileri gönder
  static uint32_t tsLastReport = 0;
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Nabız: ");
    Serial.println(BPM); // Nabız değerini seri monitöre yazdır
    Blynk.virtualWrite(V0, BPM); // Blynk uygulamasına nabız verisini gönder
    tsLastReport = millis(); // Son raporlama zamanını güncelle
  }
  
  delay(2000); // Döngü 2 saniye bekleyecek
}

KODUN ÇALIŞMA PRENSiBİ
•	LYNK_TEMPLATE_ID, BLYNK_TEMPLATE_NAME, BLYNK_AUTH_TOKEN: Blynk platformu ile bağlantı kurmak için gerekli olan kimlik bilgileri ve yetkilendirme tokeni.
•	Kütüphaneler: WiFi, WiFiClient, Wire, Blynk ve PulseSensorPlayground kütüphaneleri projede kullanılacak işlevler için dahil edilir.
•	Blynk Ayarları: auth, ssid ve password değişkenleri Blynk ve Wi-Fi bağlantı bilgilerini içerir.
•	Diğer Değişkenler: REPORTING_PERIOD_MS, BPM, PulseWire, Threshold ve pulseSensor gibi değişkenler tanımlanır.
•	Seri İletişim: 115200 baud hızıyla seri iletişim başlatılır.
•	Pulse Sensörü Ayarları: Sensörün bağlı olduğu pin ve eşik değeri ayarlanır, ardından sensör başlatılır.
•	WiFi Bağlantısı: ESP32, belirlenen SSID ve parola ile WiFi ağına bağlanır. Bağlantı sağlandığında IP adresi yazdırılır.
•	Blynk Bağlantısı: Blynk ile bağlantı kurulur ve bağlantı durumu yazdırılır.
•	Blynk İşlemleri: Blynk.run() ile Blynk fonksiyonları çalıştırılır.
•	Nabız Sensörü Verileri: Sensör, her kalp atışının başlangıcını algılar ve kalp atış hızını (BPM) hesaplar.
•	Seri İletişim: Sensörden alınan ham veri seri port üzerinden yazdırılır.
•	Blynk'e Veri Gönderme: Her REPORTING_PERIOD_MS süresi sonunda, hesaplanan BPM değeri Blynk platformuna gönderilir ve seri port üzerinden yazdırılır.
•	Döngü Gecikmesi: delay(20) ile döngüde kısa bir bekleme süresi eklenir, bu da CPU'nun aşırı yüklenmesini önler.

SONUÇLAR
 ![image](https://github.com/ssudemik/heart-rate/assets/77544171/982ba4be-df21-4c75-850b-589b08b0f9e0)
 ![image](https://github.com/ssudemik/heart-rate/assets/77544171/6f567a75-c87f-403b-a787-f83fd1717f88)
 ![image](https://github.com/ssudemik/heart-rate/assets/77544171/d223b730-ea7c-4039-b838-cac4b4bf2c5e)
![image](https://github.com/ssudemik/heart-rate/assets/77544171/d516db5e-8ee7-4d88-bcc3-970dc65b3f46)
![image](https://github.com/ssudemik/heart-rate/assets/77544171/a5f9625e-1539-4cbb-b390-5209b8b85eec)


PROJENİN GELİŞTİRİLMESİ
Ek Sensörler: Sadece nabız değil, aynı zamanda oksijen doygunluğu, kan basıncı ve vücut sıcaklığı gibi diğer sağlık parametrelerini ölçen sensörler eklenebilir.
Veri Füzyonu: Farklı sensörlerden gelen verilerin birleştirilerek daha kapsamlı sağlık analizleri yapılabilir.
Özel Mobil Uygulama: Blynk yerine, iOS ve Android için özel bir mobil uygulama geliştirilerek kullanıcı deneyimi iyileştirilebilir.
Gerçek Zamanlı Bildirimler: Anormal bir nabız veya sağlık durumu tespit edildiğinde kullanıcının telefonuna anlık bildirimler gönderilebilir.
Veri Analizi: Toplanan veriler üzerinde daha detaylı analizler yapılarak trendler ve anormallikler tespit edilebilir.
Makine Öğrenimi: Kullanıcıların geçmiş verilerine dayanarak kişiselleştirilmiş sağlık tahminleri ve önerilerde bulunmak için makine öğrenimi algoritmaları uygulanabilir.
Bulut Depolama: Verilerin bulutta saklanarak uzun vadeli sağlık takibi yapılabilir.
Uzaktan Erişim: Kullanıcılar verilerine herhangi bir yerden erişebilir ve sağlık profesyonelleri ile paylaşabilir.
Gelişmiş Grafikler: Daha iyi görselleştirmeler ve grafikler ile kullanıcıların sağlık verilerini anlamaları kolaylaştırılabilir.
Kişiselleştirme: Kullanıcı arayüzü kişiselleştirilebilir ve kullanıcının tercihlerine göre özelleştirilebilir.
Giyilebilir Teknoloji: Projeyi bir giyilebilir cihaz (örneğin, bileklik veya saat) haline getirerek kullanıcıların sürekli olarak nabızlarını izlemeleri sağlanabilir.
Kablosuz Şarj: Cihazın daha kullanıcı dostu olması için kablosuz şarj özellikleri eklenebilir.

