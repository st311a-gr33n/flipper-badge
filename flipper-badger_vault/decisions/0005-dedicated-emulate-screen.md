---
type: Decision
id: ADR-0005
status: Accepted
date: 2026-09-28
area: ui
related_to:
  - "[[0001-single-view-app|ADR-0001 Single-view app]]"
  - "[[badger-nfc|NFC Emulation]]"
tags:
  - nfc
  - ui
---

# ADR-0005 — Dedicated emulate-NFC screen (OK key)

## Context

NFC emulation must be triggered somehow. Options: always-on, a third view in the
Right/Left rotation, or a dedicated screen.

## Decision

Press **OK** from the QR/contact view to enter a dedicated emulate screen; the
`Nfc*` object is allocated and the listener started on entry, torn down on exit
(Back). Magenta LED blink signals emulation.

## Consequences

- User controls when the NFC HAL is held (avoids constant lock/battery cost).
- Clear, discoverable interaction; no risk of accidental scanning.
- Slightly more taps than an always-on tag — acceptable.
