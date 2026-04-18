#include <Arduino.h>
#include "configs.h"
#include "hardware/led.h"
#include "hardware/lora_echo.h"

void setup() {
    Serial.begin(BAUD_RATE);
    while (!Serial); 

    Serial.println("\n==================================");
    Serial.println("   Rakshak ECHO Node Started");
    Serial.println("==================================");

    init_led();
    init_lora_echo();
}

void loop() {
    // Continuously poll the LoRa module for incoming packets
    lora_listen();
    
    // A tiny 1ms delay prevents the ESP32 watchdog timer from crashing the board
    delay(1); 
}