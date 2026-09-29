#pragma once

#include <stdbool.h>

typedef struct BadgerNfc BadgerNfc;

BadgerNfc* badger_nfc_alloc(void);

void badger_nfc_free(BadgerNfc* instance);

bool badger_nfc_start(BadgerNfc* instance, const char* url);

void badger_nfc_stop(BadgerNfc* instance);
