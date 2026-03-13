#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi_types.h>

// ===== DEAUTH ATTACK TYPES =====
#define DEAUTH_TYPE_NONE 0
#define DEAUTH_TYPE_SINGLE 1
#define DEAUTH_TYPE_ALL 2

// ===== DEAUTH SETTINGS =====
#define NUM_FRAMES_PER_DEAUTH 10
#define DEAUTH_BLINK_TIMES 3
#define DEAUTH_BLINK_DURATION 100

// ===== CHANNEL SETTINGS =====
#define CHANNEL_MIN 1
#define CHANNEL_MAX 14

// ===== LED SETTINGS =====
#ifndef LED_PIN
#define LED_PIN 2  // Встроенный светодиод на ESP32
#endif

#define BLINK_LED(times, duration) \
    do { \
        pinMode(LED_PIN, OUTPUT); \
        for(int _i = 0; _i < times; _i++) { \
            digitalWrite(LED_PIN, HIGH); \
            delay(duration); \
            digitalWrite(LED_PIN, LOW); \
            delay(duration); \
        } \
    } while(0)

// ===== DEBUG MACROS =====
#ifdef DEBUG_MODE
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)
    #define DEBUG_PRINTF(...) Serial.printf(__VA_ARGS__)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
    #define DEBUG_PRINTF(...)
#endif

// ===== WIFI STRUCTURES =====
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

// ===== GLOBAL VARIABLES (extern) =====
extern int deauth_type;
extern int curr_channel;
extern std::vector<WiFiNetwork> networks;
extern std::vector<Station> stations;

// ===== FUNCTION DECLARATIONS =====
String getEncryptionType(wifi_auth_mode_t encryptionType);
void scanNetworks();
void printNetworks();
void printStations();
void deauthNetwork(uint8_t networkIndex);
void deauthStation(uint8_t stationIndex);
void getRandomMac(uint8_t* mac);
void setRandomMac();
void start_deauth(int wifi_number, int type, uint16_t reason);
void stop_deauth();
void sniffer(void* buf, wifi_promiscuous_pkt_type_t type);
void serial_interface_handle();
