#pragma once

#include <furi.h>
#include <storage/storage.h>

typedef struct {
    FuriString* url;
    FuriString* name;
    FuriString* title;
    FuriString* phone;
    FuriString* email;
    FuriString* website;
    FuriString* note;
} BadgerConfig;

BadgerConfig* badger_config_alloc(void);

void badger_config_free(BadgerConfig* config);

bool badger_config_load(BadgerConfig* config, Storage* storage, const char* path);
