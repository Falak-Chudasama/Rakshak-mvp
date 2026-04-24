#include <Arduino.h>
#include "configs.h"
#include "hardware/led.h"
#include "hardware/buzzer.h"
#include "hardware/button.h"
#include "hardware/lora_node.h"
#include "protocols/alert.h"

unsigned long lastSendTime = 0;

void setup() {
    Serial.begin(BAUD_RATE);
    while (!Serial);

    Serial.printf("\n--- Rakshak Node Started | ID: %d ---\n", DEVICE_ID);

    init_led();
    init_buzzer();
    init_button();
    init_lora_node();
    alert_protocol();
}

void loop() {
    lora_receive_alert();
}