---
type: Architecture
belongs_to: "[[overview|Architecture Overview]]"
related_to:
  - "[[badger-nfc|NFC Emulation]]"
  - "[[momentum-sdk|Momentum SDK]]"
tags:
  - nfc
  - emulation
  - ntag213
---

# NFC Emulation

The badge emulates an **NTAG213** (MIFARE Ultralight, ISO14443-3A) carrying the
NDEF URL from [[ndef-url|NDEF URL Encoding]].

## Why NTAG213

- Small, ubiquitous, universally readable by phones.
- Simple memory map (CC page + user pages) — no sector trailers or auth.
- Directly supported by the firmware `nfc` library listener.

## Emulation path (mirrors the official NFC app)

```c
Nfc* nfc = nfc_alloc();                                   // take NFC HAL lock
MfUltralightData* mfu = badger_nfc_build_ntag213(url);    // UID + CC + NDEF TLV
NfcListener* l = nfc_listener_alloc(nfc, NfcProtocolMfUltralight,
                                    (const NfcDeviceData*)mfu); // deep-copies data
nfc_listener_start(l, NULL, NULL);                        // spawn listener thread
// … on exit …
nfc_listener_stop(l);
nfc_listener_free(l);
mf_ultralight_free(mfu);
nfc_free(nfc);                                            // release NFC HAL lock
```

`nfc_listener_alloc` deep-copies the `MfUltralightData` (via
`nfc_device_set_data` → `mf_ultralight_copy`), so the caller's copy can be freed
immediately after `alloc`.

## NTAG213 data built by the app

| Page | Content |
|---|---|
| 0–2 | UID (0x04 prefix + random), internal byte 0x48, lock bytes |
| 3 | Capability Container `E1 10 12 00` (0x12×8 = 144 B user memory) |
| 4–39 | NDEF TLV (`03 <len> <record> FE`) |
| 41–43 | config (STRG_MOD_EN, AUTH0, VCTID, PWD) |

UID is randomized each session (NXP manufacturer byte + `furi_hal_random_fill_buf`).

## Hardware readiness

Before `nfc_alloc()`, check `furi_hal_nfc_is_hal_ready() == FuriHalNfcErrorNone`
(the official app's guard). The `Nfc*` object exclusively holds the NFC HAL until
`nfc_free()`, so it is allocated only while the emulate screen is active.
