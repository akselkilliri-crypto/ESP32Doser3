#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi_types.h>
#include "definitions.h"

// Get encryption type name
String getEncryptionType(wifi_auth_mode_t encryptionType) {
    switch (encryptionType) {
        case WIFI_AUTH_OPEN:
            return "OPEN";
        case WIFI_AUTH_WEP:
            return "WEP";
        case WIFI_AUTH_WPA_PSK:
            return "WPA_PSK";
        case WIFI_AUTH_WPA2_PSK:
            return "WPA2_PSK";
        case WIFI_AUTH_WPA_WPA2_PSK:
            return "WPA_WPA2_PSK";
        case WIFI_AUTH_WPA2_ENTERPRISE:
            return "WPA2_ENTERPRISE";
        case WIFI_AUTH_WPA3_PSK:
            return "WPA3_PSK";
        case WIFI_AUTH_WPA2_WPA3_PSK:
            return "WPA2_WPA3_PSK";
        default:
            return "UNKNOWN";
    }
}

// Scan WiFi networks
void scanNetworks() {
    WiFi.scanNetworks(true, true);
}

// Print scanned networks
void printNetworks() {
    int n = WiFi.scanComplete();
    if (n == WIFI_SCAN_FAILED) {
        Serial.println("Scan failed");
        return;
    } else if (n == WIFI_SCAN_RUNNING) {
        Serial.println("Scan in progress...");
        return;
    }
    
    Serial.println("Found networks:");
    for (int i = 0; i < n; ++i) {
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(") ");
        Serial.println(getEncryptionType(WiFi.encryptionType(i)));
    }
}

// Print stations
void printStations() {
    Serial.println("Stations list not implemented");
}

// Deauthenticate network
void deauthNetwork(uint8_t networkIndex) {
    Serial.print("Deauthing network: ");
    Serial.println(WiFi.SSID(networkIndex));
}

// Deauthenticate station
void deauthStation(uint8_t stationIndex) {
    Serial.print("Deauthing station: ");
    Serial.println(stationIndex);
}

// Generate random MAC
void getRandomMac(uint8_t* mac) {
    for (int i = 0; i < 6; i++) {
        mac[i] = random(256);
    }
    mac[0] &= 0xFE;
    mac[0] |= 0x02;
}

// Set random MAC address
void setRandomMac() {
    uint8_t mac[6];
    getRandomMac(mac);
    esp_base_mac_addr_set(mac);
}
