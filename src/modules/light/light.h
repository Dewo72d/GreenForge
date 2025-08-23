#pragma once
#include <esp32express.h>

void light_setup(uint8_t pin);
void light_register(Esp32express &server);
void light_on();
void light_off();
bool light_state();