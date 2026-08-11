#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include "lora_node.h"
#include "../configs.h"

static void lora_send_ack_ack(int id) {
    // NOTE: LoRa.endPacket() blocks until the packet has fully left the
    // antenna. At SF12/125kHz/CR4:8 a single "ACKACK:%d" packet takes
    // roughly 1.4-1.5 seconds of airtime. Previously this function sent
    // 3 copies back-to-back (~4.5s total), during which the radio is
    // physically transmitting and CANNOT receive anything -- this is
    // exactly the window in which echo, having timed out waiting, would
    // retransmit its ACK, and node was deaf to it. One clean send is
    // enough: if it's lost, echo's own retry loop (main.cpp, up to 60s)
    // will resend "ACK:%d" and node's next lora_wait_ack() call (part of
    // the up-to-8x alert retry loop in alert.cpp) will catch it and
    // ack-ack again.
    char tx_buffer[32];
    snprintf(tx_buffer, sizeof(tx_buffer), "ACKACK:%d", id);

    Serial.printf("TX: %s\n", tx_buffer);

    LoRa.idle();
    LoRa.beginPacket();
    LoRa.print(tx_buffer);
    LoRa.endPacket();
    LoRa.receive();
}

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
    // 3000ms: at SF12/125kHz/CR4:8, echo's "ACK:%d" reply alone takes
    // ~1.2s of airtime to transmit (see lora_send_ack_ack comment above
    // for the same airtime constraint). The previous 1000ms budget was
    // shorter than the time echo needed just to finish sending the ack,
    // so node would give up before the packet could possibly arrive.
    LoRa.receive();

    unsigned long start = millis();
    while (millis() - start < 3000) {
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
                    lora_send_ack_ack(ack_id);
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