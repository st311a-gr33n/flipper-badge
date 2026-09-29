---
type: Gap
id: GAP-001
priority: low
status: Open
belongs_to: "[[ndef-url|NDEF URL Encoding]]"
tags:
  - nfc
  - qr
---

# GAP-001 — Over-long URL handling

NDEF URL and QR both have length limits (144 B NTAG213 user memory; QR version 11
capacity). Current plan truncates/rejects over-long URLs silently. Should surface
a clear "message too long" message (like the QR app's `too_long` state).
