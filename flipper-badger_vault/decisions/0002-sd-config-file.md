---
type: Decision
id: ADR-0002
status: Accepted
date: 2026-09-28
area: config
related_to:
  - "[[config-format|Config File Format]]"
tags:
  - config
---

# ADR-0002 — SD-card config file (.badger)

## Context

The URL + contact data must come from somewhere. On-device `text_input` for many
fields is awkward and slow; a config file matches the existing QR app pattern.

## Decision

Use a single `.badger` FlipperFormat file on the SD card
(`/data/flipper_badger/`), edited via qFlipper/PC. The app opens a file browser
when launched without an argument.

## Consequences

- Simple, familiar UX (same as `.qrcode` files).
- No on-device keyboard burden for long URLs.
- Requires a computer/qFlipper to change data — acceptable for a badge.
