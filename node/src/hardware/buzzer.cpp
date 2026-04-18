#include <Arduino.h>
#include "buzzer.h"
#include "../configs.h"

void init_buzzer() {
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
}

void buzzer_on() { digitalWrite(BUZZER_PIN, HIGH); }
void buzzer_off() { digitalWrite(BUZZER_PIN, LOW); }