//
// Created by naya.vu on 09.11.2019.
//

#ifndef SONOFF_SCHEDULER_APP_H_BAK
#define SONOFF_SCHEDULER_APP_H_BAK

#include <ArduinoOTA.h>
#include "Led.h"

enum AppState { APP_NONE, APP_CONFIG, APP_CONNECTING_WIFI, APP_CONNECTED_WIFI, APP_ERROR, APP_REBOOTING };

#define BUTTON_LONG_PRESS 3000
#define OTA_PASS "P@ssw0rd"

class App {
private:
    AppState _state = APP_NONE;
    Led led;
    byte buttonPin;
    int lastButtonState;
    unsigned long lastButtonStateChanged;
public:
    const AppState& state;
    App(byte ledPin, byte btnPin) : led(ledPin), buttonPin(btnPin), state(_state) { };
    AppState setState(AppState newState);

    void setup();
    void loop();

    void notify(byte blinks = 1);
    void error(byte blinks = 0);
};


#endif //SONOFF_SCHEDULER_APP_H_BAK
