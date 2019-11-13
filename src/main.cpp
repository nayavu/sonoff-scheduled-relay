#include <Arduino.h>

#include "app/App.h"
#include "wifi/AppWifiManager.h"
#include "web/UiWebServer.h"
#include "config/AppConfigManager.h"
#include "relay/ScheduledRelay.h"

#define LED_PIN 13
#define BUTTON_PIN 0
#define RELAY_PIN 12

App app(LED_PIN, BUTTON_PIN);
AppConfigManager configManager;
AppWifiManager wifiManager(&app, &configManager);
ScheduledRelay relay(&app, &configManager, RELAY_PIN);

UiWebServer server;

void setupServer() {
    server.on("/relay/config", HTTP_GET, []() {
        if (!relay.isPersisted()) {
            server.send(404);
            return;
        }

        if (!relay.load()) {
            server.send(500);
            return;
        }
        String buf = String();
        relay.toJson(buf);
        server.sendJsonResponse(200,  buf);
    });

    server.on("/relay/config", HTTP_POST, []() {
        bool res = relay.fromJson(server.arg("plain"));
        if (res) {
            app.notify();
            server.send(200);
        } else {
            app.error();
            server.send(400);
        }
    });

    server.on("/relay/state", HTTP_GET, []() {
        String buf = String("{\"on\":") + (relay.on ? "true" : "false") + ",\"time\":\"" + relay.getFormattedTime() + "\"}";
        server.sendJsonResponse(200, buf);
    });

    server.on("/config", HTTP_GET, []() {
        if (!configManager.isPersisted()) {
            server.send(404);
            return;
        }

        if (!configManager.load()) {
            server.send(500);
            return;
        }
        String buf = String();
        configManager.toJson(buf);
        server.sendJsonResponse(200,  buf);
    });

    server.on("/config", HTTP_POST, []() {
        bool res = configManager.fromJson(server.arg("plain"));
        if (res) {
            app.notify();
            server.send(200);
        } else {
            app.error();
            server.send(400);
        }
    });

    server.on("/reboot", HTTP_POST, []() {
        app.notify();
        app.setState(APP_REBOOTING);

        server.send(200);
    });

    server.onNotFound([]() {
        server.send(404);
    });
    server.begin();
}

void setup() {
    #ifdef DEBUG_ESP_PORT
    DEBUG_ESP_PORT.begin(115200);
    #endif
    SPIFFS.begin();

    app.setup();

    if (!configManager.load()) {
        app.setState(APP_CONFIG);
    } else {
        app.setState(APP_CONNECTING_WIFI);
    }

    setupServer();

    relay.setup();
}

void loop() {
    app.loop();
    wifiManager.loop();
    server.handleClient();
    relay.loop();
}