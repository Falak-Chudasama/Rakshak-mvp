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

  Serial.println("\n--- LoRa RECEIVER ---");

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

  // LOCK PARAMETERS (MUST MATCH TRANSMITTER)
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  LoRa.setSyncWord(0xA5);
  LoRa.enableCrc();

  Serial.println("LoRa Receiver Ready ✅");
}

void loop() {

  int packetSize = LoRa.parsePacket();

  if (packetSize) {

    String incoming = "";

    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }

    Serial.print("Received: ");
    Serial.println(incoming);

    if (incoming == "ALERT") {

      Serial.println("Valid ALERT received ✅");

      digitalWrite(LED_PIN, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);

      delay(200);

      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}