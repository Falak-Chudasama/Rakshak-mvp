#include <Arduino.h>
#include "alert.h"
#include "../configs.h"
#include "../hardware/led.h"
#include "../hardware/buzzer.h"
#include "../hardware/lora_node.h"

volatile bool alert_active = false;

void pulseTask(void *pvParameters) {
    while (alert_active) {
        buzzer_on();
        led_on();
        vTaskDelay(200 / portTICK_PERIOD_MS);
        
        if (!alert_active) break; 
        
        buzzer_off();
        led_off();
        vTaskDelay(300 / portTICK_PERIOD_MS);
    }
    
    buzzer_off();
    led_off();
    vTaskDelete(NULL);
}

void run_node_alert_sequence() {
    alert_active = true;
    
    xTaskCreate(
        pulseTask,
        "PulseTask",
        2048,
        NULL,
        1,
        NULL
    );

    for (int i = 0; i < 5; i++) {
        lora_send_alert();
        
        if (lora_wait_ack()) {
            Serial.println("Stopped by ACK");
            break;
        }
    }
    
    alert_active = false; 
    delay(400); 
}