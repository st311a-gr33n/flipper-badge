#include <furi.h>
#include <furi_hal.h>

#include <dialogs/dialogs.h>
#include <gui/gui.h>
#include <storage/storage.h>
#include <notification/notification.h>
#include <notification/notification_messages.h>

#include <flipper_badger_icons.h>

#include "qrcode.h"
#include "badger_config.h"
#include "ndef_url.h"
#include "badger_nfc.h"

#define TAG "badger"

#define BADGER_FOLDER       APP_DATA_PATH("flipper_badger")
#define BADGER_EXTENSION    ".badger"

/** Valid modes are Numeric (0), Alpha-Numeric (1), and Binary (2) */
#define MAX_QRCODE_MODE 2

/** Max version is 11 (61x61) because the screen is 64 px high. */
#define MAX_QRCODE_VERSION 11

/** Valid ECC levels are Low (0), Medium (1), Quartile (2), High (3) */
#define MAX_QRCODE_ECC 3

/** Max number of rendered lines in the scrollable contact view */
#define MAX_CONTACT_LINES 64

static const uint16_t MAX_LENGTH[3][4][MAX_QRCODE_VERSION] = {
    {
        {41, 77, 127, 187, 255, 322, 370, 461, 552, 652, 772},
        {34, 63, 101, 149, 202, 255, 293, 365, 432, 513, 604},
        {27, 48, 77, 111, 144, 178, 207, 259, 312, 364, 427},
        {17, 34, 58, 82, 106, 139, 154, 202, 235, 288, 331},
    },
    {
        {25, 47, 77, 114, 154, 195, 224, 279, 335, 395, 468},
        {20, 38, 61, 90, 122, 154, 178, 221, 262, 311, 366},
        {16, 29, 47, 67, 87, 108, 125, 157, 189, 221, 259},
        {10, 20, 35, 50, 64, 84, 93, 122, 143, 174, 200},
    },
    {
        {17, 32, 53, 78, 106, 134, 154, 192, 230, 271, 321},
        {14, 26, 42, 62, 84, 106, 122, 152, 180, 213, 251},
        {11, 20, 32, 46, 60, 74, 86, 108, 130, 151, 177},
        {7, 14, 24, 34, 44, 58, 64, 84, 98, 119, 137},
    },
};

typedef enum {
    BadgerViewQr,
    BadgerViewContact,
    BadgerViewEmulate,
} BadgerView;

typedef struct {
    FuriMessageQueue* input_queue;
    Gui* gui;
    ViewPort* view_port;
    FuriMutex* mutex;
    NotificationApp* notifications;

    BadgerConfig* config;
    QRCode* qrcode;
    BadgerView view;
    bool error;
    bool too_long;

    FuriString* contact_lines[MAX_CONTACT_LINES];
    uint32_t contact_line_count;
    bool contact_dirty;
    int32_t contact_scroll;

    BadgerNfc* nfc;
} BadgerApp;

static bool is_numeric(const char* str, uint16_t len) {
    while(len > 0) {
        char c = str[--len];
        if(c < '0' || c > '9') return false;
    }
    return true;
}

static bool is_alphanumeric(const char* str, uint16_t len) {
    while(len > 0) {
        char c = str[--len];
        if(c >= '0' && c <= '9') continue;
        if(c >= 'A' && c <= 'Z') continue;
        if(c == ' ' || c == '$' || c == '%' || c == '*' || c == '+' || c == '-' || c == '.' ||
           c == '/' || c == ':')
            continue;
        return false;
    }
    return true;
}

static QRCode* qrcode_alloc(uint8_t version) {
    QRCode* qrcode = malloc(sizeof(QRCode));
    qrcode->modules = malloc(qrcode_getBufferSize(version));
    return qrcode;
}

static void qrcode_free(QRCode* qrcode) {
    if(!qrcode) return;
    free(qrcode->modules);
    free(qrcode);
}

static bool find_min_version_max_ecc(uint16_t len, uint8_t mode, uint8_t* version, uint8_t* ecc) {
    *ecc = ECC_LOW;
    *version = 0;
    while(*version < MAX_QRCODE_VERSION && MAX_LENGTH[mode][*ecc][*version] < len) {
        (*version)++;
    }
    if(*version == MAX_QRCODE_VERSION) {
        return false;
    }

    *ecc = ECC_HIGH;
    while(MAX_LENGTH[mode][*ecc][*version] < len) {
        (*ecc)--;
    }
    (*version)++;

    return true;
}

