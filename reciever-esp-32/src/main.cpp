#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

#define LORA_SS   5
#define LORA_RST  14
#define LORA_DIO0 26

#define LED_PIN     2
#define BUZZER_PIN  4

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  SPI.begin(18, 19, 23, LORA_SS);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  pinMode(LORA_RST, OUTPUT);
  digitalWrite(LORA_RST, LOW);
  delay(100);
  digitalWrite(LORA_RST, HIGH);
  delay(100);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init FAILED ❌");
    while (true);
  }

  Serial.println("LoRa Receiver Ready ✅");
}

void loop() {

  int packetSize = LoRa.parsePacket();

  if (packetSize) {

    Serial.println("LoRa Packet Detected!");

    Serial.print("Received: ");
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    Serial.print(" | RSSI: ");
    Serial.println(LoRa.packetRssi());

    // Activate alert for 10 minutes
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);

    Serial.println("Alert Active for 10 minutes");

    delay(300);

    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

    Serial.println("Alert Ended");
  }
}
