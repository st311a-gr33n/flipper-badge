---
type: Architecture
belongs_to: "[[overview|Architecture Overview]]"
related_to:
  - "[[qrcode|QR Code (vendored)]]"
  - "[[flipper-badger-app|App Core]]"
tags:
  - qr
  - rendering
---

# QR Rendering

The QR code is rendered with the vendored [[qrcode|QR library]] (ricmoo/QRCode,
same library used by the firmware's demo and bmatcuk's QR app).

## Pipeline

1. `qrcode_initBytes(&qr, modules, mode, version, ecc, data, len)` builds the
   module matrix.
2. `qrcode_getModule(&qr, x, y)` reports whether a module is dark.
3. The render callback draws each dark module as a box (`canvas_draw_box`) or dot
   (`canvas_draw_dot` when `pixel_size == 1`).

## Constraints (Flipper Zero 128×64 display)

- Maximum version 11 (61×61 modules) — version 12 is 65×65 and doesn't fit.
- `pixel_size = 64 / size`, centered vertically; horizontally centered on the
  left 65px when stats are shown, else full width.
- Auto-select the smallest version and highest ECC that fit (same heuristic as
  the QR app's `find_min_version_max_ecc`).

## Mode/ECC defaults

- Auto-detect mode (numeric → alphanumeric → binary).
- Default ECC high; fall back to lower ECC as needed to fit.
- The badge app does **not** expose the mode/version/ECC editor UI (see
  [[0001-single-view-app|ADR-0001]]).

## vCard option

A future enhancement can render the contact card as a multi-line vCard QR (the
vendored library supports multi-line binary payloads) — tracked as a gap.
