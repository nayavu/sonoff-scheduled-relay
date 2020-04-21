//
// Created by naya.vu on 14.11.2019.
//

#include "ScheduledRelay.h"

void ScheduledRelay::setup() {
    _ntpClient = new NTPClient(_udp, _appConfig->config->ntpHost, _appConfig->config->utcOffset * 60, NTP_UPDATE_INTERVAL);
    _ntpClient->begin();
    pinMode(_relayPin, OUTPUT);

    load();
}

void ScheduledRelay::loop() {
    if (_app->state == APP_CONNECTED_WIFI) {
        bool res = _ntpClient->update();
        if (!_timeInitialised) {
            _timeInitialised = res;
        }
    }
    if (!_timeInitialised) {
        // do not schedule any actions if we do not know time
        return;
    }
    unsigned int minutesOfDay = (_ntpClient->getEpochTime() / 60L) % (24L * 60L);

    bool expectedState = false;
    if (config->stopTime >= config->startTime) {
        expectedState = minutesOfDay >= config->startTime && minutesOfDay < config->stopTime;
    } else if (config->stopTime < config->startTime) {
        expectedState = minutesOfDay >= config->startTime || minutesOfDay < config->stopTime;
    }
    if (expectedState != _power) {
        _power = expectedState;
        DEBUGV("Switching relay %s\r\n", _power ? "ON" : "OFF");
        digitalWrite(_relayPin, _power);
    }
}

String ScheduledRelay::getFormattedTime() {
    return _timeInitialised ? _ntpClient->getFormattedTime() : String();
}

bool ScheduledRelay::toJson(String& json) {
    DynamicJsonDocument doc(JSON_OBJECT_SIZE(2) + 30);
    doc["startTime"] = _config.startTime;
    doc["stopTime"] = _config.stopTime;
    return serializeJson(doc, json) != 0;
}

bool ScheduledRelay::fromJson(const String& json) {
    DynamicJsonDocument doc(JSON_OBJECT_SIZE(2) + 30);
    DeserializationError error = deserializeJson(doc, json);
    if (error) {
        return false;
    }
    if (doc.containsKey("startTime")) {
        _config.startTime = doc["startTime"];
    }
    if (doc.containsKey("stopTime")) {
        _config.stopTime = doc["stopTime"];
    }
    return persist();
}