static bool rebuild_qrcode(BadgerApp* instance, uint8_t mode, uint8_t version, uint8_t ecc) {
    const char* cstr = furi_string_get_cstr(instance->config->url);
    uint16_t len = (uint16_t)furi_string_size(instance->config->url);

    instance->qrcode = qrcode_alloc(version);
    int8_t res = qrcode_initBytes(
        instance->qrcode,
        instance->qrcode->modules,
        (int8_t)mode,
        version,
        ecc,
        (uint8_t*)cstr,
        len);

    if(res != 0) {
        FURI_LOG_E(TAG, "Could not create qrcode");
        qrcode_free(instance->qrcode);
        instance->qrcode = NULL;
        return false;
    }

    return true;
}

static void build_qrcode(BadgerApp* instance) {
    if(instance->qrcode) {
        qrcode_free(instance->qrcode);
        instance->qrcode = NULL;
    }
    instance->too_long = false;

    const char* cstr = furi_string_get_cstr(instance->config->url);
    uint16_t len = (uint16_t)furi_string_size(instance->config->url);
    if(len == 0) {
        instance->error = true;
        return;
    }

    int8_t min_mode = MODE_BYTE;
    if(is_numeric(cstr, len))
        min_mode = MODE_NUMERIC;
    else if(is_alphanumeric(cstr, len))
        min_mode = MODE_ALPHANUMERIC;

    int8_t max_mode = MAX_QRCODE_MODE;
    uint8_t version = 0;
    uint8_t ecc = 0;
    while(max_mode >= min_mode &&
          !find_min_version_max_ecc(len, (uint8_t)max_mode, &version, &ecc)) {
        max_mode--;
    }

    if(max_mode < min_mode) {
        instance->too_long = true;
        return;
    }

    if(!find_min_version_max_ecc(len, (uint8_t)max_mode, &version, &ecc)) {
        instance->too_long = true;
        return;
    }

    if(!rebuild_qrcode(instance, (uint8_t)max_mode, version, ecc)) {
        instance->error = true;
    }
}

static void render_qr(Canvas* canvas, BadgerApp* instance) {
    if(instance->qrcode) {
        uint8_t size = instance->qrcode->size;
        uint8_t width = canvas_width(canvas);
        uint8_t height = canvas_height(canvas);
        uint8_t pixel_size = height / size;
        uint8_t top = (height - pixel_size * size) / 2;
        uint8_t left = (width - pixel_size * size) / 2;

        for(uint8_t y = 0; y < size; y++) {
            for(uint8_t x = 0; x < size; x++) {
                if(qrcode_getModule(instance->qrcode, x, y)) {
                    if(pixel_size == 1) {
                        canvas_draw_dot(canvas, left + x * pixel_size, top + y * pixel_size);
                    } else {
                        canvas_draw_box(
                            canvas,
                            left + x * pixel_size,
                            top + y * pixel_size,
                            pixel_size,
                            pixel_size);
                    }
                }
            }
        }
    } else {
        uint8_t width = canvas_width(canvas);
        uint8_t height = canvas_height(canvas);
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str_aligned(
            canvas, width / 2, height / 2 - 4, AlignCenter, AlignCenter, "Could not load badge.");
        if(instance->too_long) {
            canvas_set_font(canvas, FontSecondary);
            canvas_draw_str_aligned(
                canvas, width / 2, height / 2 + 8, AlignCenter, AlignCenter, "URL is too long.");
        }
    }
}

static void contact_lines_clear(BadgerApp* instance) {
    for(uint32_t i = 0; i < MAX_CONTACT_LINES; i++) {
        if(instance->contact_lines[i]) {
            furi_string_free(instance->contact_lines[i]);
            instance->contact_lines[i] = NULL;
        }
    }
    instance->contact_line_count = 0;
}

static bool contact_lines_add(BadgerApp* instance, const char* str, size_t len) {
    if(instance->contact_line_count >= MAX_CONTACT_LINES) return false;

    FuriString* line = furi_string_alloc();
    furi_string_set_strn(line, str, len);
    instance->contact_lines[instance->contact_line_count++] = line;

    return true;
}

static void contact_lines_add_wrapped(Canvas* canvas, BadgerApp* instance, const char* str) {
    const int32_t max_width = canvas_width(canvas) - 4;
    size_t len = strlen(str);
    size_t start = 0;

    FuriString* chunk = furi_string_alloc();

    while(start < len) {
        size_t end = start;
        while(end < len) {
            furi_string_set_strn(chunk, str + start, end - start + 1);
            if(canvas_string_width(canvas, furi_string_get_cstr(chunk)) > max_width) {
                break;
            }
            end++;
        }
        if(end == start) end = start + 1;

        contact_lines_add(instance, str + start, end - start);
        start = end;
    }

    furi_string_free(chunk);
}

