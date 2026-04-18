#include <Arduino.h>
#include "button.h"
#include "../configs.h"

void init_button() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

bool button_pressed() {
    return digitalRead(BUTTON_PIN) == LOW; 
}