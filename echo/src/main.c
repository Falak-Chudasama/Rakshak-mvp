#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"

#include "configs.h"
#include "hardware/button.h"
#include "hardware/led.h"
#include "hardware/buzzer.h"

void app_main()
{
    uart_set_baudrate(UART_NUM_0, BAUD_RATE);
    init_led();
    init_buzzer();
    init_button();
    while (1)
    {
        buzzer_on();
        led_on();
        vTaskDelay(pdMS_TO_TICKS(200));
        
        buzzer_off();
        led_off();
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}