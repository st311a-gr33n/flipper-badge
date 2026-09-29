#include "badger_config.h"

#include <lib/flipper_format/flipper_format.h>

#define BADGER_FILETYPE     "Badger"
#define BADGER_FILE_VERSION 1

BadgerConfig* badger_config_alloc(void) {
    BadgerConfig* config = malloc(sizeof(BadgerConfig));

    config->url = furi_string_alloc();
    config->name = furi_string_alloc();
    config->title = furi_string_alloc();
    config->phone = furi_string_alloc();
    config->email = furi_string_alloc();
    config->website = furi_string_alloc();
    config->note = furi_string_alloc();

    return config;
}

void badger_config_free(BadgerConfig* config) {
    if(!config) return;

    furi_string_free(config->url);
    furi_string_free(config->name);
    furi_string_free(config->title);
    furi_string_free(config->phone);
    furi_string_free(config->email);
    furi_string_free(config->website);
    furi_string_free(config->note);

    free(config);
}

static bool badger_config_read_optional(
    FlipperFormat* file,
    const char* key,
    FuriString* out) {
    if(!flipper_format_key_exist(file, key)) {
        return true;
    }

    flipper_format_rewind(file);

    FuriString* tmp = furi_string_alloc();
    bool ok = flipper_format_read_string(file, key, tmp);
    if(ok) {
        furi_string_set(out, tmp);
    }
    furi_string_free(tmp);

    return ok;
}

bool badger_config_load(BadgerConfig* config, Storage* storage, const char* path) {
    furi_assert(config);
    furi_assert(storage);
    furi_assert(path);

    bool result = false;
    FuriString* temp_str = furi_string_alloc();
    FlipperFormat* file = flipper_format_file_alloc(storage);

    do {
        if(!flipper_format_file_open_existing(file, path)) break;

        uint32_t version = 0;
        if(!flipper_format_read_header(file, temp_str, &version)) break;
        if(furi_string_cmp_str(temp_str, BADGER_FILETYPE) || version > BADGER_FILE_VERSION) {
            break;
        }

        if(!badger_config_read_optional(file, "Url", config->url)) break;
        if(!badger_config_read_optional(file, "Name", config->name)) break;
        if(!badger_config_read_optional(file, "Title", config->title)) break;
        if(!badger_config_read_optional(file, "Phone", config->phone)) break;
        if(!badger_config_read_optional(file, "Email", config->email)) break;
        if(!badger_config_read_optional(file, "Website", config->website)) break;
        if(!badger_config_read_optional(file, "Note", config->note)) break;

        result = !furi_string_empty(config->url);
    } while(false);

    flipper_format_free(file);
    furi_string_free(temp_str);

    return result;
}
