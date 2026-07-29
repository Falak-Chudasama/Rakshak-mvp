#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include "lora_node.h"
#include "../configs.h"

void init_lora_node() {
    Serial.println("Init LoRa");
    LoRa.setPins(LORA_CS_GPIO, LORA_RST_GPIO, LORA_DIO0_GPIO);

    if (!LoRa.begin(433E6)) { // 433E6 | 866E6
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

void lora_send_alert() {
    char tx_buffer[128];
    snprintf(tx_buffer, sizeof(tx_buffer), "A,ID:%d,LOC:%s", DEVICE_ID, DEVICE_LOCATION);

    Serial.printf("TX: %s\n", tx_buffer);
    
    LoRa.beginPacket();
    LoRa.print(tx_buffer);
    LoRa.endPacket();
}

bool lora_wait_ack() {
    LoRa.receive();

    unsigned long start = millis();
    while (millis() - start < 4000) {
        int packetSize = LoRa.parsePacket();
        if (packetSize) {
            String incoming = "";
            while (LoRa.available()) {
                incoming += (char)LoRa.read();
            }

            int ack_id;
            if (sscanf(incoming.c_str(), "ACK:%d", &ack_id) == 1) {
                if (ack_id == DEVICE_ID) {
                    Serial.println("ACK RX");
                    LoRa.sleep();
                    return true;
                }
            }
        }

        delay(10);
    }

    LoRa.sleep();
    return false;
}