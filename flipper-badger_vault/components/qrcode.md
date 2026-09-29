---
type: Component
belongs_to: "[[overview|Architecture Overview]]"
related_to:
  - "[[qr-rendering|QR Rendering]]"
  - "[[qrcode-app-analysis|QR App Analysis]]"
tags:
  - qr
  - vendored
---

# qrcode — QR Code (vendored)

`qrcode.c/h` — vendored, unmodified copy of the ricmoo/QRCode library (MIT), as
shipped with bmatcuk/flipperzero-qrcode.

## API (used by the badge)

```c
uint16_t qrcode_getBufferSize(uint8_t version);
int8_t   qrcode_initBytes(QRCode*, uint8_t* modules, int8_t mode,
                          uint8_t version, uint8_t ecc, uint8_t* data, uint16_t len);
bool     qrcode_getModule(QRCode*, uint8_t x, uint8_t y);
```

## Notes

- Modes: `MODE_NUMERIC`(0), `MODE_ALPHANUMERIC`(1), `MODE_BYTE`(2).
- ECC levels: `ECC_LOW`(0) … `ECC_HIGH`(3).
- Vendored to keep the app self-contained (no external lib dependency).
- Licensed MIT — preserve the license header.
