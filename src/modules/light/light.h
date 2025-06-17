#pragma once
#include <esp32express.h>

void light_setup(uint8_t pin);
void light_register(Esp32express &server);
void light_on(uint8_t pin);
void light_off(uint8_t pin);
bool light_state();