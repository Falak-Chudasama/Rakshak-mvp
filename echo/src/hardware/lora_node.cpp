#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include "lora_node.h"
#include "../configs.h"
#include "../protocols/alert.h"

void init_lora_node() {
    Serial.println("Initializing LoRa...");
    
    // Override default pins
    LoRa.setPins(LORA_CS_GPIO, LORA_RST_GPIO, LORA_DIO0_GPIO);

    if (!LoRa.begin(433E6)) {
        Serial.println("Starting LoRa failed! Check your wiring.");
        while (1);
    }
    
    LoRa.setSpreadingFactor(10);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(8);
    LoRa.setTxPower(20);
    LoRa.enableCrc();
    Serial.println("LoRa Initialized Successfully!");
}

void lora_send_alert() {
    char tx_buffer[128];
    snprintf(tx_buffer, sizeof(tx_buffer), "ID:%d,LOC:%s", DEVICE_ID, DEVICE_LOCATION);

    Serial.printf("Sending: %s\n", tx_buffer);
    
    LoRa.beginPacket();
    LoRa.print(tx_buffer);
    LoRa.endPacket();
}

void lora_receive_alert() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        String incoming = "";
        while (LoRa.available()) {
            incoming += (char)LoRa.read();
        }

        int rx_id;
        char rx_loc[64];
        
        if (sscanf(incoming.c_str(), "ID:%d,LOC:%s", &rx_id, rx_loc) == 2) {
            Serial.printf("--- DECODED --- From ID: %d | Loc: %s | RSSI: %d dBm | SNR: %.1f\n", 
                rx_id, rx_loc, 
                LoRa.packetRssi(),
                LoRa.packetSnr()
            );
            alert_protocol(); // Trigger buzzer/LED
        } else {
            Serial.println("Received unknown format.");
        }
    }
}