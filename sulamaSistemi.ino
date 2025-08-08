#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char* ssid = "SELÇUK";
const char* password = "matrix12";

// Röle pini
const int relayPin = D1;
// Buton pini
const int buttonPin = D2;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 10800, 60000);  // Türkiye için GMT+3 = 3*3600 = 10800

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Röle devre dışı (LOW tetikli olabilir)

pinMode(buttonPin, INPUT_PULLUP);


  // Wi-Fi'ye bağlan
  WiFi.begin(ssid, password);
  Serial.print("Wi-Fi bağlanıyor");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi bağlandı!");

  // Zaman başlat
  timeClient.begin();
}

void loop() {
  timeClient.update();

  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();

  Serial.printf("Saat: %02d:%02d\n", currentHour, currentMinute);

  // Saat 09:00 veya 18:00'de sulama yap
  if ((currentHour == 9) && currentMinute == 10) {
    sulamaYap();
    delay(60000); // Aynı dakikada tekrar çalışmasın
  }

  // Buton kontrolü (LOW ise basılmıştır)
  if (digitalRead(buttonPin) == HIGH) {
    Serial.println("Butona basıldı, röle 1 saniye çalışacak.");
    digitalWrite(relayPin, LOW); // Röle aktif
    delay(3000);                 // 3 saniye bekle
    digitalWrite(relayPin, HIGH);// Röle pasif
    // Butonun tekrar okunması için ufak bekleme (debounce)
    delay(100);
  }

  delay(5000); // Her 10 saniyede bir kontrol et
}

void sulamaYap() {
  Serial.println("Sulama başladı...");
  digitalWrite(relayPin, LOW); // Röle aktif (LOW tetiklemeli)
  delay(4000); // 4 saniye sulama 
  digitalWrite(relayPin, HIGH); // Röle pasif
  Serial.println("Sulama bitti.");
}
