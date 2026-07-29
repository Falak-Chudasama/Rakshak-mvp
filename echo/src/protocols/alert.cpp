#include <Arduino.h>
#include "alert.h"
#include "../configs.h"
#include "../hardware/led.h"
#include "../hardware/buzzer.h"
#include "../hardware/button.h"

bool echo_alert_pattern(int id) {
    auto pulse_phase = [](int ms) -> bool {
        unsigned long start = millis();
        while (millis() - start < (unsigned long)ms) {
            if (button_pressed()) {
                buzzer_off();
                led_off();
                return true;
            }
            delay(10);
        }
        buzzer_off();
        led_off();
        return false;
    };

    for (int i = 0; i < id; i++) {
        buzzer_on();
        led_on();
        if (pulse_phase(200)) return true;

        buzzer_off();
        led_off();
        if (pulse_phase(200)) return true;
    }

    if (pulse_phase(1000)) return true;

    buzzer_off();
    led_off();
    return false;
}