static void contact_field_add(
    Canvas* canvas,
    BadgerApp* instance,
    FuriString* field,
    const char* label,
    FuriString* value) {
    if(furi_string_empty(value)) return;

    furi_string_printf(field, "%s%s", label, furi_string_get_cstr(value));
    contact_lines_add_wrapped(canvas, instance, furi_string_get_cstr(field));
}

static void contact_lines_build(Canvas* canvas, BadgerApp* instance) {
    contact_lines_clear(instance);

    const BadgerConfig* c = instance->config;

    FuriString* field = furi_string_alloc();
    contact_field_add(canvas, instance, field, "Title: ", c->title);
    contact_field_add(canvas, instance, field, "Phone: ", c->phone);
    contact_field_add(canvas, instance, field, "Email: ", c->email);
    contact_field_add(canvas, instance, field, "Web: ", c->website);
    contact_field_add(canvas, instance, field, "Note: ", c->note);
    furi_string_free(field);

    contact_lines_add(instance, "Left: QR", strlen("Left: QR"));
}

static void render_contact(Canvas* canvas, BadgerApp* instance) {
    const BadgerConfig* c = instance->config;
    const bool has_name = !furi_string_empty(c->name);

    // Build the scrollable field lines (measured in FontSecondary).
    if(instance->contact_dirty) {
        canvas_set_font(canvas, FontSecondary);
        contact_lines_build(canvas, instance);
        instance->contact_dirty = false;
    }

    // Non-scrolling name header.
    int32_t header_baseline = 0;
    if(has_name) {
        canvas_set_font(canvas, FontPrimary);
        header_baseline = canvas_current_font_height(canvas);
        canvas_draw_str(canvas, 2, header_baseline, furi_string_get_cstr(c->name));
    }

    canvas_set_font(canvas, FontSecondary);
    const int32_t line_height = canvas_current_font_height(canvas) + 1;
    const int32_t first_baseline = has_name ? (header_baseline + line_height + 1) : line_height;
    const int32_t max_baseline = canvas_height(canvas) - 2;
    int32_t visible = (max_baseline - first_baseline) / line_height + 1;
    if(visible < 1) visible = 1;

    int32_t total = (int32_t)instance->contact_line_count;
    int32_t max_scroll = total - visible;
    if(max_scroll < 0) max_scroll = 0;
    if(instance->contact_scroll < 0) instance->contact_scroll = 0;
    if(instance->contact_scroll > max_scroll) instance->contact_scroll = max_scroll;

    for(int32_t i = instance->contact_scroll; i < total; i++) {
        int32_t y = first_baseline + (i - instance->contact_scroll) * line_height;
        if(y > max_baseline) break;
        canvas_draw_str(canvas, 2, y, furi_string_get_cstr(instance->contact_lines[i]));
    }
}

static void render_emulate(Canvas* canvas, BadgerApp* instance) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 16, AlignCenter, AlignCenter, "Emulating NFC");

    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 64, 30, AlignCenter, AlignCenter, "Tap a phone to open:");
    canvas_draw_str_aligned(
        canvas, 64, 40, AlignCenter, AlignCenter, furi_string_get_cstr(instance->config->url));
    canvas_draw_str_aligned(canvas, 64, 54, AlignCenter, AlignCenter, "Back: stop");
}

static void render_callback(Canvas* canvas, void* ctx) {
    furi_assert(canvas);
    furi_assert(ctx);

    BadgerApp* instance = ctx;
    furi_check(furi_mutex_acquire(instance->mutex, FuriWaitForever) == FuriStatusOk);

    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontSecondary);

    switch(instance->view) {
    case BadgerViewContact:
        render_contact(canvas, instance);
        break;
    case BadgerViewEmulate:
        render_emulate(canvas, instance);
        break;
    case BadgerViewQr:
    default:
        render_qr(canvas, instance);
        break;
    }

    furi_mutex_release(instance->mutex);
}

static void input_callback(InputEvent* input_event, void* ctx) {
    furi_assert(input_event);
    furi_assert(ctx);

    if(input_event->type == InputTypeShort) {
        BadgerApp* instance = ctx;
        furi_message_queue_put(instance->input_queue, input_event, 0);
    }
}

static void badger_nfc_enter(BadgerApp* instance) {
    if(furi_string_empty(instance->config->url)) return;

    if(badger_nfc_start(instance->nfc, furi_string_get_cstr(instance->config->url))) {
        instance->view = BadgerViewEmulate;
        notification_message(instance->notifications, &sequence_blink_start_magenta);
    }
}

static void badger_nfc_exit(BadgerApp* instance) {
    badger_nfc_stop(instance->nfc);
    notification_message(instance->notifications, &sequence_blink_stop);
    instance->view = BadgerViewQr;
}

