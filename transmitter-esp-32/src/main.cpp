#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

#define LORA_SS   5
#define LORA_RST  14
#define LORA_DIO0 26

#define LED_PIN     2
#define BUZZER_PIN  4
#define BUTTON_PIN  27

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

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

  Serial.println("System Ready ✅");
}

void loop() {

  if (digitalRead(BUTTON_PIN) == LOW) {

    Serial.println("Button Pressed");

    
    for (int i = 0; i < 10; i++) {
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);
      LoRa.beginPacket();
      LoRa.print("ALERT");
      LoRa.endPacket();
      Serial.println("ALERT sent");
      delay(300);
      
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      delay(300);
    }

    while (digitalRead(BUTTON_PIN) == LOW);
    delay(200);
  }
}
