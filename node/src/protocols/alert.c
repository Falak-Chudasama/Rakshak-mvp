#include "../configs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../hardware/led.h"
#include "../hardware/buzzer.h"

void alert_protocol()
{
    for (int i = 0; i < ALERT_DURATION; i++) 
    {
        int delay = 1000;
        for (int j = 0; j < DEVICE_ID; j++) 
        {
            buzzer_on();
            led_on();
            delay -= 150;
            vTaskDelay(pdMS_TO_TICKS(150));

            buzzer_off();
            led_off();
            delay -= 150;
            vTaskDelay(pdMS_TO_TICKS(150));
        }

        vTaskDelay(pdMS_TO_TICKS(delay));
    }
}