---
type: Gap
id: GAP-005
priority: medium
status: Resolved
belongs_to: "[[flipper-badger-app|App Core]]"
tags:
  - ui
---

# GAP-005 — Contact card vertical overflow

The contact card wraps long fields (`draw_wrapped`), but if many fields wrap the
content can exceed the 64 px screen height and clip off the bottom (and overlap
the "Left: QR" footer).

**Resolved:** the contact view is now scrollable via Up/Down; the "Left: QR"
legend is the last line of the scrollable content.
