#include "../configs.h"
#include "driver/gpio.h"

void init_button() 
{
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);
}

bool button_pressed() 
{
    int state = gpio_get_level(BUTTON_PIN);

    if (state == 1) 
    {
        printf("Button Pressed\n");
        return true;
    }
    else 
    {
        printf("Button Released\n");
        return false;
    }
}