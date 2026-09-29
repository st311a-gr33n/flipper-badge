---
type: Progress
date: 2026-09-28
belongs_to: "[[Home|flipper-badger — Project Home]]"
related_to:
  - "[[phases|Phases]]"
tags:
  - progress
---

# Changelog

User-facing behavior changes (append newest first).

## 2026-09-28

- Initialized `flipper-badger_vault` (Phase 0).
- Implemented the flipper-badger app (Phases 1–5): QR code, contact card, NFC
  NTAG213 NDEF-URL emulation; builds to `flipper_badger.fap`.
- Fixed a crash-on-exit caused by freeing the app mutex/state before removing the
  view port.
- Contact card now wraps long fields and the name no longer clips off the top.
- Contact card is scrollable with Up/Down; the "Left: QR" hint sits at the end of
  the list.
- Name is now a non-scrolling `FontPrimary` header above the scrollable fields.
