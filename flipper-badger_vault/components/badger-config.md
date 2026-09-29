---
type: Component
belongs_to: "[[overview|Architecture Overview]]"
related_to:
  - "[[config-format|Config File Format]]"
tags:
  - config
  - flipperformat
---

# badger-config — Config Loader

`badger_config.c/h` — reads the `.badger` FlipperFormat file into `FuriString` fields.

## API

```c
bool badger_config_load(BadgerConfig* cfg, Storage* storage, const char* path);
void badger_config_free(BadgerConfig* cfg);
```

## Behavior

- Opens the file with `flipper_format_file_open_existing`.
- Validates `Filetype == Badger` and `Version <= 1`.
- Reads each known key with `flipper_format_read_string` (all optional except
  `Url`; missing keys remain empty strings).
- Returns `false` on header mismatch, missing `Url`, or IO error.

## Notes

- Single-line fields only (no multi-line continuation like `.qrcode` v2).
- Empty optional fields are skipped when rendering the contact card.
