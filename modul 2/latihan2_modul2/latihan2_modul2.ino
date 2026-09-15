#include <DHT.h>

#define BUTTON_PIN D2
#define LED_PIN D1
#define DHT_PIN D7
#define LDR_PIN A0

#define DHTTYPE DHT22
DHT dht(DHT_PIN, DHTTYPE);

bool lampState = false;

int buttonState;
int lastButtonState = HIGH;

unsigned long lastSensorRead = 0;
const unsigned long sensorInterval = 2000;

void setup() {

  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  dht.begin();

  Serial.println();
  Serial.println("========================================");
  Serial.println("        SMART HOUSE NODEMCU");
  Serial.println("========================================");
  Serial.println("Sistem siap...");
  Serial.println();
}

void loop() {

  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW && lastButtonState == HIGH) {

    lampState = !lampState;

    if (lampState) {
      Serial.println();
      Serial.println(">>> PUSH BUTTON DITEKAN");
      Serial.println(">>> LAMPU : ON");
    }
    else {
      Serial.println();
      Serial.println(">>> PUSH BUTTON DITEKAN");
      Serial.println(">>> LAMPU : OFF");
    }

    delay(50);
  }

  lastButtonState = buttonState;

  if (lampState) {
    digitalWrite(LED_PIN, HIGH);
  }
  else {
    digitalWrite(LED_PIN, LOW);
  }

  if (millis() - lastSensorRead >= sensorInterval) {

    lastSensorRead = millis();

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    int ldrValue = analogRead(LDR_PIN);

    Serial.println();
    Serial.println("------------ KONDISI RUMAH ------------");

    Serial.print("Lampu      : ");

    if (lampState) {
      Serial.println("MENYALA");
    }
    else {
      Serial.println("MATI");
    }

    if (isnan(temperature)) {
      Serial.println("Suhu       : Gagal membaca DHT22");
    }
    else {
      Serial.print("Suhu       : ");
      Serial.print(temperature);
      Serial.println(" C");
    }

    if (isnan(humidity)) {
      Serial.println("Kelembapan : Gagal membaca DHT22");
    }
    else {
      Serial.print("Kelembapan : ");
      Serial.print(humidity);
      Serial.println(" %");
    }

    Serial.print("LDR        : ");
    Serial.println(ldrValue);

    Serial.print("Cahaya     : ");

    if (ldrValue < 400) {
      Serial.println("GELAP");
    }
    else if (ldrValue < 700) {
      Serial.println("REDUP");
    }
    else {
      Serial.println("TERANG");
    }

    if (!isnan(temperature)) {

      if (temperature >= 30) {
        Serial.println("Peringatan : Suhu rumah panas!");
      }
      else {
        Serial.println("Kondisi    : Suhu normal");
      }
    }

    Serial.println("----------------------------------------");
  }
}