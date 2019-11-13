//
// Created by naya.vu on 29.09.2019.
//

#include <Arduino.h>
#include <FS.h>
#include "StructPersister.h"

namespace StructPersister {
    byte calculateChecksum(byte* obj, size_t objLen) {
        uint8_t sum = 31;
        for (size_t i = 0; i < objLen; i++) {
            sum ^= obj[i];
        }
        return sum;
    }

    bool load(const char* filename, byte* obj, size_t objLen) {
        DEBUGV("Loading struct from %s\r\n", filename);
        File f = SPIFFS.open(filename, "r");
        if (!f) {
            DEBUGV("No such file\r\n");
            return false;
        }
        byte buf[objLen + 1];
        if (f.read(buf, sizeof(buf)) != sizeof(buf)) {
            f.close();
            DEBUGV("Failed to read %d bytes\r\n", sizeof(buf));
            return false;
        }
        if (calculateChecksum(buf, objLen) != buf[objLen]) {
            f.close();
            DEBUGV("Checksum verification failed\r\n");
            return false;
        }
        memcpy(obj, buf, objLen);

        f.close();
        DEBUGV("Ok\r\n");
        return true;
    }

    bool persist(const char* filename, byte* obj, size_t objLen) {
        DEBUGV("Saving struct to %s\r\n", filename);
        byte checksum = calculateChecksum(obj, objLen);
        DEBUGV("Checksum: %d\r\n", checksum);
        File f = SPIFFS.open(filename, "w");
        bool res = f.write(obj, objLen) == objLen && f.write(checksum) == 1;
        if (res) {
            DEBUGV("Ok\r\n");
        } else {
            DEBUGV("Failed\r\n");
        }
        f.close();
        return res;
    }
}
