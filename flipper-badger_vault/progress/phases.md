---
type: Progress
date: 2026-09-28
belongs_to: "[[Home|flipper-badger — Project Home]]"
related_to:
  - "[[current-state|Current State]]"
tags:
  - progress
---

# Phases

## Phase 0 — Vault scaffold (complete)

Create `flipper-badger_vault/` with conventions, types, views, and initial notes.

## Phase 1 — App scaffold (complete)

`application.fam`, icon, `flipper_badger_app.c` skeleton, vendored `qrcode.c/h`.

## Phase 2 — Config + QR (complete)

`badger_config.c/h` loader; render the QR from `Url`.

## Phase 3 — Contact view (complete)

Contact-card rendering + Right/Left navigation.

## Phase 4 — NFC emulation (complete)

`ndef_url.c/h` encoder + `badger_nfc.c/h` NTAG213 emulation + OK-to-emulate screen.

## Phase 5 — Build & test (complete)

`./fbt fap_flipper_badger` → `flipper_badger.fap` (clean, no warnings). On-device:
screens work, exit no longer crashes. Phone-read of QR/NFC still to confirm.
