#pragma once
#include <esp32express.h>

void light_setup(uint8_t pin, uint8_t toggle_pin);
void light_register(Esp32express &server);
void light_on();
void light_off();
void light_toggle();
bool light_state();