static BadgerApp* badger_app_alloc(void) {
    BadgerApp* instance = malloc(sizeof(BadgerApp));

    instance->input_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    instance->view_port = view_port_alloc();
    view_port_draw_callback_set(instance->view_port, render_callback, instance);
    view_port_input_callback_set(instance->view_port, input_callback, instance);

    instance->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(instance->gui, instance->view_port, GuiLayerFullscreen);

    instance->notifications = furi_record_open(RECORD_NOTIFICATION);

    instance->mutex = furi_mutex_alloc(FuriMutexTypeNormal);

    instance->config = badger_config_alloc();
    instance->qrcode = NULL;
    instance->view = BadgerViewQr;
    instance->error = false;
    instance->too_long = false;

    for(uint32_t i = 0; i < MAX_CONTACT_LINES; i++) {
        instance->contact_lines[i] = NULL;
    }
    instance->contact_line_count = 0;
    instance->contact_dirty = true;
    instance->contact_scroll = 0;

    instance->nfc = badger_nfc_alloc();

    return instance;
}

static void badger_app_free(BadgerApp* instance) {
    // Stop NFC emulation if still active (joins the listener thread).
    badger_nfc_free(instance->nfc);

    // Remove the view port first so the GUI thread stops invoking our
    // render/input callbacks (gui_remove_view_port synchronizes with in-flight draws).
    gui_remove_view_port(instance->gui, instance->view_port);
    furi_record_close(RECORD_GUI);

    notification_message(instance->notifications, &sequence_blink_stop);
    furi_record_close(RECORD_NOTIFICATION);

    // Now it is safe to free shared state referenced by the callbacks.
    if(instance->qrcode) qrcode_free(instance->qrcode);
    contact_lines_clear(instance);
    badger_config_free(instance->config);

    view_port_free(instance->view_port);
    furi_message_queue_free(instance->input_queue);
    furi_mutex_free(instance->mutex);

    free(instance);
}

int32_t badger_app(void* p) {
    BadgerApp* instance = badger_app_alloc();
    FuriString* file_path = furi_string_alloc();

    Storage* storage = furi_record_open(RECORD_STORAGE);
    storage_simply_mkdir(storage, BADGER_FOLDER);

    do {
        if(p && strlen(p)) {
            furi_string_set(file_path, (const char*)p);
        } else {
            furi_string_set(file_path, BADGER_FOLDER);

            DialogsFileBrowserOptions browser_options;
            dialog_file_browser_set_basic_options(&browser_options, BADGER_EXTENSION, &I_badger_10px);
            browser_options.hide_ext = true;
            browser_options.base_path = BADGER_FOLDER;

            DialogsApp* dialogs = furi_record_open(RECORD_DIALOGS);
            bool res = dialog_file_browser_show(dialogs, file_path, file_path, &browser_options);
            furi_record_close(RECORD_DIALOGS);

            if(!res) {
                FURI_LOG_E(TAG, "No file selected");
                break;
            }
        }

        if(!badger_config_load(instance->config, storage, furi_string_get_cstr(file_path))) {
            FURI_LOG_E(TAG, "Unable to load config");
            instance->error = true;
        } else {
            instance->error = false;
        }

        instance->contact_dirty = true;
        instance->contact_scroll = 0;
        build_qrcode(instance);

        InputEvent input;
        while(1) {
            if(furi_message_queue_get(instance->input_queue, &input, 100) == FuriStatusOk) {
                furi_check(furi_mutex_acquire(instance->mutex, FuriWaitForever) == FuriStatusOk);

                if(input.key == InputKeyBack) {
                    if(instance->view == BadgerViewEmulate) {
                        badger_nfc_exit(instance);
                    } else {
                        furi_mutex_release(instance->mutex);
                        break;
                    }
                } else if(input.key == InputKeyRight) {
                    instance->view = BadgerViewContact;
                    instance->contact_scroll = 0;
                } else if(input.key == InputKeyLeft) {
                    instance->view = BadgerViewQr;
                } else if(input.key == InputKeyUp) {
                    if(instance->view == BadgerViewContact) {
                        instance->contact_scroll--;
                    }
                } else if(input.key == InputKeyDown) {
                    if(instance->view == BadgerViewContact) {
                        instance->contact_scroll++;
                    }
                } else if(input.key == InputKeyOk) {
                    if(instance->view != BadgerViewEmulate) {
                        badger_nfc_enter(instance);
                    }
                }

                furi_mutex_release(instance->mutex);
            }

            view_port_update(instance->view_port);
        }

        if(p && strlen(p)) {
            // launched with an arg: exit instead of looping to the browser
            break;
        }
    } while(true);

    furi_record_close(RECORD_STORAGE);
    furi_string_free(file_path);
    badger_app_free(instance);

    return 0;
}
