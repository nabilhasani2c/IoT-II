const byte ldrPin = A0;

void setup() {  
  Serial.begin(115200);  
  Serial.println("Sistem Pembacaan LDR Dalam Persentase Siap!");
}

void loop() { 
  int ldrValue = analogRead(ldrPin);   

  float ldrPercent = (ldrValue / 1023.0) * 100.0;

  Serial.print("Intensitas Cahaya (ADC): ");  
  Serial.print(ldrValue);  
  Serial.print(" | Persentase: ");
  Serial.print(ldrPercent);
  Serial.println("%");

  delay(1000);   
}