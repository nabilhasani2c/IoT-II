
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// ==========================================
// 1. KONFIGURASI WI-FI
// ==========================================
const char* ssid = "oiiu";
const char* password = "22222229";

// ==========================================
// 2. KONFIGURASI MQTT BROKER
// ==========================================
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

// Gunakan Client ID yang unik
const char* clientId = "ESP8266_LabIoT_NIM77";

// Topik telemetri sesuai NIM
const char* topicPubTelemetry =
  "kampus/labiot/25782077/telemetri";

// ==========================================
// 3. KONFIGURASI SENSOR DHT
// ==========================================
const byte dhtPin = 2;  // D4 (GPIO 2)
#define DHTTYPE DHT22   // Ganti menjadi DHT22 jika perlu

DHT dht(dhtPin, DHTTYPE);

// ==========================================
// 4. OBJEK WI-FI DAN MQTT
// ==========================================
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long previousPublishTime = 0;
const unsigned long publishInterval = 5000;

// ==========================================
// 5. KONEKSI WI-FI
// ==========================================
void connectToWiFi() {
  Serial.print("Menghubungkan ke Wi-Fi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi Terkoneksi!");

  Serial.print("Alamat IP Lokal: ");
  Serial.println(WiFi.localIP());
}

// ==========================================
// 6. KONEKSI MQTT
// ==========================================
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT Broker...");

    if (client.connect(clientId)) {
      Serial.println("Sukses Terhubung!");
    } else {
      Serial.print("Gagal, kode status rc=");
      Serial.print(client.state());
      Serial.println(". Mencoba kembali dalam 5 detik...");
      delay(5000);
    }
  }
}

// ==========================================
// 7. SETUP
// ==========================================
void setup() {
  Serial.begin(115200);
  dht.begin();

  connectToWiFi();
  client.setServer(mqttServer, mqttPort);

  Serial.println("Sistem MQTT Publisher Siap.");
}

// ==========================================
// 8. LOOP UTAMA
// ==========================================
void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop();

  unsigned long currentMillis = millis();

  if (currentMillis - previousPublishTime >= publishInterval) {
    previousPublishTime = currentMillis;

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (isnan(t) || isnan(h)) {
      Serial.println("Peringatan: Gagal membaca data DHT!");
      return;
    }

    // ========================================
    // 9. PEMBENTUKAN JSON
    // ========================================
    StaticJsonDocument<256> doc;

    doc["node_id"] = clientId;
    doc["suhu"] = serialized(String(t, 2));
    doc["kelembapan"] = serialized(String(h, 2));

    // Tambahan Latihan 3
    doc["uptime_detik"] = millis() / 1000;
    doc["ip_lokal"] = WiFi.localIP().toString();

    char jsonBuffer[256];
    serializeJson(doc, jsonBuffer);

    // ========================================
    // 10. PUBLIKASI DATA MQTT
    // ========================================
    bool isSuccess =
      client.publish(topicPubTelemetry, jsonBuffer);

    if (isSuccess) {
      Serial.print("Telemetri Terkirim [");
      Serial.println(topicPubTelemetry);
      Serial.print("]: ");
      Serial.println(jsonBuffer);
    } else {
      Serial.println("Peringatan: Gagal memublikasikan paket!");
    }
  }
}
