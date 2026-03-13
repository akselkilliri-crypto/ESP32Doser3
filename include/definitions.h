#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi_types.h>

// WiFi Scan structures
struct WiFiNetwork {
    String ssid;
    uint8_t bssid[6];
    int32_t rssi;
    uint8_t channel;
    wifi_auth_mode_t encryption;
    bool selected;
};

struct Station {
    String mac;
    String name;
    uint8_t macBytes[6];
    bool selected;
};

// Function declarations
String getEncryptionType(wifi_auth_mode_t encryptionType);
void scanNetworks();
void printNetworks();
void printStations();
void deauthNetwork(uint8_t networkIndex);
void deauthStation(uint8_t stationIndex);
void getRandomMac(uint8_t* mac);
void setRandomMac();
