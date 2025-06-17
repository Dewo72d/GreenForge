#include <ArduinoJson.h>
#include <esp32/rom/ets_sys.h>
// modules
#include "modules/light/light.h"
#include "modules/fan/fan.h"
#include "modules/wpump/wpump.h"
// Pins
#define LIGHT_PIN 23
#define FAN_PIN 19
#define WPUMP_PIN 18

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
}

// This function is called periodically to check the toggle state of the fan
void init_toggles_handler()
{
    static uint32_t lastToggleMillis = 0;
    static const uint32_t toggleInterval = 50; // 0.5 second
    uint32_t now = millis();
    if (now - lastToggleMillis >= toggleInterval)
    {
        lastToggleMillis = now;
        fan_toggle();
    }
}

String get_json_data(Esp32express &server, String field, String payload)
{
    StaticJsonDocument<200> data;
    uint32_t heap = esp_get_free_heap_size();
    String ip = server.deviceIP();

    data["ip"] = ip;
    data["heap"] = heap;
    data[field] = payload;

    String json;
    serializeJson(data, json);

    return json;
}