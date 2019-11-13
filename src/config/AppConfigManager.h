//
// Created by naya.vu on 30.09.2019.
//

#ifndef REMOTE_CONTROL_CONFIGMANAGER_H
#define REMOTE_CONTROL_CONFIGMANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include "ConfigManager.h"

struct __attribute__((__packed__)) AppConfig {
    char deviceName[33];
    char wifiSsid[33];
    char wifiPass[65];
    char ntpHost[100];
    int utcOffset; // offset in minutes between the local time zone and UTC
};

class AppConfigManager : public ConfigManager<AppConfig> {
public:
    AppConfigManager() : ConfigManager("/app.cfg") { sprintf(_config.deviceName, "scheduled-relay-%04x", ESP.getChipId()); };

    bool toJson(String& json);
    bool fromJson(const String& json);
};

#endif //REMOTE_CONTROL_CONFIGMANAGER_H
