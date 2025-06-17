#pragma once
#include <stdint.h>
#include "modules/init/init.h"

void fan_setup(uint8_t pin);
void fan_pwm(uint8_t value);
void fan_off();
void fan_toggle();
bool fan_pwm_state();
uint8_t fan_pwm_value();
void fan_register(Esp32express &server);