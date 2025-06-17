#pragma once
#include <esp32express.h>

void wpump_setup(uint8_t pin);
void wpump_register(Esp32express &server);
void wpump_on(uint8_t pin);
void wpump_off(uint8_t pin);
bool wpump_state();