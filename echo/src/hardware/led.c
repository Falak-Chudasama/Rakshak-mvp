#include "../configs.h"
#include "driver/gpio.h"

void init_led() 
{
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
}

void led_on() 
{
    gpio_set_level(LED_PIN, 1);
}

void led_off() 
{
    gpio_set_level(LED_PIN, 0);
}