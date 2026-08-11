#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include "lora_echo.h"
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

    LoRa.idle();
    LoRa.beginPacket();
    LoRa.print(tx_buffer);
    LoRa.endPacket();
    LoRa.receive();
}

bool lora_wait_ack_ack(int id, unsigned long timeout_ms) {
    // At SF12/125kHz/CR4:8, node's single "ACKACK:%d" reply takes
    // roughly 1.4-1.5s of airtime by itself, on top of the time node
    // needs to detect our "ACK:%d" and start its own transmit. Callers
    // should pass a timeout comfortably above that (main.cpp now passes
    // 3000ms instead of the previous 1000ms, which was shorter than the
    // ack-ack's airtime alone).
    LoRa.receive();

    unsigned long start = millis();
    while (millis() - start < timeout_ms) {
        int packetSize = LoRa.parsePacket();
        if (packetSize) {
            String incoming = "";
            while (LoRa.available()) {
                incoming += (char)LoRa.read();
            }

            int ackack_id;
            if (sscanf(incoming.c_str(), "ACKACK:%d", &ackack_id) == 1) {
                if (ackack_id == id) {
                    Serial.printf("ACKACK RX | ID: %d\n", ackack_id);
                    LoRa.sleep();
                    return true;
                }
            }
        }

        delay(10);
    }

    return false;
}