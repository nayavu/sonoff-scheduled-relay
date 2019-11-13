//
// Created by naya.vu on 14.11.2019.
//

#ifndef SONOFF_SCHEDULER_CONFIGMANAGER_H
#define SONOFF_SCHEDULER_CONFIGMANAGER_H


#include <FS.h>
#include "StructPersister.h"

template <typename Config> class ConfigManager {
protected:
    Config _config;
    const char* _file;
public:
    const Config* config;
    ConfigManager(const char* fileName) : _file(fileName), config(&_config) {};

    bool isPersisted();
    bool load();
    bool persist();
};


template <typename Config> bool ConfigManager<Config>::isPersisted() {
    return SPIFFS.exists(_file);
}

template <typename Config> bool ConfigManager<Config>::load() {
    return StructPersister::load(_file, (uint8_t*) &_config, sizeof(Config));
}

template <typename Config> bool ConfigManager<Config>::persist() {
    return StructPersister::persist(_file, (uint8_t*) &_config, sizeof(Config));
}

#endif //SONOFF_SCHEDULER_CONFIGMANAGER_H
