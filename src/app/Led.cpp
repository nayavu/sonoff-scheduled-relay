//
// Created by naya.vu on 13.11.2019.
//

#include "Led.h"

void Led::setup() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, !LED_ON);
}

void Led::setPattern(LedPattern pattern) {
    this->pattern = pattern;
    patternBit = 0;
    nextPatternBitChangeOverflow = false;
    nextPatternBitChange = 0;
}

void Led::blink(byte count) {
    // 1 blink => OFF, ON, OFF
    // 2 blinks => OFF, ON, OFF, ON, OFF
    blinkBit = count * 2 + 1;
}

void Led::loop() {
    unsigned long current = millis();

    if (blinkBit) {
        if (current >= nextBlinkBitChange && (!nextBlinkBitChangeOverflow || current < UINT32_MAX - LED_BLINK_INTERVAL + nextBlinkBitChange)) {
            nextBlinkBitChange = current + LED_BLINK_INTERVAL;
            nextBlinkBitChangeOverflow = UINT32_MAX - current <= LED_BLINK_INTERVAL;

            digitalWrite(pin, (blinkBit-- % 2) && LED_ON);
        }

        // while blinking, ignore current LED pattern
        return;
    }

    if (current >= nextPatternBitChange && (!nextPatternBitChangeOverflow || current < UINT32_MAX - LED_PATTERN_INTERVAL + nextPatternBitChange)) {
        nextPatternBitChange = current + LED_PATTERN_INTERVAL;
        nextPatternBitChangeOverflow = UINT32_MAX - current <= LED_PATTERN_INTERVAL;

        digitalWrite(pin, (pattern >> patternBit) && LED_ON);
        if (++patternBit >=8) {
            patternBit = 0;
        }
    }

}