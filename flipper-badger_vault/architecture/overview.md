---
type: Architecture
belongs_to: "[[Home|flipper-badger — Project Home]]"
related_to:
  - "[[config-format|Config File Format]]"
  - "[[qr-rendering|QR Rendering]]"
  - "[[ndef-url|NDEF URL Encoding]]"
  - "[[nfc-emulation|NFC Emulation]]"
tags:
  - architecture
  - qr
  - nfc
---

# Architecture Overview

**flipper-badger** is a standalone external Flipper Zero app (FAP) that combines
three capabilities into a single screen flow:

1. **QR code** — render a URL (or arbitrary data) as a QR code on the 128×64 display.
2. **Contact card** — show name/title/phone/email/website/note as text.
3. **NFC emulation** — emulate an NTAG213 carrying an NDEF URL record.

## Design goals

- Single, self-contained FAP in `applications_user/flipper_badger/`.
- Reuse the proven QR library (vendored `qrcode.c/h` from bmatcuk/flipperzero-qrcode).
- Link against the firmware `nfc` SDK library for tag emulation (no custom HAL code).
- Data-driven: one `.badger` config file on the SD card drives QR, contact, and NFC.

## Data flow

```
.badger file (FlipperFormat)
        │ badger_config_load()
        ▼
BadgerApp (holds FuriString fields + QRCode)
        ├─ render QR   ← qrcode_initBytes() + qrcode_getModule()
        ├─ render text ← contact fields
        └─ emulate NFC ← badger_nfc_start(url) → nfc_listener_alloc/start
```

## Screen flow (input state machine)

| Key | Action |
|---|---|
| (default) | show QR |
| Right | show contact card |
| Left | back to QR |
| OK | enter emulate-NFC screen |
| Back | exit emulate (→ QR) / exit app from QR |

## Modules

- [[flipper-badger-app|App Core]] — `flipper_badger_app.c`
- [[badger-config|Config Loader]] — `badger_config.c/h`
- [[ndef-url|NDEF URL]] — `ndef_url.c/h`
- [[badger-nfc|NFC Emulation]] — `badger_nfc.c/h`
- [[qrcode|QR Code (vendored)]] — `qrcode.c/h`
