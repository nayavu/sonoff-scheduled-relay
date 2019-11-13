//
// Created by naya.vu on 13.11.2019.
//

#ifndef SONOFF_SCHEDULER_APPWIFIMANAGER_H
#define SONOFF_SCHEDULER_APPWIFIMANAGER_H

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

#include <app/App.h>
#include <config/AppConfigManager.h>

#define WIFI_MAX_CONNECT_TIME 60 * 1000 // 1 min
#define ERROR_RECONNECT_TIME 10 * 60 * 1000 // 10 min
#define WIFI_AP_PASS "smarthome"

class AppWifiManager {
private:
    App* app;
    AppConfigManager* configManager;

    boolean setApMode;
    boolean setClientMode;
    unsigned long lastAttemptAt;

public:
    AppWifiManager(App* _app, AppConfigManager* _configManager) : app(_app), configManager(_configManager) {};
    void loop();
};


#endif //SONOFF_SCHEDULER_APPWIFIMANAGER_H
