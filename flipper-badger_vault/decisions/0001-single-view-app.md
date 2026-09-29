---
type: Decision
id: ADR-0001
status: Accepted
date: 2026-09-28
area: ui
related_to:
  - "[[overview|Architecture Overview]]"
tags:
  - ui
---

# ADR-0001 — Single view_port app (no scene manager)

## Context

The badge needs a simple three-state flow (QR → contact → emulate NFC). The
official NFC app uses a heavy scene-manager + plugin architecture.

## Decision

Implement flipper-badger as a single `view_port` app with an input loop and a
small view-state enum, mirroring `qrcode_app.c`.

## Consequences

- Minimal boilerplate; fast to build and review.
- No scene manager, submenu, or plugin scaffolding.
- Easily extended to more views later if needed (would then justify a refactor).
