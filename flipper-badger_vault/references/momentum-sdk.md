---
type: Reference
kind: sdk
belongs_to: "[[nfc-emulation|NFC Emulation]]"
related_to:
  - "[[nfc-app-analysis|NFC App Analysis]]"
tags:
  - nfc
  - sdk
---

# Momentum SDK Notes

Environment: `~/Documents/sg-git/flipper-projects/Momentum-Firmware/`.

## External FAP NFC access

- `lib/nfc/SConscript` declares `SDK_HEADERS` (incl. `nfc.h`, `nfc_listener.h`,
  `nfc_device.h`, `mf_ultralight.h`, `nfc_data_generator.h`).
- Symbols in `targets/f7/api_symbols.csv`: `nfc_alloc`, `nfc_listener_alloc/start/stop/free`,
  `nfc_data_generator_fill_data`, `furi_hal_nfc_is_hal_ready`, etc.
- `nfc` is in `targets/f7/target.json` `linker_dependencies`.
- Therefore an external app uses `fap_libs=["nfc"]` and `#include <nfc/nfc.h>` etc.

## Key types/signatures

- `NfcListener* nfc_listener_alloc(Nfc*, NfcProtocol, const NfcDeviceData*)`.
- `nfc_device_set_data` deep-copies via `nfc_devices[protocol]->copy`.
- `MfUltralightData { Iso14443_3aData* iso14443_3a_data; MfUltralightType type;
  MfUltralightVersion version; MfUltralightPage page[510]; uint16_t pages_read, pages_total; … }`.
- `Iso14443_3aData { uint8_t uid[10]; uint8_t uid_len; uint8_t atqa[2]; uint8_t sak; }`.

## Storage macros

- `STORAGE_APP_DATA_PATH_PREFIX` = `/data` (= `/ext/apps_data`).
- `APP_DATA_PATH(path)` = `/data/path`; badge folder = `APP_DATA_PATH("flipper_badger")`.
