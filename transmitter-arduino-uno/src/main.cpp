#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

#define BUTTON_PIN 5
#define BUZZER_PIN 6
#define LED_PIN 7

// LoRa Pins for UNO
#define LORA_SS 10
#define LORA_RST 9
#define LORA_DIO0 2

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed");
    while (1);
  }

  Serial.println("LoRa Transmitter Ready");
}

void buzz(int howManyTimes) {
  for (int i = 0; i < howManyTimes; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

void loop() {

  if (digitalRead(BUTTON_PIN) == LOW) {

    // Send LoRa packet
    LoRa.beginPacket();
    LoRa.print("ALERT_UNO_BUTTON");
    LoRa.endPacket();

    Serial.println("Alert Sent");

    buzz(3);
    delay(1000);  // basic debounce + spam protection
  }
}
