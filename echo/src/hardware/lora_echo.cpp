#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include "lora_echo.h"
#include "../configs.h"

void init_lora_node() {
    Serial.println("Init LoRa");
    LoRa.setPins(LORA_CS_GPIO, LORA_RST_GPIO, LORA_DIO0_GPIO);

    if (!LoRa.begin(866E6)) { // 433E6 | 866E6
        Serial.println("LoRa Failed");
        while (1);
    }
    
    LoRa.setSpreadingFactor(12);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(8);
    LoRa.setTxPower(20);
    LoRa.setPreambleLength(16);
    LoRa.enableCrc();
    Serial.println("LoRa Ready");
}

int lora_receive_alert() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        String incoming = "";
        while (LoRa.available()) {
            incoming += (char)LoRa.read();
        }

        int rx_id;
        char rx_loc[64];
        
        if (sscanf(incoming.c_str(), "A,ID:%d,LOC:%s", &rx_id, rx_loc) == 2) {
            Serial.printf("RX ALERT | ID: %d | Loc: %s | RSSI: %d | SNR: %.1f\n", 
                rx_id, rx_loc, LoRa.packetRssi(), LoRa.packetSnr());
            return rx_id;
        }
    }
    return -1;
}

void lora_send_ack(int id) {
    char tx_buffer[32];
    snprintf(tx_buffer, sizeof(tx_buffer), "ACK:%d", id);

    Serial.printf("TX: %s\n", tx_buffer);
    
    LoRa.beginPacket();
    LoRa.print(tx_buffer);
    LoRa.endPacket();
}