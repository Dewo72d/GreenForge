#include "light.h"
#include <Arduino.h>
#include "modules/init/init.h"

static uint8_t _light_pin;
static bool _light_state = false;

void light_setup(uint8_t pin)
{
    _light_pin = pin;
    pinMode(_light_pin, OUTPUT);
    digitalWrite(_light_pin, LOW);
    _light_state = false;
}

void light_on()
{
    digitalWrite(_light_pin, HIGH);
    _light_state = true;
}

void light_off()
{
    digitalWrite(_light_pin, LOW);
    _light_state = false;
}

bool light_state()
{
    return _light_state;
}

void light_register(Esp32express &server)
{
    server.on("/light/on", [&server](WebServer &webServer)
              {
                  light_on();
                  String payload = get_json_data(server, "light", "ON");
                  webServer.send(200, "application/json", payload);
              });

    server.on("/light/off", [&server](WebServer &webServer)
              {
                  light_off();
                  String payload = get_json_data(server, "light", "OFF");
                  webServer.send(200, "application/json", payload);
              });

    server.on("/light/state", [&server](WebServer &webServer)
              {
                  String state = light_state() ? "ON" : "OFF";
                  String payload = get_json_data(server, "light_state", state);
                  webServer.send(200, "application/json", payload);
              });
}
