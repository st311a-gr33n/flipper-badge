---
type: Reference
kind: analysis
belongs_to: "[[nfc-emulation|NFC Emulation]]"
related_to:
  - "[[badger-nfc|NFC Emulation]]"
tags:
  - nfc
  - analysis
---

# NFC App Analysis (official Momentum NFC app)

Reference analysis of the official `nfc/` app included in the project folder.

## Key finding

The MF Ultralight emulation path is exactly:

```c
// nfc_scene_emulate_on_enter_mf_ultralight (helpers/protocol_support/mf_ultralight/mf_ultralight.c)
const MfUltralightData* data =
    nfc_device_get_data(instance->nfc_device, NfcProtocolMfUltralight);
instance->listener = nfc_listener_alloc(instance->nfc, NfcProtocolMfUltralight, data);
nfc_listener_start(instance->listener, NULL, NULL);
```

## Confirmed SDK surface (used by the badge)

- `nfc_alloc` / `nfc_free`
- `nfc_listener_alloc` / `nfc_listener_start` / `nfc_listener_stop` / `nfc_listener_free`
- `nfc_data_generator_fill_data` (blank NTAG generation — the badge builds its own
  NTAG with NDEF instead)
- `mf_ultralight_alloc` / `mf_ultralight_set_uid` / `mf_ultralight_free`
- `furi_hal_nfc_is_hal_ready`

All exported in `targets/f7/api_symbols.csv`; headers listed in
`lib/nfc/SConscript` `SDK_HEADERS`, so an external FAP links them via
`fap_libs=["nfc"]`.

## NDEF parsing reference

`plugins/supported_cards/ndef.c` documents the URI prepend table (`ndef_uri_prepends`)
and TLV layout used by [[ndef-url|NDEF URL Encoding]].
