---
type: Component
belongs_to: "[[overview|Architecture Overview]]"
related_to:
  - "[[nfc-emulation|NFC Emulation]]"
  - "[[ndef-url|NDEF URL]]"
tags:
  - nfc
  - emulation
---

# badger-nfc — NFC Emulation

`badger_nfc.c/h` — builds an NTAG213 with an NDEF URL and runs the listener.

## API

```c
typedef struct BadgerNfc BadgerNfc;

BadgerNfc* badger_nfc_alloc(void);
void       badger_nfc_free(BadgerNfc* instance);
bool       badger_nfc_start(BadgerNfc* instance, const char* url);
void       badger_nfc_stop(BadgerNfc* instance);
```

## Behavior

- `start`: check `furi_hal_nfc_is_hal_ready()`, `nfc_alloc()`, build
  `MfUltralightData` (NTAG213), `nfc_listener_alloc(... NfcProtocolMfUltralight …)`,
  `nfc_listener_start(listener, NULL, NULL)`.
- `stop`: `nfc_listener_stop`, `nfc_listener_free`, free built data, `nfc_free`.
- The `Nfc*` HAL lock is held only while emulating.

## Dependencies

- `nfc` SDK library (`nfc_alloc`, `nfc_listener_*`), `mf_ultralight` protocol,
  `furi_hal_nfc`, `furi_hal_random`.
