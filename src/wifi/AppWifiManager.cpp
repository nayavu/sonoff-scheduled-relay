//
// Created by naya.vu on 13.11.2019.
//

#include "AppWifiManager.h"

void AppWifiManager::loop() {
    if (app->state == APP_ERROR) {
        setApMode = false;
    }
    if (app->state == APP_CONFIG) {
        if (!setApMode) {
            WiFi.disconnect();

            DEBUGV("Setting up wifi AP with ssid=%s pass=%s\r\n", configManager->config->deviceName, WIFI_AP_PASS);
            setApMode = true;
            WiFi.mode(WIFI_AP);
            WiFi.softAP(configManager->config->deviceName, WIFI_AP_PASS);
            DEBUGV("IP: %s\r\n", WiFi.softAPIP().toString().c_str());

        }
        app->loop();
        return;
    }
    if (app->state == APP_CONNECTING_WIFI && !setClientMode) {
        DEBUGV("Connecting to wifi\r\n");
        setClientMode = true;

        WiFi.mode(WIFI_STA);
        WiFi.setAutoConnect(true);
        WiFi.begin(configManager->config->wifiSsid, configManager->config->wifiPass);
        WiFi.setSleepMode(WIFI_NONE_SLEEP);

        lastAttemptAt = millis();
    }

    if (WiFi.status() == WL_CONNECTED) {
        if (app->state == APP_CONNECTING_WIFI) {
            app->setState(APP_CONNECTED_WIFI);
            DEBUGV("Connected as %s\r\n", WiFi.localIP().toString().c_str());
            String domainName = String(configManager->config->deviceName) + ".local";
            if (MDNS.begin(domainName)) {
                DEBUGV("Registered mDNS %s\r\n", domainName.c_str());
            } else {
                DEBUGV("Could not register mDNS %s\r\n", domainName.c_str());
            }
        }
    } else {
        if (app->state == APP_CONNECTING_WIFI) {
            if (WiFi.status() == WL_NO_SSID_AVAIL) {
                DEBUGV("No SSID available\r\n");
                app->error(10);
                return;
            }

            unsigned long current = millis();
            if ((current > lastAttemptAt && current - lastAttemptAt > WIFI_MAX_CONNECT_TIME)
                || (current < lastAttemptAt && UINT32_MAX - lastAttemptAt + current > WIFI_MAX_CONNECT_TIME)) {
                // could not connect to wifi within WIFI_MAX_CONNECT_TIME
                DEBUGV("Could not connect to wifi (status=%d) within %d ms\r\n", WiFi.status(), WIFI_MAX_CONNECT_TIME);
                app->error(5);

                lastAttemptAt = millis();
            }
        } else if (app->state != APP_ERROR) {
            if (WiFi.status() == WL_DISCONNECTED) {
                DEBUGV("Disconnected WiFi, reconnecting\r\n");
                WiFi.reconnect();
            }

            app->setState(APP_CONNECTING_WIFI);
        } else {
            unsigned long current = millis();
            if ((current > lastAttemptAt && current - lastAttemptAt > ERROR_RECONNECT_TIME)
                || (current < lastAttemptAt && UINT32_MAX - lastAttemptAt + current > ERROR_RECONNECT_TIME)) {
                // try to reconnect
                DEBUGV("Was in error state within %d ms, will try to reconnect\r\n", ERROR_RECONNECT_TIME);
                app->setState(APP_CONNECTING_WIFI);

                lastAttemptAt = millis();
            }
        }
    }
}