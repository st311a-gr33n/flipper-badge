---
type: Progress
date: 2026-09-28
belongs_to: "[[Home|flipper-badger — Project Home]]"
related_to:
  - "[[current-state|Current State]]"
tags:
  - progress
---

# Worklog

Append-only session log. Never rewrite history.

## 2026-09-28

- Explored reference apps: `qrcode/` (bmatcuk) and `nfc/` (official Momentum).
- Confirmed external-FAP NFC access via the `nfc` SDK library
  (`lib/nfc/SConscript` `SDK_HEADERS`, `targets/f7/api_symbols.csv`).
- Finalized plan: single-view app, `.badger` config, NTAG213 NDEF-URL emulation,
  dedicated emulate screen (OK key).
- Scaffolded `flipper-badger_vault` (Phase 0).
- Implemented app source: `application.fam`, `flipper_badger_app.c`,
  `badger_config.c/h`, `ndef_url.c/h`, `badger_nfc.c/h`, vendored `qrcode.c/h`.
- Initialized Momentum firmware submodules and built with
  `FBT_TOOLCHAIN_PATH=~/.ufbt FBT_NO_SYNC=1 ./fbt fap_flipper_badger` → `flipper_badger.fap`
  (APPCHK passed).
- Fixed an exit crash: `badger_app_free` freed the QR/config/mutex before
  `gui_remove_view_port`, so in-flight render callbacks could touch freed memory.
  Reordered teardown (remove view port first, free mutex last) and added a
  defensive `sequence_blink_stop`.
- Fixed contact-card rendering: proper text baseline (name no longer clips off
  the top) and word/char wrapping for long fields (email, website, note).
- Silenced build warnings by fixing two third-party manifests in
  `applications/external` (`cli_bridge`, `mtp`) whose appids had a leading dot.
- Made the contact view scrollable (Up/Down) and moved the "Left: QR" legend to
  the last line, resolving GAP-005. Lines are built once per config load and
  cached in `contact_lines[]`; rendering draws a clamped scroll window.
- Pinned the `Name` as a non-scrolling `FontPrimary` header above the scrollable
  contact fields.
