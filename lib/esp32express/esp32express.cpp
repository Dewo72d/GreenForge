#include "esp32express.h"

Esp32express::Esp32express(const char *ssid, const char *password, const uint16_t port, const uint8_t octet)
    : _ssid(ssid), _password(password), _port(port), _octet(octet), _server(port) {};

void Esp32express::connectWiFi()
{
    Serial.print("Connect to:");
    Serial.println(_ssid);

    IPAddress local_IP(192, 168, 0, _octet);
    IPAddress gateway(192, 168, 0, 1);
    IPAddress subnet(255, 255, 255, 0);
    IPAddress primaryDNS(8, 8, 8, 8);
    IPAddress secondaryDNS(8, 8, 4, 4);

    // Config static IP
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
    {
        Serial.println("STA Failed to configure");
    }

    WiFi.begin(_ssid, _password);

    unsigned long startTimer = millis();
    const unsigned long wifiTimeout = 18000;

    while (WiFi.status() != WL_CONNECTED)
    {
        if (millis() - startTimer >= wifiTimeout)
        {
            Serial.println("ERROR: connect to WIFI: Time Out");
            return;
        }

        yield();
    }

    Serial.println("Connected to WIFI");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    _server.begin();
    Serial.println("SERVER IS STARTED");
}

void Esp32express::handleClient()
{
    _server.handleClient();
}

void Esp32express::on(const String &uri, std::function<void(WebServer &)> handler)
{
    _server.on(uri.c_str(), [this, handler]()
               { handler(_server); });
}

String Esp32express::httpGET(const String &url)
{
    WiFiClient client;
    HTTPClient http;

    http.begin(client, url);
    int httpCode = http.GET();

    String response = "";

    if (httpCode > 0)
    {
        response = http.getString();
        Serial.print("HTTP Code: ");
        Serial.println(httpCode);
    }
    else
    {
        Serial.print("REQUEST GET ERROR: ");
        Serial.println(http.errorToString(httpCode));
    }

    http.end();
    return response;
}

String Esp32express::httpPOST(const String &url, const String &payload, const bool isJson)
{
    WiFiClient client;
    HTTPClient http;

    http.begin(client, url);
    if (isJson)
    {
        http.addHeader("Content-Type", "application/json");
    }
    else
    {
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    }
    http.addHeader("User-Agent", "ESP32Client");

    int httpCode = http.POST(payload);
    String response = "";

    if (httpCode > 0)
    {
        response = http.getString();
    }
    else
    {
        Serial.print("REQUEST POST ERROR: ");
        Serial.println(http.errorToString(httpCode));
    }

    http.end();
    return response;
}

String Esp32express::deviceIP()
{
    IPAddress ip = WiFi.localIP();
    return ip.toString();
}
