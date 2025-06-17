#include "wpump.h"
#include <Arduino.h>
#include "modules/init/init.h"

static uint8_t _wpump_pin;
static bool _wpump_state = false;

void wpump_setup(uint8_t pin)
{
    _wpump_pin = pin;
    pinMode(_wpump_pin, OUTPUT);
    digitalWrite(_wpump_pin, LOW);
    _wpump_state = false;
}

void wpump_on()
{
    digitalWrite(_wpump_pin, HIGH);
    _wpump_state = true;
}

void wpump_off()
{
    digitalWrite(_wpump_pin, LOW);
    _wpump_state = false;
}

bool wpump_state()
{
    return _wpump_state;
}

void wpump_register(Esp32express &server)
{
    server.on("/wpump/on", [&server](WebServer &webServer)
              {
                  wpump_on();
                  String payload = get_json_data(server, "wpump", "ON");
                  webServer.send(200, "application/json", payload); });

    server.on("/wpump/off", [&server](WebServer &webServer)
              {
                  wpump_off();
                  String payload = get_json_data(server, "wpump", "OFF");
                  webServer.send(200, "application/json", payload); });

    server.on("/wpump/state", [&server](WebServer &webServer)
              {
                  String state = wpump_state() ? "ON" : "OFF";
                  String payload = get_json_data(server, "wpump_state", state);
                  webServer.send(200, "application/json", payload); });
}
