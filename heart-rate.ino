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

