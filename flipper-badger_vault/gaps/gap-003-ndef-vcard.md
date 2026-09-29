---
type: Gap
id: GAP-003
priority: feature
status: Open
belongs_to: "[[0004-url-only-ndef|ADR-0004 URL-only NDEF]]"
tags:
  - nfc
  - vcard
---

# GAP-003 — NDEF vCard record

Extend the tag to optionally carry a vCard contact record (Media Type
`text/vcard`) alongside or instead of the URL record. Larger payload; may need
NTAG215/216 or a Type-4 tag.
