//
// Created by naya.vu on 11.09.2019.
//
#include "web/UiWebServer.h"

void UiWebServer::begin() {
    // ESP8266WebServer tries to load gzipped file if there's no original file present on the filesystem,
    // i.e for `/main.js` path, it will try to load `/main.js.gz` if there's no `/main.js`.
    // All our UI files are gzipped

    DEBUGV("index.html exists: %d\r\n", SPIFFS.exists("/index.html"));
    serveStatic("/index.html", SPIFFS, "/index.html", "");
    serveStatic("/favicon.ico", SPIFFS, "/favicon.ico", "");
    serveStatic("/main.js", SPIFFS, "/main.js", "");
    serveStatic("/polyfills.js", SPIFFS, "/polyfills.js", "");
    serveStatic("/runtime.js", SPIFFS, "/runtime.js", "");
    serveStatic("/styles.css", SPIFFS, "/styles.css", "");
    serveStatic("/", SPIFFS, "/index.html", "");
    ESP8266WebServer::begin();
}

void UiWebServer::sendJsonResponse(byte code, String& content) {
    send(code, "application/json", content);
}