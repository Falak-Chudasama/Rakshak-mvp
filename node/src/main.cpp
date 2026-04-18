#include <Arduino.h>
#include "configs.h"
#include "hardware/led.h"
#include "hardware/buzzer.h"
#include "hardware/button.h"
#include "hardware/lora_node.h"

unsigned long lastSendTime = 0;

void setup() {
    Serial.begin(BAUD_RATE);
    while (!Serial); // Wait for serial monitor to connect

    Serial.printf("\n--- Rakshak Node Started | ID: %d ---\n", DEVICE_ID);

    // Initialize every single component
    init_led();
    init_buzzer();
    init_button();
    init_lora_node();
}

void loop() {
    // Dead simple: Emit LoRa signal every 1 second
    if (millis() - lastSendTime >= 1000) {
        lastSendTime = millis();
        
        led_on();             // Visual indicator of sending
        lora_send_alert();    // Transmit
        led_off();            // Turn off visual indicator
    }
}