#include <Arduino.h>
#include <esp32express.h>
#include "modules/init/init.h"

// Wi-Fi
const char *ssid = "wifi";
const char *password = "wifi123";
// Server
const uint16_t port = 3000;
const uint8_t octet = 150;
Esp32express server(ssid, password, port, octet);

void setup()
{
  Serial.begin(115200);
  Serial.println("START >>>");

  String mac = WiFi.macAddress();
  Serial.print("MAC address: ");
  Serial.println(mac);
  server.connectWiFi();

  init_modules(server);

  Serial.println("Program begun!");

  server.on("/ping", [](WebServer &webServer)
            { 
              String payload = get_json_data(server, "ping", "pong");
              webServer.send(200, "application/json", payload);
                            Serial.println("SEND"); });
}

void loop()
{
  server.handleClient();
  init_toggles_handler();
}