//
// Created by naya.vu on 30.09.2019.
//

#include "AppConfigManager.h"

bool AppConfigManager::toJson(String& json) {
    DynamicJsonDocument doc(JSON_OBJECT_SIZE(7) + 260);
    doc["deviceName"] = _config.deviceName;
    doc["wifiSsid"] = _config.wifiSsid;
    doc["wifiPass"] = _config.wifiPass;
    doc["ntpHost"] = _config.ntpHost;
    doc["utcOffset"] = _config.utcOffset;
    return serializeJson(doc, json) != 0;
}

bool AppConfigManager::fromJson(const String& json) {
    DynamicJsonDocument doc(JSON_OBJECT_SIZE(7) + 260);
    DeserializationError error = deserializeJson(doc, json);
    if (error) {
        return false;
    }
    const char *deviceName = doc["deviceName"];
    const char *wifiSsid = doc["wifiSsid"];
    const char *wifiPass = doc["wifiPass"];
    const char *ntpHost = doc["ntpHost"];

    if (!deviceName || !wifiSsid) {
        return false;
    }
    strncpy(_config.deviceName, deviceName, sizeof(_config.deviceName) - 1);
    strncpy(_config.wifiSsid, wifiSsid, sizeof(_config.wifiSsid) - 1);
    if (wifiPass) {
        strncpy(_config.wifiPass, wifiPass, sizeof(_config.wifiPass) - 1);
    }
    if (ntpHost) {
        strncpy(_config.ntpHost, ntpHost, sizeof(_config.ntpHost) - 1);
    }
    if (doc.containsKey("utcOffset")) {
        _config.utcOffset = doc["utcOffset"];
    }
    return persist();
}