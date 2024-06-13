#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Definisikan DHT11 sensor
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// SSID dan Password WiFi
const char* ssid = "<your-ssid>";
const char* password = "<your-password>";

// URL endpoint
const char* serverName = "https://abbf-202-125-95-131.ngrok-free.app/api/sensor_data";

// Interval pengiriman data (ms)
const long interval = 2000;
unsigned long previousMillis = 0;

// NTP Client untuk mendapatkan waktu
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7*3600, 60000); // NTP server, offset time (in seconds), update interval (in ms)

void setup() {
  Serial.begin(9600);
  delay(1000); // Tambahkan sedikit delay untuk memastikan koneksi serial siap
  dht.begin();
  
  // Koneksi ke WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi");

  // Inisialisasi NTP Client
  timeClient.begin();
  timeClient.update();
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Membaca data dari sensor DHT11
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    // Mengecek apakah data valid
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    
    // Menampilkan data ke Serial Monitor
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C");

    // Update waktu dari NTP server
    timeClient.update();
    String timestamp = timeClient.getFormattedTime();

    // Membuat payload JSON dengan format yang diinginkan
    String payload = "temperature=" + String(t) + "&humidity=" + String(h) + "&timestamp=" + timestamp;
    Serial.print("JSON Payload: ");
    Serial.println(payload);
    
    // Mengirim data ke server
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      // Mengirim request POST
      int httpResponseCode = http.POST(payload);
      
      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
      } else {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
      }

      // Mengakhiri HTTP request
      http.end();
    } else {
      Serial.println("Error in WiFi connection");
    }
  }
}
