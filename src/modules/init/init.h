#pragma once
#include <ArduinoJson.h>
#include <Arduino.h>
#include <esp32express.h>

void init_modules(Esp32express &server);

String get_json_data(Esp32express &server, const String &field, const String &payload);
String get_json_data(Esp32express &server, const String &field, ArduinoJson::JsonVariant payload);
