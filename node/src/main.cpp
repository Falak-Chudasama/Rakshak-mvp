#include <Arduino.h>
#include "configs.h"
#include "hardware/led.h"
#include "hardware/buzzer.h"
#include "hardware/button.h"
#include "hardware/lora_node.h"

void setup() {
    Serial.begin(BAUD_RATE);
    while (!Serial);

    Serial.printf("\n--- Rakshak Node Started | ID: %d ---\n", DEVICE_ID);

    init_led();
    init_buzzer();
    init_button();
    init_lora_node();
}

void loop() {
    if (button_pressed()) 
    {
        led_on();
        // buzzer_on();
        lora_send_alert();
        led_off();
        buzzer_off();
        delay(500);
    }
}