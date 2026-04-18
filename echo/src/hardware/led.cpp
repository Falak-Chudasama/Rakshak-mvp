#include <Arduino.h>
#include "led.h"
#include "../configs.h"

void init_led() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW); // Start with LED off
}

void led_on() { digitalWrite(LED_PIN, HIGH); }
void led_off() { digitalWrite(LED_PIN, LOW); }