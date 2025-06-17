#ifndef ESP32EXPRESS
#define ESP32EXPRESS

#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>

class Esp32express
{
public:
    Esp32express(const char *ssid, const char *password, const uint16_t port, const uint8_t octet);

    void connectWiFi();
    void handleClient();

    String httpGET(const String &url);

    String httpPOST(const String &url, const String &payload, const bool isJson);

    String deviceIP();

    void on(const String &uri, std::function<void(WebServer &)> handler);

private:
    const char *_ssid;
    const char *_password;
    const uint16_t _port;
    const uint8_t _octet;
    WebServer _server;
};

#endif
