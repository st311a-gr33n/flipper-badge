---
type: Progress
date: 2026-09-28
belongs_to: "[[Home|flipper-badger — Project Home]]"
related_to:
  - "[[phases|Phases]]"
  - "[[gap-registry|Gap Registry]]"
tags:
  - progress
---

# Current State

**Single source of current truth** — read this first.

## Summary

- **Phase:** Phase 5 complete — app builds cleanly to `flipper_badger.fap`.
- **Repo:** `flipper-badger` (working folder holds `nfc/`, `qrcode/` reference apps).
- **App code:** `Momentum-Firmware/applications_user/flipper_badger/`.
- **Firmware:** Momentum (`~/Documents/sg-git/flipper-projects/Momentum-Firmware/`).

## What works

- Vault scaffolded: `AGENTS.md`, `Home.md`, types, architecture, components,
  decisions (ADR-0001…0005), references, progress, gaps, views.
- App implemented and compiled cleanly (no build warnings):
  - `application.fam`, vendored `qrcode.c/h`, icon.
  - `badger_config.c/h` — `.badger` FlipperFormat loader.
  - `ndef_url.c/h` — NDEF URI record + Type-2 TLV encoder.
  - `badger_nfc.c/h` — NTAG213 build + `nfc_listener` emulation.
  - `flipper_badger_app.c` — QR / contact / emulate view state machine.
- Build: `./fbt fap_flipper_badger` → `build/f7-firmware-C/.extapps/flipper_badger.fap`
  (APPCHK passed).

## Verified on hardware

- App launches, QR / contact / NFC screens work; exit no longer crashes.

## Fixed during bring-up

- Exit crash: `badger_app_free` freed state before `gui_remove_view_port`; now the
  view port is removed first (synchronizing with in-flight draws) and the mutex
  freed last.
- Contact card: name no longer clips off the top (proper baseline); long fields
  (email, website, note) wrap via `draw_wrapped()`.
- Build warnings from third-party `applications/external` manifests (`cli_bridge`,
  `mtp`) fixed by removing invalid leading-dot appids.
- Contact card made scrollable (Up/Down) with the "Left: QR" hint at the very end,
  resolving [[gap-005-contact-overflow|GAP-005]].

## Test config

`stella-green.badger` in the project folder (`flipper-badger/`) is a template.
Copy to `/ext/apps_data/flipper_badger/` to load it on device.

## Not yet verified on hardware

- Phone reads the QR and the emulated NTAG213 NDEF URL.

## Active gaps

See [[gap-registry|Gap Registry]].

## Next actions

1. Verify a phone reads the QR and the emulated NFC URL.
