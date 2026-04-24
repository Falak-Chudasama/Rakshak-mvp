#include <Arduino.h>
#include "alert.h"
#include "../configs.h"
#include "../hardware/led.h"
#include "../hardware/buzzer.h"

void alert_protocol() {
    unsigned long start = millis();
    while (millis() - start < ALERT_DURATION) {
        buzzer_on();
        led_on();
        delay(150);
        buzzer_off();
        led_off();
        delay(150);
    }
}