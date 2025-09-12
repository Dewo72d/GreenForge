#include <Adafruit_AHTX0.h>
#include <Arduino.h>
#include "modules/init/init.h"
#include <ArduinoJson.h>

Adafruit_AHTX0 aht;

static bool _temphum_state = false;

void temphum_setup()
{

    if (!aht.begin())
    {
        Serial.println("AHT10: Check wiring");
        while (1)
            delay(10);
    }
    Serial.println("AHT10 found");
    _temphum_state = true;
}

void temphum_register(Esp32express &server)
{

    server.on("/temphum/state", [&server](WebServer &webServer)
              {
                sensors_event_t humidity, temp;
                aht.getEvent(&humidity, &temp);

                DynamicJsonDocument doc(128);
                doc["temperature"] = temp.temperature;
                doc["humidity"] = humidity.relative_humidity;
        
                String payload = get_json_data(server, "temphum", doc.as<JsonVariant>());
                webServer.send(200, "application/json", payload); });
}