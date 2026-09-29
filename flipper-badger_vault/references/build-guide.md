---
type: Reference
kind: guide
belongs_to: "[[flipper-badger-app|App Core]]"
related_to:
  - "[[momentum-sdk|Momentum SDK]]"
tags:
  - build
  - guide
---

# Build Guide

## Build (from the firmware tree)

```bash
cd ~/Documents/sg-git/flipper-projects/Momentum-Firmware
./fbt fap_flipper_badger
```

Output: `build/f7-firmware-C/.extapps/flipper_badger.fap` (fbt prints the path).

Fast rebuild (reuses the installed ufbt toolchain and skips the submodule sync):

```bash
FBT_TOOLCHAIN_PATH=~/.ufbt FBT_NO_SYNC=1 ./fbt fap_flipper_badger
```

## Install

Copy `flipper_badger.fap` to `apps/Tools/` on the SD card (via qFlipper).

## Config

Create `/ext/apps_data/flipper_badger/<name>.badger`:

```
Filetype: Badger
Version: 1
Url: https://example.com
Name: John Smith
Title: Engineer
Phone: +18005551212
Email: john@example.com
Website: https://example.com
Note: Hi, I'm a badge
```

## Icon

`icons/badger_10px.png` (10×10, 1-bit) referenced as `fap_icon` in
`application.fam`; `fap_icon_assets="icons"` generates `flipper_badger_icons.h`
(symbol `I_badger_10px`).
