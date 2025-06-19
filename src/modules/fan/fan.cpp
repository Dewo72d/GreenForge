#include "fan.h"
#include <Arduino.h>
#include "modules/init/init.h"

static uint8_t _fan_pin;
static uint8_t _fan_toggle_pin;
static uint8_t _fan_pwm_channel = 0;
static uint8_t _fan_pwm_value = 0;
static bool _fan_state = false;
static bool _fan_toggle_last_state = HIGH;

void fan_setup(uint8_t pin, uint8_t toggle_pin)
{
    _fan_pin = pin;
    ledcSetup(_fan_pwm_channel, 25000, 8);
    ledcAttachPin(_fan_pin, _fan_pwm_channel);
    ledcWrite(_fan_pwm_channel, 0);

    _fan_toggle_pin = toggle_pin;
    pinMode(_fan_toggle_pin, INPUT_PULLUP);
    digitalWrite(_fan_toggle_pin, LOW);

    _fan_state = false;
    _fan_pwm_value = 0;
}

void fan_pwm(uint8_t value)
{
    _fan_pwm_value = value;
    ledcWrite(_fan_pwm_channel, _fan_pwm_value);
    _fan_state = (_fan_pwm_value > 0);
}

void fan_off()
{
    ledcWrite(_fan_pwm_channel, 0);
    _fan_state = false;
    _fan_pwm_value = 0;
}

bool fan_pwm_state()
{
    return _fan_state;
}

uint8_t fan_pwm_value()
{
    return _fan_pwm_value;
}

void fan_toggle()
{
    bool current = digitalRead(_fan_toggle_pin);
    if (current == LOW)
    {
        _fan_state = true;
        ledcWrite(_fan_pwm_channel, _fan_pwm_value);
    }
    else
    {
        _fan_state = false;
        ledcWrite(_fan_pwm_channel, 0);
    }
}

void fan_register(Esp32express &server)
{
    server.on("/fan/pwm", [&server](WebServer &webServer)
              {
                  uint8_t value = 0;
                  if (webServer.hasArg("value")) {
                      value = webServer.arg("value").toInt();
                  } else if (webServer.method() == HTTP_POST && webServer.hasArg("plain")) {
                      String body = webServer.arg("plain");
                      value = body.toInt();
                  }
                  if (value > 255) value = 255;
                  fan_pwm(value);
                  String payload = get_json_data(server, "fan_pwm", String(value));
                  webServer.send(200, "application/json", payload); });

    server.on("/fan/off", [&server](WebServer &webServer)
              {
                  fan_off();
                  String payload = get_json_data(server, "fan", "OFF");
                  webServer.send(200, "application/json", payload); });

    server.on("/fan/state", [&server](WebServer &webServer)
              {
                  String state = fan_pwm_state() ? "ON" : "OFF";
                  String json_payload = get_json_data(server, "fan_state", state + ", value: " + String(fan_pwm_value()));
                  webServer.send(200, "application/json", json_payload); });
}
