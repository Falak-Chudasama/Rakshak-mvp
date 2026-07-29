#include <Arduino.h>
#include <LoRa.h>
#include "configs.h"
#include "hardware/led.h"
#include "hardware/buzzer.h"
#include "hardware/button.h"
#include "hardware/lora_node.h"
#include "protocols/alert.h"

RTC_DATA_ATTR bool cooldown_active = false;

void setup() {
    Serial.begin(BAUD_RATE);
    while (!Serial);

    Serial.printf("\nNode ID: %d\n", DEVICE_ID);

    init_led();
    init_buzzer();
    init_button();
    init_lora_node();

    esp_sleep_enable_ext0_wakeup(BUTTON_WAKEUP_PIN, 0);

    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
        Serial.println("Wake: Button");
        if (!cooldown_active) {
            run_node_alert_sequence();
            cooldown_active = true;
            Serial.println("Cooldown 3s");
            delay(3000);
            cooldown_active = false;
        } else {
            Serial.println("Cooldown Active");
        }
    }
    
    Serial.println("Deep Sleep");
    LoRa.sleep();
    esp_deep_sleep_start();
}

void loop() {}