#include <Arduino.h>
#define BUTTON_PIN 14 // D5
#define BUZZER_PIN 5 // D1
#define LED_PIN 4 // D2

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
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
    buzz(15);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
}