#include "light.h"
#include <Arduino.h>
#include "modules/init/init.h"

static uint8_t _light_pin;
static bool _light_state = false;
static bool _light_toggle_last_state = HIGH;
static uint8_t _light_toggle_pin;

void light_setup(uint8_t pin, uint8_t toggle_pin)
{
    _light_pin = pin;
    _light_toggle_pin = toggle_pin;
    pinMode(_light_pin, OUTPUT);
    digitalWrite(_light_pin, LOW);
    _light_state = false;
}

void light_on()
{
    _light_state = true;
    digitalWrite(_light_pin, HIGH);
}

void light_off()
{
    _light_state = false;
    digitalWrite(_light_pin, LOW);
}

bool light_state()
{
    return _light_state;
}

void light_toggle()
{
    bool current = digitalRead(_light_toggle_pin);
    if (current == LOW)
    {
        _light_state = true;
        digitalWrite(_light_pin, HIGH);
    }
    else
    {
        _light_state = false;
        digitalWrite(_light_pin, LOW);
    }
}

void light_register(Esp32express &server)
{
    server.on("/light/on", [&server](WebServer &webServer)
              {
                  light_on();
                  String payload = get_json_data(server, "light", "ON");
                  webServer.send(200, "application/json", payload); });

    server.on("/light/off", [&server](WebServer &webServer)
              {
                  light_off();
                  String payload = get_json_data(server, "light", "OFF");
                  webServer.send(200, "application/json", payload); });

    server.on("/light/state", [&server](WebServer &webServer)
              {
                  String state = light_state() ? "ON" : "OFF";
                  String payload = get_json_data(server, "light_state", state);
                  webServer.send(200, "application/json", payload); });
}
