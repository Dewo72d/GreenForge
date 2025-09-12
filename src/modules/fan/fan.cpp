#include "fan.h"
#include <Arduino.h>
#include "modules/init/init.h"
#include <ArduinoJson.h>

static uint8_t _fan_pin;
static uint8_t _fan_pwm_channel = 0;
static uint8_t _fan_pwm_value = 0;
static bool _fan_state = false;

void fan_setup(uint8_t pin)
{
    _fan_pin = pin;
    ledcSetup(_fan_pwm_channel, 25000, 8);
    ledcAttachPin(_fan_pin, _fan_pwm_channel);
    ledcWrite(_fan_pwm_channel, 0);

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

void fan_register(Esp32express &server)
{
    String response = server.httpGET("http://192.168.0.103:3001/api/fan/init");
    Serial.println("Fan init response: " + response);

    if (response.length() > 0)
    {
        DynamicJsonDocument doc(256);
        DeserializationError err = deserializeJson(doc, response);
        if (!err)
        {
            int rawValue = doc["value"] | 0;
            bool state = doc["state"] | false;

            uint8_t pwmValue = rawValue > 255 ? 255 : rawValue;
            if (!state)
                pwmValue = 0;
            fan_pwm(pwmValue);

            Serial.printf("Init fan: serverState=%s rawValue=%d appliedPWM=%u\n",
                          state ? "ON" : "OFF", rawValue, pwmValue);
        }
        else
        {
            Serial.print("JSON parse error: ");
            Serial.println(err.f_str());
        }
    }

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

        DynamicJsonDocument doc(128);
        doc["state"] = fan_pwm_state();
        doc["value"] = fan_pwm_value();
        String payload = get_json_data(server, "fan", doc.as<JsonVariant>());
        webServer.send(200, "application/json", payload); });

    server.on("/fan/off", [&server](WebServer &webServer)
              {
        fan_off();
        DynamicJsonDocument doc(128);
        doc["state"] = false;
        doc["value"] = 0;
        String payload = get_json_data(server, "fan", doc.as<JsonVariant>());
        webServer.send(200, "application/json", payload); });

    server.on("/fan/state", [&server](WebServer &webServer)
              {
        DynamicJsonDocument doc(128);
        doc["state"] = fan_pwm_state();
        doc["value"] = fan_pwm_value();
        String payload = get_json_data(server, "fan", doc.as<JsonVariant>());
        webServer.send(200, "application/json", payload); });
}