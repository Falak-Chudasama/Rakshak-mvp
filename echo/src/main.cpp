#include <Arduino.h>
#include <LoRa.h>
#include "configs.h"
#include "hardware/led.h"
#include "hardware/buzzer.h"
#include "hardware/button.h"
#include "hardware/lora_echo.h"
#include "protocols/alert.h"

void setup() {
    Serial.begin(BAUD_RATE);
    while (!Serial);

    Serial.println("\nEcho Started");

    init_led();
    init_buzzer();
    init_button();
    init_lora_node();
    
    esp_sleep_enable_ext0_wakeup(LORA_DIO0_WAKEUP_PIN, 1);
}

void loop() {
    LoRa.receive();
    Serial.flush();
    esp_light_sleep_start();
    
    int triggered_id = lora_receive_alert();
    
    if (triggered_id > 0) {
        Serial.printf("Active Alert ID: %d\n", triggered_id);
        unsigned long alert_start = millis();
        bool ack_sent = false;
        
        while (millis() - alert_start < 60000) {
            if (button_pressed()) {
                Serial.println("Guard ACK");
                lora_send_ack(triggered_id);
                ack_sent = true;
                break;
            }
            echo_alert_pattern(triggered_id);
        }
        
        if (!ack_sent) {
            Serial.println("Timeout");
        }
    }
}