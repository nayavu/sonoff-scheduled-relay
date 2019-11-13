//
// Created by naya.vu on 13.11.2019.
//

#include "App.h"

AppState App::setState(AppState newState) {
    AppState oldState = _state;
    _state = newState;
    switch (_state) {
        case APP_NONE:
            led.setPattern(LED_NONE);
            break;
        case APP_CONFIG:
            led.setPattern(LED_CONFIG);
            ArduinoOTA.begin(false);
            break;
        case APP_CONNECTING_WIFI:
            led.setPattern(LED_CONNECTING);
            break;
        case APP_CONNECTED_WIFI:
            led.setPattern(LED_CONNECTED);
            break;
        case APP_ERROR:
            led.setPattern(LED_ERROR);
            break;
        case APP_REBOOTING:
            led.blink(1);
            delay(100);
            ESP.restart();
            break;
    }
    DEBUGV("Changed state to %d from %d\r\n", _state, oldState);
    return oldState;
}

void App::setup() {
    led.setup();
    pinMode(buttonPin, INPUT);
    lastButtonState = LOW;
    lastButtonStateChanged = 0;

    // setup OTA
    ArduinoOTA.setPort(8266);
    ArduinoOTA.setRebootOnSuccess(true);
    ArduinoOTA.setPassword(OTA_PASS);
    ArduinoOTA.onStart([this]() {
        led.blink(5);
        if (ArduinoOTA.getCommand() == U_FLASH) {
            DEBUGV("Start OTA updating sketch\r\n");
        } else { // U_FS
            DEBUGV("Start OTA updating filesystem\r\n");
        };
    });
    ArduinoOTA.onEnd([this]() {
        led.blink(5);
        DEBUGV("End OTA updating\r\n");
    });
    ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total) {
        led.blink(1);
        DEBUGV("Progress: %u%%\r\n", (progress / (total / 100)));
    });
    ArduinoOTA.onError([this](ota_error_t error) {
        if (error == OTA_AUTH_ERROR) {
            DEBUGV("OTA auth Failed\r\n");
        } else if (error == OTA_BEGIN_ERROR) {
            DEBUGV("OTA begin Failed\r\n");
        } else if (error == OTA_CONNECT_ERROR) {
            DEBUGV("OTA connect Failed\r\n");
        } else if (error == OTA_RECEIVE_ERROR) {
            DEBUGV("OTA receive failed\r\n");
        } else if (error == OTA_END_ERROR) {
            DEBUGV("OTA end failed\r\n");
        } else {
            DEBUGV("Unknown error %d\r\n", error);
        }
        led.blink(10);
    });
}

void App::loop() {
    ArduinoOTA.handle();

    led.loop();

    int buttonState = digitalRead(buttonPin);
    if (buttonState != lastButtonState) {
        if (buttonState == LOW) {
            if (millis() - lastButtonStateChanged > BUTTON_LONG_PRESS) {
                setState(APP_CONFIG);
            } else {
                setState(APP_REBOOTING);
            }
        }
        lastButtonState = buttonState;
        lastButtonStateChanged = millis();
    }
}

void App::notify(byte blinks) {
    led.blink(blinks);
}

void App::error(byte blinks) {
    setState(APP_ERROR);
    if (blinks) {
        notify(blinks);
    }
}