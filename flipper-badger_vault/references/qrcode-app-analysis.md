---
type: Reference
kind: analysis
belongs_to: "[[qr-rendering|QR Rendering]]"
related_to:
  - "[[qrcode|QR Code (vendored)]]"
tags:
  - qr
  - analysis
---

# QR App Analysis (bmatcuk/flipperzero-qrcode)

Reference analysis of the reference `qrcode/` app included in the project folder.

## What it does

- Single `view_port` app: `qrcode_app.c` (~910 lines).
- Loads `.qrcode` files via file browser, renders the QR, Right→stats, Left→hide.
- Vendors `qrcode.c/h` (ricmoo/QRCode) with `qrcode_initBytes` / `qrcode_getModule`.

## Reused by flipper-badger

- `qrcode.c/h` vendored as-is.
- `find_min_version_max_ecc` heuristic (smallest version, highest ECC that fit).
- Render loop (`pixel_size`, centered, `canvas_draw_box`/`draw_dot`).
- `qrcode_alloc`/`qrcode_free` + `rebuild_qrcode` pattern.

## Not reused

- The mode/version/ECC stats editor UI (out of scope for the badge).
- `.qrcode` multi-line message continuation (badge uses named keys instead).
