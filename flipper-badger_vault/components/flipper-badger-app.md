---
type: Component
belongs_to: "[[overview|Architecture Overview]]"
related_to:
  - "[[badger-config|Config Loader]]"
  - "[[badger-nfc|NFC Emulation]]"
  - "[[qrcode|QR Code (vendored)]]"
tags:
  - ui
  - app-core
---

# flipper-badger-app — App Core

`flipper_badger_app.c` — entry point (`badger_app`) and UI state machine.

## Responsibilities

- Allocate/free the app instance (message queue, view port, gui, mutex).
- Load the config file (direct arg, or file browser fallback).
- Run the input loop: Right → contact, Left → QR, OK → emulate NFC, Back → exit.
- Render the active view (QR / contact / emulate) to the canvas.

## Structure (mirrors `qrcode_app.c`)

```c
typedef struct {
    FuriMessageQueue* input_queue;
    Gui* gui;
    ViewPort* view_port;
    FuriMutex* mutex;

    FuriString* url, *name, *title, *phone, *email, *website, *note;
    QRCode* qrcode;
    BadgerView view;   // Qr | Contact | Emulate
    bool error;
    BadgerNfc* nfc;
} BadgerApp;
```

## Key constants

- `BADGER_FOLDER` = `APP_DATA_PATH("flipper_badger")` (i.e. `/data/flipper_badger`)
- `BADGER_EXTENSION` = `.badger`
- `BADGER_FILETYPE` = `Badger`, `BADGER_FILE_VERSION` = 1

## Notes

- Contact rendering builds a flat list of wrapped lines once per config load
  (`contact_lines_build` → `contact_lines_add_wrapped`, cached in
  `contact_lines[]`). Rendering draws a clamped scroll window; Up/Down changes
  `contact_scroll`. The "Left: QR" hint is the last line. The `Name` field is a
  non-scrolling `FontPrimary` header pinned above the scrollable fields.
- Teardown order in `badger_app_free`: `gui_remove_view_port` first (synchronizes
  with in-flight draws), then free shared state, then `view_port_free`,
  `furi_message_queue_free`, and `furi_mutex_free` last. Freeing the mutex before
  removing the view port caused a crash on exit.
