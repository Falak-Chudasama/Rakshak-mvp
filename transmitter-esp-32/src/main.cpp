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

  Serial.println("\n--- LoRa TRANSMITTER ---");

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

  // LOCK PARAMETERS
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  LoRa.setSyncWord(0xA5);
  LoRa.enableCrc();
  LoRa.setTxPower(17);  // Max safe for RA-02

  Serial.println("LoRa initialized ✅");
}

void loop() {

  if (digitalRead(BUTTON_PIN) == LOW) {

    Serial.println("Button pressed. Sending ALERT...");

    for (int i = 0; i < 10; i++) {
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);
    
      LoRa.beginPacket();
      LoRa.print("ALERT");
      int result = LoRa.endPacket();
    
      if (result == 1) {
        Serial.println("ALERT sent successfully ✅");
      } else {
        Serial.println("ALERT send failed ❌");
      }
  
      delay(200);
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      delay(200);
    }

    while (digitalRead(BUTTON_PIN) == LOW);
    delay(200);
  }
}