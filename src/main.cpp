#include <WiFi.h>
#include <esp_wifi.h>
#include "types.h"
#include "serial_interface.h"
#include "deauth.h"
#include "definitions.h"

int curr_channel = 1;

void setup() {
    #ifdef LED
    pinMode(LED, OUTPUT);
    #endif
    
    WiFi.mode(WIFI_MODE_STA);
    start_serial_interface();
}

void loop() {
    if (deauth_type == DEAUTH_TYPE_ALL) {
        if (curr_channel > CHANNEL_MAX) curr_channel = 1;
        esp_wifi_set_channel(curr_channel, WIFI_SECOND_CHAN_NONE);
        curr_channel++;
        delay(10);
    } else {
        serial_interface_handle();
    }
}
