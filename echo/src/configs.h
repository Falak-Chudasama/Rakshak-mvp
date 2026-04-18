#pragma once

#define BAUD_RATE 115200

// Echo Hardware
#define LED_PIN 2 // Built-in blue LED on most ESP32 dev boards

// LoRa SPI Pins (Must match your Echo's physical wiring)
#define LORA_SCK_GPIO 18
#define LORA_MISO_GPIO 19
#define LORA_MOSI_GPIO 23
#define LORA_CS_GPIO 5
#define LORA_RST_GPIO 13
#define LORA_DIO0_GPIO 12