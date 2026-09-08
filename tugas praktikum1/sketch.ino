const int ledPin = 5;       // LED terhubung ke GPIO 5
const int buttonPin = 4;    // Push button terhubung ke GPIO 4

// Menyimpan kondisi LED
// false = LED OFF
// true  = LED ON
bool ledState = false;

// Menyimpan kondisi tombol sebelumnya
int lastButtonState = LOW;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  // Kondisi awal LED mati
  digitalWrite(ledPin, LOW);

  Serial.println("=== SISTEM SAKELAR TOGGLE ESP32 ===");
  Serial.println("LED awal: OFF");
}

void loop() {

  // Membaca kondisi tombol pada GPIO 4
  int buttonState = digitalRead(buttonPin);

  // Mendeteksi tombol baru saja ditekan
  // dari LOW menjadi HIGH
  if (buttonState == HIGH && lastButtonState == LOW) {

    // Membalik status LED
    // OFF -> ON
    // ON  -> OFF
    ledState = !ledState;

    // Mengatur LED sesuai status
    digitalWrite(ledPin, ledState);

    if (ledState == true) {
      Serial.println("Tombol ditekan -> LED ON");
    } else {
      Serial.println("Tombol ditekan -> LED OFF");
    }

    // Debounce selama 200 ms
    // agar satu tekanan tidak terbaca berkali-kali
    delay(200);
  }

  // Menyimpan kondisi tombol saat ini
  // untuk pengecekan pada loop berikutnya
  lastButtonState = buttonState;
}