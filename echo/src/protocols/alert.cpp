#include <Arduino.h>
#include "alert.h"
#include "../configs.h"
#include "../hardware/led.h"
#include "../hardware/buzzer.h"
#include "../hardware/button.h"

void echo_alert_pattern(int id) {
    for (int i = 0; i < id; i++) {
        buzzer_on();
        led_on();
        delay(200);
        buzzer_off();
        led_off();
        delay(200);
    }
    
    unsigned long pause_start = millis();
    while (millis() - pause_start < 1000) {
        if (button_pressed()) return;
        delay(10);
    }
}