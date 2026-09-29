---
type: Reference
kind: spec
belongs_to: "[[ndef-url|NDEF URL Encoding]]"
related_to:
  - "[[ndef-url|NDEF URL]]"
tags:
  - ndef
  - spec
---

# NDEF Spec (URI record + Type-2 TLV)

## URI record (Well-Known type "U")

- TNF = 1 (Well-Known), type = `"U"`.
- Payload = `[identifier code][uri bytes]`.
- Identifier code maps to scheme prefixes (0x01 `http://www.`, 0x02 `https://www.`,
  0x03 `http://`, 0x04 `https://`, …).

## Type-2 tag TLV

- `0x03` NDEF message TLV, followed by 1- or 3-byte length.
- `0xFE` terminator.
- `0x00` padding, `0x01`/`0x02` lock/memory control (ignored by the badge).

## Capability Container (CC), page 3

`E1 10 <size> 00` where `<size>` = user-memory bytes / 8.

## Sources

- NFC Forum Type 2 Tag Operation spec.
- Adafruit PN532 NDEF guide (URI records / storing NDEF).
- `ndef.c` reference parser (URI prepend table).
