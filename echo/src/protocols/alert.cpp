#include <Arduino.h>
#include "alert.h"
#include "../hardware/led.h"

void trigger_echo_alert(int source_id, const char* location) {
    Serial.printf("\n[!!!] ALERT RECEIVED [!!!]\n");
    Serial.printf("Source Node ID: %d\n", source_id);
    Serial.printf("Location: %s\n", location);
    Serial.println("--------------------------");

    // Violently blink the LED 5 times to indicate an alert
    for (int i = 0; i < 5; i++) {
        led_on();
        delay(100);
        led_off();
        delay(100);
    }
}