//
// Created by naya.vu on 29.09.2019.
//

#ifndef REMOTE_CONTROL_STRUCTPERSISTER_H
#define REMOTE_CONTROL_STRUCTPERSISTER_H

#include <Arduino.h>

namespace StructPersister {
    byte calculateChecksum(byte* config, size_t len);
    bool load(const char* filename, byte* obj, size_t objLen);
    bool persist(const char* filename, byte* obj, size_t objLen);
};

#endif //REMOTE_CONTROL_STRUCTPERSISTER_H
