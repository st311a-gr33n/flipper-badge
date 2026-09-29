---
type: Architecture
belongs_to: "[[overview|Architecture Overview]]"
related_to:
  - "[[ndef-url|NDEF URL]]"
  - "[[ndef-spec|NDEF Spec]]"
tags:
  - ndef
  - nfc
  - url
---

# NDEF URL Encoding

The emulated tag carries a single NDEF **URI** record (Well-Known type `U`).

## Record layout

```
byte 0: 0xD1           flags/TNF (MB=1, ME=1, CF=0, SR=1, IL=0, TNF=1 Well-Known)
byte 1: 0x01           type length ("U")
byte 2: <payload len>  payload length (1 + url_len), short record (< 256)
byte 3: 'U'            type
payload: [prepend][url]
```

## URI prepend (identifier code)

The first payload byte is a compact scheme code:

| Code | Scheme |
|---|---|
| 0x00 | none (full URL in payload) |
| 0x01 | `http://www.` |
| 0x02 | `https://www.` |
| 0x03 | `http://` |
| 0x04 | `https://` |

`ndef_url_build()` strips a matching prefix and emits the corresponding code,
falling back to `0x00` with the full URL otherwise.

## TLV wrapper (Type-2 tag)

```
0x03            NDEF message TLV
<len>           message length
<record bytes>
0xFE            terminator
```

Length is a single byte while the message is < 255 bytes (the URL budget here).

## Budget

NTAG213 user memory is 144 bytes (pages 4–39). The encoder caps the URL so the
full TLV fits; over-long URLs are truncated or rejected (see gap for graceful
handling).
