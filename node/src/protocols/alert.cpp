#include <Arduino.h>
#include "alert.h"
#include "../configs.h"
#include "../hardware/led.h"
#include "../hardware/buzzer.h"

void alert_protocol() {
    for (int i = 0; i < ALERT_DURATION; i++) {
        for (int j = 0; j < DEVICE_ID; j++) {
            buzzer_on();
            led_on();
            delay(150);

            buzzer_off();
            led_off();
            delay(150);
        }
        delay(1000);
    }
}