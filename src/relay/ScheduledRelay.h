//
// Created by naya.vu on 14.11.2019.
//

#ifndef SONOFF_SCHEDULER_SCHEDULEDRELAY_H
#define SONOFF_SCHEDULER_SCHEDULEDRELAY_H

#include <ArduinoJson.h>
#include <NTPClient.h>

#include <config/ConfigManager.h>
#include <app/App.h>
#include <config/AppConfigManager.h>

#define NTP_UPDATE_INTERVAL 10 * 60 * 1000

struct __attribute__((__packed__)) Schedule {
    uint16_t startTime;
    uint16_t stopTime;
};

class ScheduledRelay : public ConfigManager<Schedule> {
private:
    App* _app;
    AppConfigManager* _appConfig;
    WiFiUDP _udp;
    NTPClient* _ntpClient;

    bool _timeInitialised;

    bool _on;
    byte _relayPin;
public:
    bool& on;
    String getFormattedTime();

    ScheduledRelay(App* app, AppConfigManager* appConfig, byte relayPin) : ConfigManager("/schedule.cfg"), _app(app), _appConfig(appConfig), _relayPin(relayPin), on(_on) {};

    void setup();
    void loop();

    bool toJson(String& json);
    bool fromJson(const String& json);
};


#endif //SONOFF_SCHEDULER_SCHEDULEDRELAY_H
