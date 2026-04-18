#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include "lora_echo.h"
#include "../configs.h"
#include "../protocols/alert.h"

void init_lora_echo() {
    Serial.println("Initializing Echo LoRa...");
    
    LoRa.setPins(LORA_CS_GPIO, LORA_RST_GPIO, LORA_DIO0_GPIO);

    if (!LoRa.begin(433E6)) {
        Serial.println("Echo LoRa failed to start! Check wiring.");
        while (1); 
    }
    
    LoRa.enableCrc(); // Must be enabled to match the transmitter
    Serial.println("Echo LoRa Initialized. Listening for alerts...");
}

void lora_listen() {
    int packetSize = LoRa.parsePacket();
    
    if (packetSize) {
        String incoming = "";
        // Read the entire packet into a string
        while (LoRa.available()) {
            incoming += (char)LoRa.read();
        }

        int rx_id;
        char rx_loc[64];
        
        // Only trigger the alert IF the packet perfectly matches your protocol format
        if (sscanf(incoming.c_str(), "ID:%d,LOC:%s", &rx_id, rx_loc) == 2) {
            trigger_echo_alert(rx_id, rx_loc);
        } else {
            Serial.printf("Ignored unknown or corrupted packet: %s\n", incoming.c_str());
        }
    }
}