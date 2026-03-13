#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include "definitions.h"

// Global variables
int curr_channel = 1;
std::vector<WiFiNetwork> networks;
std::vector<Station> stations;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n\n=== WiFi Deauther Starting ===");
    
    // Init LED
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    
    // Init WiFi
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    
    // Set random MAC
    setRandomMac();
    
    Serial.println("Setup complete!");
    Serial.println("Commands:");
    Serial.println("  scan          - Scan networks");
    Serial.println("  show          - Show networks");
    Serial.println("  deauth <n>    - Deauth network n (single)");
    Serial.println("  deauth all    - Deauth all networks");
    Serial.println("  stop          - Stop attack");
    Serial.println("==============================");
}

void loop() {
    // Handle serial commands
    serial_interface_handle();
    
    // Channel hop if attack is active
    if (deauth_type == DEAUTH_TYPE_ALL) {
        curr_channel++;
        if (curr_channel > CHANNEL_MAX) curr_channel = CHANNEL_MIN;
        esp_wifi_set_channel(curr_channel, WIFI_SECOND_CHAN_NONE);
        delay(100);
    }
    
    delay(50);
}
