//
// Created by naya.vu on 13.11.2019.
//

#ifndef SONOFF_SCHEDULER_LED_H
#define SONOFF_SCHEDULER_LED_H

#include <Arduino.h>

#define LED_BLINK_INTERVAL 100
#define LED_PATTERN_INTERVAL 200

#define LED_ON HIGH

enum LedPattern {
    LED_NONE = 0,
    LED_CONFIG = 0b00001111,
    LED_CONNECTING = 0b01010000,
    LED_CONNECTED = 0b01000000,
    LED_ERROR = 0b01010101
};

class Led {
private:
    byte pin;
    LedPattern pattern = LED_NONE;

    byte patternBit;
    unsigned long nextPatternBitChange = 0;
    bool nextPatternBitChangeOverflow;

    byte blinkBit = 0;
    unsigned long nextBlinkBitChange = 0;
    bool nextBlinkBitChangeOverflow;

public:
    Led(byte ledPin) : pin(ledPin) { };

    void setup();
    void loop();

    void setPattern(LedPattern pattern);
    void blink(byte count = 1);
};

#endif //SONOFF_SCHEDULER_LED_H
