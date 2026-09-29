---
type: Architecture
belongs_to: "[[overview|Architecture Overview]]"
related_to:
  - "[[badger-config|Config Loader]]"
tags:
  - config
  - flipperformat
---

# Config File Format

The app is driven by a single FlipperFormat text file with the `.badger`
extension, stored under `/data/flipper_badger/` (i.e. `/ext/apps_data/flipper_badger/`).

## Format (Version 1)

```
Filetype: Badger
Version: 1
Url: https://example.com
Name: John Smith
Title: Engineer
Phone: +18005551212
Email: john@example.com
Website: https://example.com
Note: Hi, I'm a badge
```

## Field semantics

| Field | Required | Drives |
|---|---|---|
| `Url` | yes | QR code content **and** NDEF URL payload |
| `Name` | no | contact card |
| `Title` | no | contact card |
| `Phone` | no | contact card |
| `Email` | no | contact card |
| `Website` | no | contact card |
| `Note` | no | contact card |

## Parsing rules

- Read header: `Filetype` must equal `Badger`; `Version` ≤ 1.
- Missing/empty `Url` → app shows an error, no QR and no NFC.
- Contact fields are optional; empty fields are hidden from the contact card.
- Extra/unknown keys are ignored (forward compatible).

## Notes

- Modeled on the QR app's `.qrcode` files (`Filetype: QRCode` / `Message:`), but
  with named keys for the badge fields.
- Keeping it on the SD card (editable via qFlipper) avoids on-device text input
  complexity — see [[0002-sd-config-file|ADR-0002]].
