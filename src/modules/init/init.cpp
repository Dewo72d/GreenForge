#include <esp32/rom/ets_sys.h>
// modules
#include "modules/light/light.h"
#include "modules/fan/fan.h"
#include "modules/wpump/wpump.h"
#include "modules/temphum/temphum.h"
// Pins
#define LIGHT_PIN 23
#define FAN_PIN 19
#define WPUMP_PIN 5

void init_modules(Esp32express &server)
{
    // Initialize light module
    light_setup(LIGHT_PIN);
    light_register(server);

    // Initialize fan module
    fan_setup(FAN_PIN);
    fan_register(server);

    // Initialize water pump module
    wpump_setup(WPUMP_PIN);
    wpump_register(server);

    // Initialize temperature and humidity sensor module ATH10
    temphum_setup();
    temphum_register(server);
}

String get_json_data(Esp32express &server, const String &field, const String &payload)
{
    JsonDocument data;
    data["ip"] = server.deviceIP();
    data["heap"] = esp_get_free_heap_size();
    data[field] = payload;

    String out;
    serializeJson(data, out);
    return out;
}

String get_json_data(Esp32express &server, const String &field, ArduinoJson::JsonVariant payload)
{
    JsonDocument data;
    data["ip"] = server.deviceIP();
    data["heap"] = esp_get_free_heap_size();
    data[field] = payload;

    String out;
    serializeJson(data, out);
    return out;
}