---
type: Decision
id: ADR-0004
status: Accepted
date: 2026-09-28
area: nfc
related_to:
  - "[[ndef-url|NDEF URL Encoding]]"
tags:
  - ndef
  - nfc
---

# ADR-0004 — NDEF URL only (no vCard in the tag)

## Context

The tag could carry a URL, a vCard contact, or both.

## Decision

Emit a single NDEF **URI** record (Well-Known type `U`) — the custom URL — only.

## Consequences

- Matches the physical e-ink badge behavior ("scan to open a custom URL").
- Smallest, most reliable NDEF payload; best phone compatibility.
- Contact info remains display-only (Right key) and can later be added as a
  vCard QR or a second NDEF record if desired.
