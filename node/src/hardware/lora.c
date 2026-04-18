#include "../configs.h"
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "LoRa.h"
#include "../protocols/alert.h"

void init_lora()
{
    ESP_LOGI(TAG, "Initializing LoRa...");
    lora_init();

    lora_set_frequency(433e6);
    lora_enable_crc();

    ESP_LOGI(TAG, "LoRa initialized at 433MHz");
}

void lora_send_alert(void)
{   
    char tx_buffer[128];
    int msg_len = snprintf(tx_buffer, sizeof(tx_buffer),
                            "ID:%s,LOC:%s",
                            DEVICE_ID,
                            DEVICE_LOCATION);

    if (msg_len < 0)
    {
        ESP_LOGE(TAG, "Encoding error while formatting alert");
        return;
    }

    ESP_LOGI(TAG, "Sending LoRa Alert: %s", tx_buffer);

    lora_send_packet((uint8_t *)tx_buffer, msg_len);

    ESP_LOGI(TAG, "Alert sent successfully!");
    // call protocol function to buzz and lit.
}

void lora_receive_alert(void)
{
    if (lora_received())
    {
        uint8_t buf[128];
        int len = lora_receive_packet(buf, sizeof(buf));
        
        if (len > 0)
        {
            buf[len] = '\0';
            
            char rx_id[32];
            char rx_loc[64];
            
            int matches = sscanf((char *)buf, "ID:%[^,],LOC:%s", rx_id, rx_loc);
            
            if (matches == 2)
            {
                ESP_LOGI(TAG, "--- ALERT DECODED ---");
                ESP_LOGI(TAG, "From Device: %s", rx_id);
                ESP_LOGI(TAG, "At Location: %s", rx_loc);
                alert_protocol();
            }
            else
            {
                ESP_LOGW(TAG, "Received unknown packet format: %s", (char *)buf);
            }
        }
    }
}