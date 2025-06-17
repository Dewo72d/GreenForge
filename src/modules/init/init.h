#pragma once
#include <Arduino.h>
#include <esp32express.h>

void init_modules(Esp32express &server);
String get_json_data(Esp32express &server, String field = "", String payload = "");
