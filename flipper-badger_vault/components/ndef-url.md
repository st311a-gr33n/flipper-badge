---
type: Component
belongs_to: "[[overview|Architecture Overview]]"
related_to:
  - "[[ndef-url|NDEF URL Encoding]]"
tags:
  - ndef
  - nfc
---

# ndef-url — NDEF URL Encoder

`ndef_url.c/h` — builds a single-record NDEF URI message wrapped in a Type-2 TLV.

## API

```c
size_t ndef_url_build(const char* url, uint8_t* out, size_t out_size);
```

Returns the number of bytes written (TLV + terminator), or 0 if the URL is too
long for `out_size`.

## Implementation detail

- Detect the URI scheme prefix and map to the compact prepend code (0x01–0x04),
  else `0x00` + full URL.
- Emit `0xD1 0x01 <len> 'U' <prepend><url>` then wrap as `0x03 <msg_len> … 0xFE`.
- Pure function — no heap, no Flipper dependencies, unit-testable off-device.
