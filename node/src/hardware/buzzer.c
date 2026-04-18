#include "../configs.h"
#include "driver/gpio.h"

void init_buzzer()
{
    gpio_reset_pin(BUZZER_PIN);
    gpio_set_direction(BUZZER_PIN, GPIO_MODE_OUTPUT);
}

void buzzer_on()
{
    gpio_set_level(BUZZER_PIN, 1);
}

void buzzer_off()
{
    gpio_set_level(BUZZER_PIN, 0);
}