#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi_types.h>
#include "definitions.h"

// Get encryption type name
String getEncryptionType(wifi_auth_mode_t encryptionType) {
    switch (encryptionType) {
        case WIFI_AUTH_OPEN: return "OPEN";
        case WIFI_AUTH_WEP: return "WEP";
        case WIFI_AUTH_WPA_PSK: return "WPA_PSK";
        case WIFI_AUTH_WPA2_PSK: return "WPA2_PSK";
        case WIFI_AUTH_WPA_WPA2_PSK: return "WPA_WPA2_PSK";
        case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2_ENTERPRISE";
        case WIFI_AUTH_WPA3_PSK: return "WPA3_PSK";
        case WIFI_AUTH_WPA2_WPA3_PSK: return "WPA2_WPA3_PSK";
        default: return "UNKNOWN";
    }
}

// Scan WiFi networks
void scanNetworks() {
    networks.clear();
    int n = WiFi.scanNetworks(false, true, false, 200);
    
    for (int i = 0; i < n; ++i) {
        WiFiNetwork net;
        net.ssid = WiFi.SSID(i);
        net.rssi = WiFi.RSSI(i);
        net.channel = WiFi.channel(i);
        net.encryption = WiFi.encryptionType(i);
        WiFi.BSSID(i, net.bssid);
        net.selected = false;
        networks.push_back(net);
    }
    WiFi.scanDelete();
}

// Print scanned networks
void printNetworks() {
    if (networks.size() == 0) {
        Serial.println("No networks found. Run 'scan' first.");
        return;
    }
    
    Serial.println("\nFound networks:");
    Serial.println("ID | SSID | RSSI | CH | ENC | BSSID");
    Serial.println("---+------+------ +----+-----+-----------------");
    
    for (size_t i = 0; i < networks.size(); i++) {
        Serial.printf("%2d | %s | %4d | %2d | %s | %02X:%02X:%02X:%02X:%02X:%02X\n",
            i + 1,
            networks[i].ssid.c_str(),
            networks[i].rssi,
            networks[i].channel,
            getEncryptionType(networks[i].encryption).c_str(),
            networks[i].bssid[0], networks[i].bssid[1], networks[i].bssid[2],
            networks[i].bssid[3], networks[i].bssid[4], networks[i].bssid[5]);
    }
}

// Print stations (placeholder)
void printStations() {
    Serial.println("Stations list not implemented");
}

// Generate random MAC
void getRandomMac(uint8_t* mac) {
    for (int i = 0; i < 6; i++) {
        mac[i] = random(256);
    }
    mac[0] &= 0xFE;  // Clear multicast bit
    mac[0] |= 0x02;  // Set local bit
}

// Set random MAC address
void setRandomMac() {
    uint8_t mac[6];
    getRandomMac(mac);
    esp_base_mac_addr_set(mac);
}
